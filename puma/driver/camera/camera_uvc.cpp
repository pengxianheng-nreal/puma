#include "camera_uvc.h"
#if defined(PUMA_SYSTEM_LINUX) || defined(PUMA_SYSTEM_ANDROID)
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <libuvc/libuvc_internal.h>
#include <puma/util/log.h>
#include <puma/common/common.h>



CameraUvc::CameraUvc(std::function<void (const GenericShmWrapper&)> _callback) :
    CameraDriver(_callback) {
}

void CameraUvc::uvc_callback(uvc_frame_t *frame, void *ptr) {
    CameraUvc *ctx = (CameraUvc *) ptr;
    if (frame->data_bytes != frame->width * frame->height * ctx->info_.channels) {
        PUMA_LOG_WARN("CameraUvc data_bytes may invalid, should be = {}, cur = {}",
            frame->width * frame->height * ctx->info_.channels, frame->data_bytes);
        return;
    }

    frame_t * pFrame = (frame_t *)ctx->curr_frame_.reserved;
    pFrame->device_start_time = ctx->startTime();
    ctx->curr_frame_.reserved_size = sizeof(frame_t);
    ctx->curr_frame_.type = 1;

    if (frame->data_bytes > 0) {
        ctx->uvc_is_callback_++;
        if (ctx->running_) {
            pFrame->data = (uint8_t*)frame->data;
            pFrame->width = frame->width;
            pFrame->height = frame->height;
            pFrame->step = frame->step;
            pFrame->sof = frame->cam_sof;
            pFrame->data_bytes = frame->data_bytes;
            pFrame->bytes_per_pixel = frame->step / frame->width; //XXX: xreal use it as camera_num
            if (ctx->frame_callback_)
                ctx->frame_callback_(ctx->curr_frame_);
            //ctx->softAEAG(*pFrame);
        }
    }
}

CameraUvc::~CameraUvc() {
    stop();
}

void CameraUvc::stop() {
    if (cam_proc_status_ == UVC_CAM_PROC_START_STREAM) {
        uvc_stop_streaming(dev_handle_);
        cam_proc_status_ = UVC_CAM_PROC_STOP_STREAM;
        PUMA_LOG_INFO("CameraUvc stopped streaming");
    }
    if (cam_proc_status_ == UVC_CAM_PROC_STOP_STREAM || cam_proc_status_ == UVC_CAM_PROC_OPEN) {
        uvc_close(dev_handle_);
        cam_proc_status_ = UVC_CAM_PROC_CLOSE;
        PUMA_LOG_INFO("CameraUvc closed");
    }
    if (cam_proc_status_ == UVC_CAM_PROC_CLOSE || cam_proc_status_ == UVC_CAM_PROC_FIND_DEV) {
        uvc_unref_device(dev_);
        cam_proc_status_ = UVC_CAM_PROC_UNREF;
        PUMA_LOG_TRACE("CameraUvc unref device");
    }
    if (cam_proc_status_ == UVC_CAM_PROC_UNREF || cam_proc_status_ == UVC_CAM_PROC_INIT) {
        uvc_exit(contex_);
        cam_proc_status_ = UVC_CAM_PROC_EXIT;
        PUMA_LOG_INFO("CameraUvc exit");
    }
}

bool CameraUvc::init() {
    if (UVC_SUCCESS == uvcOpen()) {
        initGainRange();
        return true;
    } else {
        PUMA_LOG_ERROR("CameraUvc init failed");
        return false;
    }
}

bool CameraUvc::start() {
    uvcStartStream();
    if (cam_proc_status_ != UVC_CAM_PROC_START_STREAM) {
        PUMA_LOG_ERROR("CameraUvc start failed");
        return false;
    } else {
        return true;
    }
}



void CameraUvc::GetSupportFrameInfo(const uvc_format_desc_t *format_descs) {
    int resolution_index=0;
    while (format_descs != NULL) {
        uvc_frame_desc_t *frame_desc = format_descs->frame_descs;
        while (frame_desc != NULL) {
            uint16_t fps = 10000000 / frame_desc->dwDefaultFrameInterval;
            support_frame_infos_.emplace_back(frame_desc->wWidth,frame_desc->wHeight,fps);
            PUMA_LOG_INFO("frame info: index:{} w:{}, h:{}, fps:{},dwDefaultFrameInterval:{}",
            resolution_index, frame_desc->wWidth, frame_desc->wHeight,fps,frame_desc->dwDefaultFrameInterval);
            resolution_index++;
            frame_desc = frame_desc->next;
        }
        format_descs = format_descs->next;
    }
}

