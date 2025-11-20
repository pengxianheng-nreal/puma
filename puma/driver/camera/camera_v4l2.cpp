#include "camera_v4l2.h"
#if defined(PUMA_SYSTEM_LINUX) || defined(PUMA_SYSTEM_ANDROID)
#include <unistd.h>
#include <sys/mman.h>   /// use : mmap/munmap
#include <sys/ioctl.h>  /// use : ioctrl
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/select.h> /// use : select
#include <sys/stat.h>   /// use : stat
#include <pthread.h>    /// use : pthread

#include <dirent.h>     /// use : opendir readdir closedir
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdlib.h>     /// use : memcpy
#include <puma/util/log.h>


std::string fourcc(unsigned int format)
{
    char formatArray[] = { (char)(format&0xff), (char)((format>>8)&0xff), (char)((format>>16)&0xff), (char)((format>>24)&0xff), 0 };
    return std::string(formatArray, strlen(formatArray));
}

static int xioctl(int fh, int request, void *arg)
{
    int ret;
    do {
        ret = ioctl(fh, request, arg);
    } while (-1 == ret && EINTR == errno);
    return ret;
}

CameraV4l2::CameraV4l2(std::function<void (const GenericShmWrapper&)> _callback) :
    CameraDriver(_callback) {
}

void CameraV4l2::setCameraProperties(const CameraInfo& info) {
    CameraDriver::setCameraProperties(info);
    pix_format_.pixelformat = info_.uvc_fmt > 0 ? info_.uvc_fmt : V4L2_PIX_FMT_GREY;
    pix_format_.width = info_.width;
    pix_format_.height = info_.height;
    image_fps_ = info_.frame_rate;
    image_bytes_ = pix_format_.width * pix_format_.height * info_.channels;
    PUMA_LOG_INFO("CameraV4l2::setCameraProperties pix_format_.pixelformat = {}, pix_format_.width = {}, pix_format_.height = {}, image_fps_ = {}, image_bytes_ = {}",
        fourcc(pix_format_.pixelformat).c_str(), pix_format_.width, pix_format_.height, image_fps_, image_bytes_);
}

bool CameraV4l2::setOutsideMem(const void *ptr, size_t size) {
    PUMA_LOG_TRACE("SET OUTSIDE MEMORY");
    if (nullptr == ptr) {
        PUMA_LOG_TRACE("input user pointer is null");
        return false;
    }

    outside_buff_ptr = (uint8_t *)ptr;
    outside_buff_size_ = size;

    PUMA_LOG_TRACE("input ptr = {:p}, input size = {}, image_bytes_ = {}", ptr, size, image_bytes_);
    size_t request_num = outside_buff_size_ / image_bytes_;
    PUMA_LOG_TRACE("setOutsideMem {} User buffers, input_bytes = {}, image_bytes = {}",
        request_num, outside_buff_size_, image_bytes_);
    if (request_num < 4 || request_num > V4L2_BUF_NUM_) {
        request_num = V4L2_BUF_NUM_;
        outside_buff_ptr = nullptr;
        outside_buff_size_ = 0;
        PUMA_LOG_TRACE("allocate internal buffer");
        return false;
    }

    buffers_num_ = request_num;
    return true;
}

void CameraV4l2::setIOMethod(IO_Method _io_method) {
    // io_method_ = (outside_buff_ptr == nullptr)
    //              ? _io_method : IO_Method::IO_METHOD_USERPTR;
    io_method_ = _io_method;
}

void CameraV4l2::run() {
    do {
        while (running_) {
            timeval timeout = {1, 0};
            int isread = is_readable(&timeout);
            if (isread > 0) {
                if (read_frame() <= 0) {
                    PUMA_LOG_TRACE("FRAME SIZE <= 0");
                    if (ENODEV == errno) {
                        PUMA_LOG_FATAL("{}", strerror(errno));
                        std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    }
                }
            } else if (0 == isread) {
                PUMA_LOG_ERROR("Capture frame timeout, restarting devices");
                restart_flag_ = 1;
                break;
            } else {
                if (EINTR == errno) {
                    continue;
                } else {
                    PUMA_LOG_ERROR("Capture frame failure");
                    return;
                }
            }
        }

        if (0 == restart_flag_) {
            break;
        } else {
            restart_device();
        }

    }while(1);

    while (!memory_user_queue_.empty()) {
        struct v4l2_buffer tmp = memory_user_queue_.front();
        if (-1 == xioctl(camera_fd_, VIDIOC_QBUF, &tmp)) {
            PUMA_LOG_ERROR("VIDIOC_QBUF, errno={},{}", errno, strerror(errno));
        }
        memory_user_queue_.pop();
    }
    PUMA_LOG_INFO("Camera thread exit.");
}

CameraV4l2::~CameraV4l2() {
    PUMA_LOG_TRACE("CameraV4l2 Destructor");
    stop();
}

void CameraV4l2::stop() {
    PUMA_LOG_TRACE("CameraV4l2 stop");
    CameraDriver::stop();
    stop_device();
    close_device();
}