int CameraUvc::uvcStartStream() {

    auto camera_autoexposure = AE_mode_;
    if (cam_proc_status_ != UVC_CAM_PROC_OPEN) {
        return UVC_ERROR_OTHER;
    }

    uint32_t height = 0;
    enum uvc_frame_format uvc_fmt = UVC_FRAME_FORMAT_COUNT;
    uvc_error_t res = UVC_ERROR_OTHER;
    if (info_.uvc_fmt == 0) {
        info_.uvc_fmt = UVC_FRAME_FORMAT_GRAY8;
    }

    do {

        const uvc_format_desc_t *format_desc = uvc_get_format_descs(dev_handle_);
        if(format_desc) {
            GetSupportFrameInfo(format_desc);
        }
        else {
            PUMA_LOG_TRACE("uvcStartStream format_desc is nullptr!");
        }
        PUMA_LOG_INFO("resolution size:{}",support_frame_infos_.size());
        if(!info_.height_from_config && support_frame_infos_.size() > 0) {
            uint16_t min_height = support_frame_infos_[0].height;
            uint16_t fps =  support_frame_infos_[0].fps;
            for(const auto& image_info : support_frame_infos_) {
                if(image_info.height < min_height) {
                    min_height = image_info.height;
                    fps = image_info.fps;
                }
            }
            info_.height = min_height;
            info_.frame_rate = fps;
        }
        

        uvc_fmt = static_cast<uvc_frame_format>(info_.uvc_fmt);
        height = info_.height;
        res = uvc_get_stream_ctrl_format_size(dev_handle_, &ctrl_,
                                                      uvc_fmt,
                                                      info_.width,
                                                      height, info_.frame_rate);

        if (UVC_SUCCESS == res) { break; }

        PUMA_LOG_TRACE("#1 uvc_get_stream_ctrl_format_size fail:{}, uvc_fmt:{},width:{},height:{},frame_rate:{}",
        res,uvc_fmt,info_.width,height,info_.frame_rate);
        
        height = info_.height - 1;
        res = uvc_get_stream_ctrl_format_size(dev_handle_, &ctrl_,
                                                        uvc_fmt,
                                                        info_.width,
                                                        height, info_.frame_rate);
        if (UVC_SUCCESS == res) { break; }

        PUMA_LOG_TRACE("#2 uvc_get_stream_ctrl_format_size fail:{}, uvc_fmt:{},width:{},height:{},frame_rate:{}",
        res,uvc_fmt,info_.width,height,info_.frame_rate);

        uvc_fmt = UVC_FRAME_FORMAT_GRAY8;
        res = uvc_get_stream_ctrl_format_size(dev_handle_, &ctrl_,
                                                      uvc_fmt,
                                                      info_.width,
                                                      height, info_.frame_rate);
        if (UVC_SUCCESS == res) { break; }

        PUMA_LOG_TRACE("#3 uvc_get_stream_ctrl_format_size fail:{}, uvc_fmt:{},width:{},height:{},frame_rate:{}",
        res,uvc_fmt,info_.width,height,info_.frame_rate);

        height = info_.height;
        res = uvc_get_stream_ctrl_format_size(dev_handle_, &ctrl_,
                                                        uvc_fmt,
                                                        info_.width,
                                                        height, info_.frame_rate);
        if (UVC_SUCCESS == res) { break; }

        PUMA_LOG_TRACE("#4 uvc_get_stream_ctrl_format_size fail:{}, uvc_fmt:{},width:{},height:{},frame_rate:{}",
        res,uvc_fmt,info_.width,height,info_.frame_rate);
    }while(0);
    PUMA_LOG_TRACE("uvcStartStream, width = {}, height = {}, fmt = {}, fps = {}",
            info_.width, height, info_.uvc_fmt, info_.frame_rate);

    if (UVC_FRAME_FORMAT_YUYV == uvc_fmt) {
        PUMA_LOG_TRACE("uvc use format : UVC_FRAME_FORMAT_YUYV");
    } else {
        PUMA_LOG_TRACE("uvc use format : UVC_FRAME_FORMAT_GRAY8");
    }

    if (res != UVC_SUCCESS) {
        PUMA_LOG_FATAL("CameraUvc get_mode : {} ({})", uvc_strerror(res), res);
    } else {
        /** when call uvc_start_streaming() , uvc_callback will got data,
         * but sometimes there is no callback, so we use 'uvc_is_callback_' to count callbacks
         * when start we wait(50*20ms) the uvc_is_callback_ counter to make sure we got images,
         * then return uvc_sucess, otherwise stop streaming wait 300ms and redo start_streaming.
         * this process we do at most 5 times, if still no callbacks, do exit process.
         */

        for (int i = 0; i < 5; i++) {
            res = uvc_start_streaming(dev_handle_, &ctrl_, uvc_callback, (uvc_frame_copyframe_t *)copy_frame_, this, 0);
            if (res < 0) {
                PUMA_LOG_ERROR("CameraUvc start streaming : {} ({})", uvc_strerror(res), res);
            } else {
                cam_proc_status_ = UVC_CAM_PROC_START_STREAM;
                uvc_is_callback_ = -1;
                /// set auto gain control, auto exposure
                setAutoExposure(static_cast<int>(AEAGMode::AE_MODE_SOFT));
                setExposureTime(exposure_time_);
                setGain(gain_);

                for (int j = 0; j < 50; j++) {
                    /// set auto gain control, auto exposure
                    setAutoExposure(static_cast<int>(AEAGMode::AE_MODE_SOFT));
                    setExposureTime(exposure_time_);
                    setGain(gain_);

                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    if (uvc_is_callback_ > 0) {
                        setAutoExposure(static_cast<int>(camera_autoexposure));
                        setExposureTime(exposure_time_);
                        setGain(gain_);
                        PUMA_LOG_INFO("CameraUvc streaming started");
                        running_ = true;
                        setStartTime();
                        
                        {
                            std::unique_lock <std::mutex> lck(thread_id_mutex_);
                            thread_id_ = uvc_stream_thread_id(dev_handle_);
                            thread_id_cond_.notify_all();
                            PUMA_LOG_TRACE("CameraUvc thread id = {}", thread_id_);
                        }
                        
                        return UVC_SUCCESS;
                    }
                }

                PUMA_LOG_INFO("CameraUvc has no streaming, restart streaming");
                uvc_stop_streaming(dev_handle_);
                PUMA_LOG_TRACE("CameraUvc streaming stopped");
                cam_proc_status_ = UVC_CAM_PROC_STOP_STREAM;
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
            }
        }
    }

    uvc_close(dev_handle_);
    cam_proc_status_ = UVC_CAM_PROC_CLOSE;
    uvc_unref_device(dev_);
    cam_proc_status_ = UVC_CAM_PROC_UNREF;
    uvc_exit(contex_);
    cam_proc_status_ = UVC_CAM_PROC_EXIT;
    PUMA_LOG_TRACE("UVC exited");

    return res;
}

int CameraUvc::uvcOpen() {
    uvc_error_t res = UVC_ERROR_OTHER;
    contex_ = NULL;

    if (info_.usb_fd > 0) {
        contex_ = (uvc_context_t *)calloc(1, sizeof(uvc_context_t));
        if(0 == libusb_init(&(contex_->usb_ctx))) {
            contex_->own_usb_ctx = 1;
            res = UVC_SUCCESS;
        } else {
            PUMA_LOG_FATAL("usb_fd: {} CameraUvc init failed", info_.usb_fd);
            return UVC_ERROR_OTHER;
        }
    } else {
        res = uvc_init(&contex_, NULL);
    }

    if (res < 0) {
        PUMA_LOG_FATAL("CameraUvc init : {} ({})", uvc_strerror(res), res);
        // exit(0);
        return res;
    }
    cam_proc_status_ = UVC_CAM_PROC_INIT;

    PUMA_LOG_DEBUG("product_id_ size:{}", product_id_.size());
    if (info_.usb_fd <= 0) {
        for (auto it : product_id_) {
            PUMA_LOG_TRACE("CameraUvc filtering device of vid: {} pid: {}",
                                std::get<0>(it), std::get<1>(it));
            res = uvc_find_device(contex_, &dev_, std::get<0>(it), std::get<1>(it), NULL);
            if (res == UVC_SUCCESS) {
                PUMA_LOG_TRACE("CameraUvc Found target device");
                break;
            }
        }
    }

    if (res < 0) {
        PUMA_LOG_FATAL("CameraUvc find device : {} ({})", uvc_strerror(res), res);
        return res;
    } else {
        cam_proc_status_ = UVC_CAM_PROC_FIND_DEV;
        PUMA_LOG_TRACE("CameraUvc found valid device");
        if (info_.usb_fd <= 0) {
            PUMA_LOG_INFO("CameraUvc using device found by libuvc");
            res = uvc_open(dev_, &dev_handle_);
        } else {
            PUMA_LOG_INFO("CameraUvc open with handle (usb_fd passed in)");
            res = uvc_wrap_sys_device(contex_, info_.usb_fd, &dev_, &dev_handle_);
        }
        if (res < 0) {
            PUMA_LOG_FATAL("CameraUvc open : {} ({})", uvc_strerror(res), res);
        } else {
            uvc_ref_device(dev_);
            cam_proc_status_ = UVC_CAM_PROC_OPEN;
            PUMA_LOG_INFO("CameraUvc opened");
            return UVC_SUCCESS;
        }
        cam_proc_status_ = UVC_CAM_PROC_UNREF;
    }
    uvc_exit(contex_);
    cam_proc_status_ = UVC_CAM_PROC_EXIT;
    PUMA_LOG_FATAL("CameraUVC open failed");

    return res;
}