bool CameraV4l2::start() {
    PUMA_LOG_TRACE("CameraV4l2 start");
    if (false == find_device()) {
        PUMA_LOG_ERROR("Find device failure");
        return false;
    }

    if (false == open_device()) {
        close_device();
        return false;
    }

    if (false == init_device()) {
        close_device();
        return false;
    }

    if (false == start_device()) {
        stop_device();
        close_device();
        return false;
    }

    return CameraDriver::start();
}

void CameraV4l2::setAutoExposure(int _auto_exposure) {
    CameraDriver::setAutoExposure(_auto_exposure);
    PUMA_LOG_TRACE("setAutoExposure = {} --> {}", static_cast<int>(AE_mode_), _auto_exposure);

    if (-1 == camera_fd_ || false == isStreamOn_) {
        PUMA_LOG_TRACE("setAutoExposure, invalid device handle or not stream");
        return;
    }

    struct v4l2_contrl ctrl;
    ctrl.id = V4L2_CID_EXPOSURE_AUTO;

    if (-1 == xioctl(camera_fd_, VIDIOC_G_CTRL, &ctrl)) {
        PUMA_LOG_TRACE("Cannot get exposure mode for device:{}, {}", device_node_name_, strerror(errno));
        return;
    } else {
        PUMA_LOG_TRACE("get current exposure mode = {}", ctrl.value);
    }

    if (V4L2_EXPOSURE_AUTO == ctrl.value) {
        if (AE_mode_ == AEAGMode::AE_MODE_ISP) {
            PUMA_LOG_TRACE("AE_mode_ = {}", static_cast<int>(AE_mode_));
            return;
        }

        ctrl.id = V4L2_CID_EXPOSURE_AUTO;
        ctrl.value = V4L2_EXPOSURE_SHUTTER_PRIORITY;
        if (-1 == xioctl(camera_fd_, VIDIOC_S_CTRL, &ctrl)) {
            PUMA_LOG_TRACE("Cannot set exposure mode to manual for device:{}, {}", device_node_name_, strerror(errno));
            return;
        } else {
            PUMA_LOG_TRACE("Set exposure mode to manual for device:{} success", device_node_name_);
        }
    } else {
        if (AE_mode_ == AEAGMode::AE_MODE_ISP) {
            ctrl.id = V4L2_CID_EXPOSURE_AUTO;
            ctrl.value = V4L2_EXPOSURE_AUTO;
            if (-1 == xioctl(camera_fd_, VIDIOC_S_CTRL, &ctrl)) {
                PUMA_LOG_TRACE("Cannot set exposure mode to auto for device:{}, {}", device_node_name_, strerror(errno));
                return;
            } else {
                PUMA_LOG_TRACE("Set exposure mode to auto for device:{} success", device_node_name_);
            }
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void CameraV4l2::setExposureTime(double _exposure_time) {

    CameraDriver::setExposureTime(_exposure_time);
    PUMA_LOG_TRACE("setExposureTime = {} --> {}", exposure_time_, _exposure_time);

    if (-1 == camera_fd_ || false == isStreamOn_) {
        PUMA_LOG_TRACE("setExposureTime invalid device handle or not stream");
        return;
    }

    if (AE_mode_ == AEAGMode::AE_MODE_ISP) {
        PUMA_LOG_TRACE("AE_mode_ = {}", static_cast<int>(AE_mode_));
        return;
    }

    struct v4l2_contrl ctrl;
    ctrl.id = V4L2_CID_EXPOSURE_ABSOLUTE;
    if (-1 == xioctl(camera_fd_, VIDIOC_G_CTRL, &ctrl)) {
        PUMA_LOG_TRACE("Cannot get exposure absolute value for device:{}, {}", device_node_name_, strerror(errno));
        return;
    } else {
        PUMA_LOG_TRACE("get current exposure absolute value = {}", ctrl.value);
    }

    ctrl.id = V4L2_CID_EXPOSURE_ABSOLUTE;
    ctrl.value = exposure_time_ * 10;
    if (-1 == xioctl(camera_fd_, VIDIOC_S_CTRL, &ctrl)) {
        PUMA_LOG_TRACE("Cannot set exposure absolute value for device:{}, {}", device_node_name_, strerror(errno));
        return;
    } else {
        PUMA_LOG_TRACE("set exposure absolute value = {}", ctrl.value);
    }
}

void CameraV4l2::setGain(int _gain) {
    CameraDriver::setGain(_gain);
    PUMA_LOG_TRACE("setGain = {} --> {}", gain_, _gain);

    if (-1 == camera_fd_ || false == isStreamOn_) {
        PUMA_LOG_TRACE("setGain invalid device handle or not stream");
        return;
    }

    if (AE_mode_ == AEAGMode::AE_MODE_ISP) {
        PUMA_LOG_TRACE("AE_mode_ = {}", static_cast<int>(AE_mode_));
        return;
    }

    struct v4l2_contrl ctrl;
    ctrl.id = V4L2_CID_GAIN;

    if (-1 == xioctl(camera_fd_, VIDIOC_G_CTRL, &ctrl)) {
        PUMA_LOG_TRACE("Cannot get gain value for device:{}, {}", device_node_name_, strerror(errno));
        return;
    } else {
        PUMA_LOG_TRACE("get current gain = {}", ctrl.value);
    }

    ctrl.id = V4L2_CID_GAIN;
    ctrl.value = gain_;
    if (-1 == xioctl(camera_fd_, VIDIOC_S_CTRL, &ctrl)) {
        PUMA_LOG_TRACE("Cannot set gain to {} for device:{}, {}", ctrl.value, device_node_name_, strerror(errno));
    } else {
        PUMA_LOG_TRACE("Set gain to {} for device:{} success", ctrl.value, device_node_name_);
    }
}

bool CameraV4l2::valid() {
    bool success = find_device();
    if (success) {
        if(open_device()) {
            close_device();
            success = true;
        }
    }
    return success;
}

int CameraV4l2::is_readable(timeval* tv) {
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(camera_fd_, &fdset);
    return select(camera_fd_+1, &fdset, NULL, NULL, tv);
}

void CameraV4l2::frame_process(void* _frame, uint32_t _size, int _index) {
    if (_frame == NULL) {
        PUMA_LOG_ERROR("Frame pointer is null");
    }

    if (pix_format_.sizeimage != pix_format_.width * pix_format_.height * info_.channels) {
        PUMA_LOG_ERROR("Camera data_bytes {}, should be = {}, cur = {}", _size,
                    pix_format_.width * pix_format_.height * info_.channels,
                    pix_format_.sizeimage);
        return;
    }

    frame_t * ptr = (frame_t *)curr_frame_.reserved;
    curr_frame_.reserved_size = sizeof(frame_t);
    ptr->device_start_time = startTime();
    ptr->data = (uint8_t*)_frame;;
    ptr->data_bytes = pix_format_.sizeimage;
    ptr->height = pix_format_.height;
    ptr->width = pix_format_.width;
    ptr->step = pix_format_.bytesperline;
    ptr->bytes_per_pixel = pix_format_.bytesperline / pix_format_.width; //XXX: xreal use it as camera_num
    ptr->sof = 0;

    if (io_method_ == IO_Method::IO_METHOD_USERPTR && outside_buff_ptr != nullptr){
        /// IO_METHOD_USERPTR & use outside shared memory
        curr_frame_.index = _index;
        curr_frame_.size = _size;
        curr_frame_.type = 0;
    } else {
        curr_frame_.type = 1;
    }

    if (frame_callback_) {
        frame_callback_(curr_frame_);
       // softAEAG(*ptr);
    } else {
        PUMA_LOG_WARN("NO available callback function");
    }
}

bool CameraV4l2::ConvertImageData(void* _frame, uint32_t _size){
    PUMA_LOG_TRACE("ConvertImageData _size:{},{}",_size,converted_image_data_.size());
    if(_size != converted_image_data_.size()) {
        PUMA_LOG_ERROR("ConvertImageData check size failed! _size:{},{}",_size,converted_image_data_.size());
        return false;
    }

    copy_frame_((uint8_t *)_frame, (uint8_t *)converted_image_data_.data(), _size, pix_format_.width, pix_format_.height, pix_format_.bytesperline/pix_format_.width);

    return true;
};



int CameraV4l2::read_frame() {
    int size = 0;
    unsigned int i = 0;
    struct v4l2_buffer buf;
    memset (&buf, 0, sizeof(buf));

    switch (io_method_) {
    case IO_Method::IO_METHOD_READ:
        if (-1 == (size=read(camera_fd_, buffers_[0].start, buffers_[0].length))) {
            PUMA_LOG_ERROR("IO_METHOD_READ, errno={},{}", errno, strerror(errno));
            return -1;
        }
        frame_process(buffers_[buf.index].start, size);
        break;

    case IO_Method::IO_METHOD_MMAP:
        buf.type = device_type_;
        buf.memory = V4L2_MEMORY_MMAP;

        if (-1 == xioctl(camera_fd_, VIDIOC_DQBUF, &buf)) {
            PUMA_LOG_ERROR("VIDIOC_DQBUF, errno={},{}", errno, strerror(errno));
            return -1;
        } else if (buf.index < buffers_num_) {
            size = buf.bytesused;
            if(copy_frame_){
                if(!ConvertImageData(buffers_[buf.index].start, size))
                {
                    frame_process(buffers_[buf.index].start, size);
                }
                else{
                    //convert成功
                    frame_process((uint8_t *)converted_image_data_.data(), size);
                }
            }
            else{
                frame_process(buffers_[buf.index].start, size);
            }
            if (-1 == xioctl(camera_fd_, VIDIOC_QBUF, &buf)) {
                PUMA_LOG_ERROR("VIDIOC_QBUF, errno={},{}", errno, strerror(errno));
                return -1;
            }
        }
        break;

    case IO_Method::IO_METHOD_USERPTR:
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_USERPTR;
        if (-1 == xioctl(camera_fd_, VIDIOC_DQBUF, &buf)) {
            PUMA_LOG_ERROR("VIDIOC_DQBUF, errno={},{}", errno, strerror(errno));
            return -1;
        }

        for (i = 0; i < buffers_num_; ++i) {
            if (buf.m.userptr == (unsigned long)buffers_[i].start
                && buf.length == buffers_[i].length) {
                break;
            }
        }

        size = i < buffers_num_ ? buf.bytesused : 0;
        if (size > 0) { frame_process((void*)buf.m.userptr, size, i); }

        if (nullptr == outside_buff_ptr) {
            if (-1 == xioctl(camera_fd_, VIDIOC_QBUF, &buf)) {
                PUMA_LOG_ERROR("VIDIOC_QBUF, errno={},{}", errno, strerror(errno));
                return -1;
            }
        } else {
            PUMA_LOG_TRACE("push new frame index = {} into queue", buf.index);
            memory_user_queue_.push(buf);
            if (memory_user_queue_.size() >= buffers_num_/2) {
                struct v4l2_buffer tmp = memory_user_queue_.front();
                PUMA_LOG_TRACE("try to free old frame index = {} out of queue", tmp.index);
                if (-1 == xioctl(camera_fd_, VIDIOC_QBUF, &tmp)) {
                    PUMA_LOG_ERROR("VIDIOC_QBUF, errno={},{}", errno, strerror(errno));
                    return -1;
                } else {
                    PUMA_LOG_TRACE("free old frame success");
                }
                memory_user_queue_.pop();
            }
        }
        break;
    }
    return size;
}

bool CameraV4l2::open_device() {
    struct stat sb;

    if (-1 == stat(device_node_name_.c_str(), &sb)) {
        PUMA_LOG_ERROR("Cannot identify {}, errno={}, {}", device_node_name_, errno, strerror(errno));
        return false;
    }

    if (S_IFCHR != (sb.st_mode & S_IFMT)) {
        PUMA_LOG_ERROR("{} is no device", device_node_name_);
        return false;
    }

    do {
        camera_fd_ = open(device_node_name_.c_str(), O_RDWR | O_NONBLOCK);
    }while(EALREADY == errno);

    if (camera_fd_ < 0) {
        PUMA_LOG_ERROR("Can not open device : {}, errno={}, {}", device_node_name_, errno, strerror(errno));
        close_device();
        return false;
    }

    return true;
}

void CameraV4l2::close_device() {
    if (-1 != camera_fd_) {
        close(camera_fd_);
    }
    camera_fd_ = -1;
}

bool CameraV4l2::init_device() {
    bool success = false;
    do {
        if (!check_capabilities())
            break;
        if (!check_format())
            break;
        if (!config_format())
            break;
        if (!config_fps())
            break;
        // if (!init_gain_range()) {
        //     PUMA_LOG_WARN("Init Gain Range failed");
        // }

        switch (io_method_) {
        case IO_Method::IO_METHOD_READ:
            PUMA_LOG_TRACE("IO_METHOD_READ");
            success = init_read();
            break;

        case IO_Method::IO_METHOD_MMAP:
            PUMA_LOG_TRACE("IO_METHOD_MMAP");
            success = init_mmap();
            break;

        case IO_Method::IO_METHOD_USERPTR:
            PUMA_LOG_TRACE("IO_METHOD_USERPTR");
            success = init_userptr();
            break;
        }

        if (!success) {
            uninit_device();
        } else {
            if(copy_frame_  && (converted_image_data_.size()==0) ) {
                converted_image_data_.resize(image_bytes_);
            }
            return true;
        }
    }while(0);

    close_device();
    return success;
}

bool CameraV4l2::start_device() {
    bool success = false;

    if (start_capturing()) {
        isStreamOn_ = true;
        success = true;
        setStartTime();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        setAutoExposure(static_cast<int>(AE_mode_));
        setExposureTime(exposure_time_);
        setGain(gain_);
    }

    return success;
}

bool CameraV4l2::start_capturing() {
    bool success = true;
    unsigned int i = 0;
    v4l2_buf_type type;
    switch (io_method_) {
    case IO_Method::IO_METHOD_READ:
        PUMA_LOG_TRACE("Nothing to do");
        break;

    case IO_Method::IO_METHOD_MMAP:
        for (i = 0; i < buffers_num_; ++i) {
            struct v4l2_buffer buf;
            memset (&buf, 0, sizeof(buf));
            buf.type        = device_type_;
            buf.memory      = V4L2_MEMORY_MMAP;
            buf.index       = i;

            if (-1 == xioctl(camera_fd_, VIDIOC_QBUF, &buf)) {
                PUMA_LOG_ERROR("VIDIOC_QBUF, errno={},{}", errno, strerror(errno));
                success = false;
            }
        }
        type = device_type_;
        if (-1 == xioctl(camera_fd_, VIDIOC_STREAMON, &type)) {
            PUMA_LOG_ERROR("VIDIOC_STREAMON, errno={},{}", errno, strerror(errno));
            success = false;
        }
        break;

    case IO_Method::IO_METHOD_USERPTR:
        for (i = 0; i < buffers_num_; ++i) {
            struct v4l2_buffer buf;
            memset (&buf, 0, sizeof(buf));
            buf.type        = device_type_;
            buf.memory      = V4L2_MEMORY_USERPTR;
            buf.index       = i;
            buf.m.userptr = (unsigned long)buffers_[i].start;
            buf.length = buffers_[i].length;

            if (-1 == xioctl(camera_fd_, VIDIOC_QBUF, &buf)) {
                PUMA_LOG_ERROR("VIDIOC_QBUF, errno={},{}", errno, strerror(errno));
                success = false;
            }
        }
        type = device_type_;
        if (-1 == xioctl(camera_fd_, VIDIOC_STREAMON, &type)) {
            PUMA_LOG_ERROR("VIDIOC_STREAMON, errno={},{}", errno, strerror(errno));
            success = false;
        }
        break;

    }
    return success;
}

bool CameraV4l2::stop_capturing() {
    bool success = true;
    v4l2_buf_type type;
    switch (io_method_) {
    case IO_Method::IO_METHOD_READ:
        PUMA_LOG_TRACE("stop_capturing : IO_METHOD_READ, Nothing to do");
        break;

    case IO_Method::IO_METHOD_MMAP:
    case IO_Method::IO_METHOD_USERPTR:
        type  = device_type_;
        if (-1 == xioctl(camera_fd_, VIDIOC_STREAMOFF, &type)) {
            PUMA_LOG_ERROR("VIDIOC_STREAMOFF, errno={},{}", errno, strerror(errno));
            success = false;
        }
        break;
    }
    return success;
}

bool CameraV4l2::uninit_device() {
    bool success = true;
    unsigned int i = 0;
    struct v4l2_requestbuffers req;

    switch (io_method_) {
    case IO_Method::IO_METHOD_READ:
        delete [] (uint8_t *)buffers_[0].start;
        break;

    case IO_Method::IO_METHOD_MMAP:
        PUMA_LOG_TRACE("mmap buffer size = {}", buffers_num_);
        for (i = 0; i < buffers_num_; ++i) {
            if (-1 == munmap (buffers_[i].start, buffers_[i].length)) {
                PUMA_LOG_ERROR("munmap, errno={},{}", errno, strerror(errno));
                success = false;
            }
        }

        // free buffers
        memset (&req, 0, sizeof(req));
        req.count               = 0;
        req.type                = device_type_;
        req.memory              = V4L2_MEMORY_MMAP;
        if (-1 == xioctl(camera_fd_, VIDIOC_REQBUFS, &req)) {
            PUMA_LOG_ERROR("VIDIOC_REQBUFS, errno={},{}", errno, strerror(errno));
            success = false;
        }

        break;

    case IO_Method::IO_METHOD_USERPTR:
        for (i = 0; i < buffers_num_; ++i) {
            if (nullptr == outside_buff_ptr) {
                delete [] ((uint8_t *)buffers_[i].start);
            }
            buffers_[i].start = nullptr;
        }
        break;
    }

    delete [] buffers_;

    if(converted_image_data_.size() > 0 ) {
        std::vector<uint8_t> temp_vec;
        temp_vec.swap(converted_image_data_);
    }
    return success;
}

bool CameraV4l2::init_read() {
    buffers_ = new buffer_t[1];

    buffers_[0].length = image_bytes_;
    buffers_[0].start = new uint8_t[image_bytes_];

    if (!buffers_[0].start) {
        PUMA_LOG_ERROR("Out of memory");
        return false;
    }
    return true;
}

bool CameraV4l2::init_mmap() {
    bool success = true;
    struct v4l2_requestbuffers req;
    memset (&req, 0, sizeof(req));
    req.count               = V4L2_BUF_NUM_;
    req.type                = device_type_;
    req.memory              = V4L2_MEMORY_MMAP;

    if (-1 == xioctl(camera_fd_, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            PUMA_LOG_ERROR("Device {} does not support memory mapping", device_node_name_);
            success = false;
        } else {
            PUMA_LOG_ERROR("VIDIOC_REQBUFS, errno={},{}", errno, strerror(errno));
            success = false;
        }
    } else {
        PUMA_LOG_TRACE("Device {}, Request {} buffer", device_node_name_, req.count);



        buffers_ = new buffer_t[req.count];
        if (nullptr == buffers_) {
            PUMA_LOG_ERROR("Out of memory");
            return false;
        }

        memset(buffers_,0, req.count*sizeof(buffer_t));

        // allocate buffers
        for (buffers_num_ = 0; buffers_num_ < req.count; ++buffers_num_) {
            struct v4l2_buffer buf;
            memset (&buf, 0, sizeof(buf));
            buf.type        = device_type_;
            buf.memory      = V4L2_MEMORY_MMAP;
            buf.index       = buffers_num_;

            if (-1 == xioctl(camera_fd_, VIDIOC_QUERYBUF, &buf)) {
                PUMA_LOG_ERROR("VIDIOC_QUERYBUF, errno={},{}", errno, strerror(errno));
                success = false;
            } else {
                PUMA_LOG_TRACE("Device {}, buffer idx:{}, size:{}", device_node_name_, buffers_num_, buf.length);
                buffers_[buffers_num_].length = buf.length;
                buffers_[buffers_num_].start = mmap (NULL,
                                                buf.length,
                                                PROT_READ | PROT_WRITE /* required */,
                                                MAP_SHARED /* recommended */,
                                                camera_fd_,
                                                buf.m.offset);

                if (MAP_FAILED == buffers_[buffers_num_].start) {
                    PUMA_LOG_ERROR("Mmap, errno={},{}", errno, strerror(errno));
                    success = false;
                }
            }
        }
    }
    return success;
}

bool CameraV4l2::init_userptr() {
    struct v4l2_requestbuffers req;
    memset(&req, 0, sizeof(req));

    req.count  = buffers_num_;
    req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_USERPTR;

    PUMA_LOG_TRACE("request {} buffers", buffers_num_);
    if (-1 == xioctl(camera_fd_, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            PUMA_LOG_ERROR("{} does not support user pointer I/O", device_node_name_);
            return false;
        } else {
            PUMA_LOG_ERROR("VIDIOC_REQBUFS , errno={},{}", errno, strerror(errno));
            return false;
        }
    }

    buffers_ = new buffer_t[req.count];
    if (nullptr == buffers_) {
        PUMA_LOG_ERROR("Out of memory");
        return false;
    }

    buffers_num_ = req.count;
    memset(buffers_,0, buffers_num_*sizeof(buffer_t));
    PUMA_LOG_TRACE("Device offered {} buffers", buffers_num_);

    for (size_t i = 0; i < buffers_num_; ++i) {
        buffers_[i].length = image_bytes_;
        if (nullptr == outside_buff_ptr) {
            buffers_[i].start = new uint8_t[image_bytes_];
        } else {
            buffers_[i].start = outside_buff_ptr + i*image_bytes_;
        }

        if (!buffers_[i].start) {
            PUMA_LOG_ERROR("Out of memory");
            return false;
        }
    }
    return true;
}

bool CameraV4l2::stop_device() {
    bool success = true;
    if (-1 == camera_fd_) {
        PUMA_LOG_TRACE("camera_fd_ = -1");
        return success;
    }

    isStreamOn_ = false;
    success = stop_capturing();
    uninit_device();
    return success;
}

int CameraV4l2::restart_device() {
    stop_device();
    close_device();

    if (false == find_device()) {
        return restart_flag_;
    }

    if (false == open_device()) {
        close_device();
        return restart_flag_;
    }

    if (false == init_device()) {
        close_device();
        return restart_flag_;
    }

    if (false == start_device()) {
        stop_device();
        close_device();
        return restart_flag_;
    }

    restart_flag_ = 0;
    PUMA_LOG_TRACE("Restart successful");
    return restart_flag_;
}

bool CameraV4l2::find_device(int16_t _vid, int16_t _pid) {
    device_node_name_.clear();
    const char* dir = "/sys/class/video4linux";

    struct stat dir_state;
    if (lstat(dir, &dir_state) == -1 || !S_ISDIR(dir_state.st_mode)) {
        PUMA_LOG_ERROR("{} does not exist or is not a directory", dir);
        return false;
    }

    DIR* pDir = opendir(dir);
    if (!pDir) {
        PUMA_LOG_ERROR("Failed to open directory: {}", dir);
        return false;
    }

    char path[256];
    char modalias[128];

    for (int trynum = 0; trynum < 20 && device_node_name_.empty(); trynum++) {
        rewinddir(pDir);

        struct dirent* entry;
        while ((entry = readdir(pDir)) && device_node_name_.empty()) {
            std::string file(entry->d_name);
            if (file.rfind("video", 0) != 0) continue;

            snprintf(path, sizeof(path), "%s/%s/device/modalias", dir, file.c_str());

            std::ifstream cam_desc(path);
            if (!cam_desc.good()) continue;

            cam_desc.getline(modalias, sizeof(modalias));
            cam_desc.close();

            uint32_t vid, pid;
            if (sscanf(modalias, "usb:v%4xp%4x", &vid, &pid) != 2)
                continue;

            PUMA_LOG_TRACE("USB device {}, VID:0x{:x}, PID:0x{:x}", file, vid, pid);

            if (vid == (uint32_t)_vid && pid == (uint32_t)_pid) {
                device_node_name_ = "/dev/" + file;
                PUMA_LOG_TRACE("Find device: {}", device_node_name_);
                break;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    closedir(pDir);

    if (device_node_name_.empty()) {
        PUMA_LOG_FATAL("Cannot find camera devices");
        return false;
    }

    return true;
}


bool CameraV4l2::find_device(const std::string& _device_name) {
    device_node_name_.clear();
    char tmp_buffer[128] = {0};
    const char* dir = "/dev";

    struct stat dir_state;
    int ret = lstat(dir, &dir_state);

    /// check if dir is exist
    if (-1 == ret) {
        PUMA_LOG_ERROR("{} {}, errno = {}",dir, strerror(errno), errno);
        return false;
    }

    if (!S_ISDIR(dir_state.st_mode)) {
        PUMA_LOG_ERROR("{} is not a directory",dir);
        return false;
    }

    DIR* pDir = opendir(dir);
    if (NULL == pDir) {
        PUMA_LOG_ERROR("{} is not exist directory", dir);
    }

    int trynum = 0;
    while (++trynum < 20 && device_node_name_.empty()) {
        /// find files in directory 'dir'
        struct dirent * dirent = NULL;
        while(NULL != (dirent = readdir(pDir))) {
            std::string file(dirent->d_name);
            if (file.length() < 6 || file.substr(0,5) != std::string("video")) {
                continue;
            }
            sprintf(tmp_buffer, "%s/%s", dir, file.c_str());
            PUMA_LOG_TRACE("{}, len = {}", tmp_buffer, strlen(tmp_buffer));
            device_node_name_ = std::string(tmp_buffer);
            PUMA_LOG_TRACE("CHECK {}", device_node_name_);
            if (open_device()) {
                struct v4l2_capability cap;
                memset(&(cap), 0, sizeof(cap));
                if (-1 == xioctl(camera_fd_, VIDIOC_QUERYCAP, &cap))
                {
                    PUMA_LOG_ERROR("Cannot get capabilities for device: {}, errno={}, {}", device_node_name_, errno, strerror(errno));
                    close_device();
                    continue;
                }
                std::string device_card((char *)cap.card);
                close_device();

                if (device_card.find(_device_name) != std::string::npos) {
                    //注意：device_caps 只在内核 >= 2.6.26 有效
                    //如果驱动支持 V4L2_CAP_DEVICE_CAPS 位（0x80000000），则 应使用 device_caps 判断 node 能力，而不是 capabilities。
                    PUMA_LOG_TRACE("capabilities: 0x{:x}", cap.capabilities);
                    if (!(cap.device_caps & V4L2_CAP_VIDEO_CAPTURE)) {
                        PUMA_LOG_TRACE("This is NOT a VIDEO_CAPTURE device!");
                        continue;
                    }

                    PUMA_LOG_TRACE("Found device {}, Card : {}", device_node_name_, (char *)cap.card);
                    break;
                } else {
                    PUMA_LOG_TRACE("Wrong device {}, Card : {}", device_node_name_, (char *)cap.card);
                    device_node_name_.clear();
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    closedir(pDir);

    if (device_node_name_.empty()) {
        PUMA_LOG_FATAL("Cannot find camera devices");
        return false;
    } else {
        return true;
    }
}

bool CameraV4l2::find_device() {
    if (!device_node_name_.empty()) {
        PUMA_LOG_TRACE("NODE : {}", device_node_name_);
        return true;
    }

    bool found = false;
    for (auto it : product_id_) {
        if (find_device(std::get<0>(it), std::get<1>(it))) {
            PUMA_LOG_TRACE("VID / PID : 0x{:x}/0x{:x}", std::get<0>(it), std::get<1>(it));
            found = true;
            break;
        }
    }

    if (found) { return found; }

    found = false;
    for (auto it : product_name_) {
        if (find_device(it)) {
            PUMA_LOG_TRACE("NAME : {}", it);
            found = true;
            break;
        }
    }
    return found;
}

bool CameraV4l2::check_capabilities() {
    struct v4l2_capability cap;
    memset(&(cap), 0, sizeof(cap));
    if (-1 == xioctl(camera_fd_, VIDIOC_QUERYCAP, &cap))
    {
        PUMA_LOG_ERROR("Cannot get capabilities for device: {}, errno={}, {}", device_node_name_, errno, strerror(errno));
        return false;
    }

    // PUMA_LOG_TRACE("Driver: {}, capabilities: 0x{:x}, Card: {}, Bus info: {}, Version: 0x{:x}, Device_caps: 0x{:x}",
    //             cap.driver, cap.capabilities, cap.card, cap.bus_info, cap.version, cap.device_caps);

    if ( (cap.capabilities & device_type_) != device_type_ ) {
        PUMA_LOG_ERROR("Cannot get capabilities for device: {}, errno={}, {}", device_node_name_, errno, strerror(errno));
        return false;
    }

    switch (io_method_) {
    case IO_Method::IO_METHOD_READ:
        if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
            PUMA_LOG_ERROR("{} does not support read IO", device_node_name_);
            return false;
        }
        break;

    case IO_Method::IO_METHOD_MMAP:
    case IO_Method::IO_METHOD_USERPTR:
        if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
            PUMA_LOG_ERROR("{} does not support streaming IO", device_node_name_);
            return false;
        }
        break;
    }

    return true;
}

bool CameraV4l2::check_format() {
    struct v4l2_fmtdesc fmt;
    memset(&fmt, 0, sizeof(fmt));
    fmt.index = 0;
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret = 0;
    while ( (ret=xioctl(camera_fd_, VIDIOC_ENUM_FMT, &fmt)) == 0 ) {
        PUMA_LOG_TRACE("check_format : The camera support {}", fourcc(fmt.pixelformat));
        fmt.index++;
        if (pix_format_.pixelformat != fmt.pixelformat) {
            PUMA_LOG_TRACE("check_format : The camera support {} instead of {}",
                fourcc(fmt.pixelformat), fourcc(pix_format_.pixelformat));
            continue;
        }
        PUMA_LOG_TRACE("Found pixel format {}", fourcc(pix_format_.pixelformat));

        struct v4l2_frmsizeenum frmsize;
        frmsize.pixel_format = fmt.pixelformat;
        frmsize.index = 0;

        uint32_t width = pix_format_.width;
        uint32_t height = pix_format_.height;
        bool isFound = false;
        while (0 == xioctl(camera_fd_, VIDIOC_ENUM_FRAMESIZES, &frmsize)){

            if(frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE){
                if (width == frmsize.discrete.width && height == frmsize.discrete.height) {
                    isFound = true;
                    PUMA_LOG_TRACE("check_format : The camera supports {}x{}", pix_format_.width, pix_format_.height);
                    break;
                }
            }
            frmsize.index++;
        }

        if (isFound) {
            PUMA_LOG_TRACE("check_format : width={}, height={}, pixelformat={}", pix_format_.width, pix_format_.height, fourcc(pix_format_.pixelformat));
            return true;
        } else {
            PUMA_LOG_ERROR("{} does not support {}x{}", camera_fd_, width, height);
        }
    }
    if(ret != 0) {
        PUMA_LOG_ERROR("check format faile, camera_fd_={}, ret={},errno={},", camera_fd_, ret, errno);
    }

    return false;
}

bool CameraV4l2::config_format() {
    struct v4l2_format   fmt;
    memset(&(fmt), 0, sizeof(fmt));
    fmt.type                = device_type_;
    fmt.fmt.pix.width       = pix_format_.width;
    fmt.fmt.pix.height      = pix_format_.height;
    fmt.fmt.pix.pixelformat = pix_format_.pixelformat;
    fmt.fmt.pix.field       = V4L2_FIELD_ANY;

    if (xioctl(camera_fd_, VIDIOC_S_FMT, &fmt) == -1)
    {
        PUMA_LOG_ERROR("Cannot set format for device:{}, {},width={}, height={}, pixelformat={}", device_node_name_, strerror(errno), pix_format_.width, pix_format_.height, fourcc(pix_format_.pixelformat));
        return false;
    }
    if (fmt.fmt.pix.pixelformat != pix_format_.pixelformat)
    {
        PUMA_LOG_ERROR("Cannot set pixelformat to: {}, format is: {}", fourcc(pix_format_.pixelformat), fourcc(fmt.fmt.pix.pixelformat));
        return false;
    }
    if ((fmt.fmt.pix.width != pix_format_.width) || (fmt.fmt.pix.height != pix_format_.height))
    {
        PUMA_LOG_TRACE("Cannot set size to: {}x{}, size is: {}x{}", pix_format_.width, pix_format_.height, fmt.fmt.pix.width, fmt.fmt.pix.height);
    }

    PUMA_LOG_TRACE("Set pixelformat to : {}, width={}, height={} successful", fourcc(fmt.fmt.pix.pixelformat), fmt.fmt.pix.width, fmt.fmt.pix.height);
    pix_format_ = fmt.fmt.pix;
    return true;
}

bool CameraV4l2::config_fps() {
    struct v4l2_streamparm   param;
    memset(&(param), 0, sizeof(param));
    param.type = device_type_;
    param.parm.capture.timeperframe.numerator = 1;
    param.parm.capture.timeperframe.denominator = image_fps_;

    if (xioctl(camera_fd_, VIDIOC_S_PARM, &param) == -1)
    {
        PUMA_LOG_ERROR("Cannot set FPS for device:{}, errno={}, {}", device_node_name_, errno, strerror(errno));
        return false;
    }

    if (image_fps_ != param.parm.capture.timeperframe.denominator) {
        PUMA_LOG_WARN("Fps is {}, which should be {}", param.parm.capture.timeperframe.denominator, image_fps_);
    }

    PUMA_LOG_TRACE("fps: {} / {}", param.parm.capture.timeperframe.numerator, param.parm.capture.timeperframe.denominator);
    return true;
}

bool CameraV4l2::init_gain_range() {
    struct v4l2_queryctrl  setting;
    setting.id = V4L2_CID_GAIN;
    if (xioctl(camera_fd_, VIDIOC_QUERYCTRL, &setting) == -1) {
        PUMA_LOG_ERROR("Cannot get range of gain for device:{}, error={}, {}",device_node_name_, errno, strerror(errno));
        return false;
    }

    min_gain_ = setting.minimum;
    max_gain_ = setting.maximum;
    max_valid_gain_ = ceil(max_gain_*3.0/8.0);
    min_valid_gain_ = min_gain_;
    PUMA_LOG_TRACE("min_gain = {}, max_gain = {}, step = {}", min_gain_, max_gain_, setting.step);
    PUMA_LOG_TRACE("min_valid_gain = {}, max_valid_gain = {}", min_valid_gain_, max_valid_gain_);
    return true;
}

#endif