void CameraUvc::setAutoExposure(int _auto_exposure) {
    CameraDriver::setAutoExposure(_auto_exposure);
    if (cam_proc_status_ == UVC_CAM_PROC_START_STREAM) {
        uvc_error_t res = UVC_SUCCESS;
        res = uvc_set_ae_mode(dev_handle_, AE_mode_ == AEAGMode::AE_MODE_ISP ? 2 : 4);
        if (res != UVC_SUCCESS) {
            uvc_set_ae_mode(dev_handle_, AE_mode_ == AEAGMode::AE_MODE_ISP ? 8 : 1);
            uvc_set_ae_priority(dev_handle_, 0); /// set frame rate remain constant
        }
    }
}

void CameraUvc::setExposureTime(double _exposure_time) {
    CameraDriver::setExposureTime(_exposure_time);
    if (AE_mode_ == AEAGMode::AE_MODE_ISP) {
        return;
    }
    if (cam_proc_status_ == UVC_CAM_PROC_START_STREAM) {
        uvc_set_exposure_abs(dev_handle_, (uint32_t)(exposure_time_ * 10));
    }
}

void CameraUvc::setGain(int _gain) {
    CameraDriver::setGain(_gain);
    if (AE_mode_ == AEAGMode::AE_MODE_ISP) {
        return;
    }
    if (cam_proc_status_ == UVC_CAM_PROC_START_STREAM) {
        uvc_set_gain(dev_handle_, gain_);
    }
}

bool CameraUvc::usbInit() {
    return false;
}

bool CameraUvc::setVideoProperties(uint32_t prop, uint8_t value) {
    if (!usbInit()) {
        return false;
    }
    
    return false;
}

bool CameraUvc::getVideoProperties(uint32_t prop, uint8_t& value) {
    if (!usbInit()) {
        return false;
    }
    return false;
}

bool CameraUvc::setVideoProperties(uint32_t prop, const std::vector<uint8_t>& value) {
    if (!usbInit()) {
        return false;
    }
 
    return false;
}

bool CameraUvc::getVideoProperties(uint32_t prop, std::vector<uint8_t>& value) {
    if (!usbInit()) {
        return false;
    } 
    return false;
}

bool CameraUvc::initGainRange() {
    int ret = uvc_get_gain(dev_handle_, &max_gain_, UVC_GET_MAX);
    if (ret == UVC_SUCCESS)
        ret = uvc_get_gain(dev_handle_, &min_gain_, UVC_GET_MIN);
    if (ret == UVC_SUCCESS) {
        max_valid_gain_ = ceil(max_gain_*3.0/8.0);
        min_valid_gain_ = min_gain_;
    }
    PUMA_LOG_TRACE("min_gain = {}, max_gain = {}", min_gain_, max_gain_);
    PUMA_LOG_TRACE("min_valid_gain = {}, max_valid_gain = {}", min_valid_gain_, max_valid_gain_);
    return ret == UVC_SUCCESS;
}
#endif