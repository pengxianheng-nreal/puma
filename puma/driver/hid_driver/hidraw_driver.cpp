#include "hidraw_driver.h"
#if defined(PUMA_SYSTEM_LINUX) || defined(PUMA_SYSTEM_ANDROID)
#include "hid_factory.h"
#include <puma/util/log.h>
#include <puma/common/common.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include <cstdlib>
#include <linux/hidraw.h>
#include <sys/ioctl.h>
#include <unistd.h> /// use open/close
#include <fcntl.h> /// use O_RDWR
#include <framework/util/os_time.h>

HidrawDriver::HidrawDriver(std::function<void (const hid_data_t&)> _callback) :
    HidDriver(_callback) {
}

HidrawDriver::~HidrawDriver() {
    HidDriver::stop();

    close(dev_fd_);
    dev_fd_ = -1;
}

bool HidrawDriver::init() {
    if (!initDevice()) {
        return false;
    }
    return true;
}

bool HidrawDriver::start() {
    return HidDriver::start();
}

bool HidrawDriver::initDevice()
{
    char tmp_buffer[256];
    int32_t index = 0;
    struct hidraw_devinfo dev_info;
    int32_t ret = 0;
    while (index < 100) {
        sprintf(tmp_buffer, "/dev/hidraw%d", index++);
        if (info_.is_imu) {
            dev_fd_ = open(tmp_buffer, O_RDWR);
        }
        else {
            dev_fd_ = open(tmp_buffer, O_RDWR | O_NONBLOCK);
        }
        if (dev_fd_ < 0) {
            PUMA_LOG_TRACE("open {} errno = {}, {}", tmp_buffer, errno, strerror(errno));
            continue;
        }

        bool found = false;
        ret = ioctl(dev_fd_, HIDIOCGRAWINFO, &dev_info);
        PUMA_LOG_TRACE("bustype = {}, pid = 0x{:x}, vid = 0x{:x}", dev_info.bustype, dev_info.product, dev_info.vendor);
        for (auto it : product_id_) {
            if (ret >= 0 && dev_info.vendor == std::get<0>(it) && dev_info.product == std::get<1>(it)) {
                char buf[256];
                ret = ioctl(dev_fd_, HIDIOCGRAWPHYS(256), buf);
                if (ret < 0) {
                    PUMA_LOG_ERROR("Get raw phys failed, errno = {}, {}", buf, errno, strerror(errno));
                }
                else {
                    std::string str(buf);
                    auto pos = str.find("input");
                    if (pos != std::string::npos) {
                        auto s = str.substr(pos + 5);
                        uint8_t interface = strtol(s.c_str(), nullptr, 10);
                        if (std::get<2>(it) == 0 || std::get<2>(it) == interface) {
                            PUMA_LOG_TRACE("Interface {}", interface);
                            vid_ = dev_info.vendor;
                            pid_ = dev_info.product;
                            found = true;
                            break;
                        }
                        else {
                            PUMA_LOG_TRACE("Cannot find interface {} in 'input' {}", std::get<2>(it), str);
                        }
                    }
                    else {
                        PUMA_LOG_ERROR("Cannot find 'input' in {}", str);
                    }
                }

            }
        }

        if (found) { break; }

        /// Enumerate device via device name
        char rawname[256];
        ret = ioctl(dev_fd_, HIDIOCGRAWNAME(256), rawname);
        if (ret < 0) {
            PUMA_LOG_ERROR("Get device name failed");
        }
        else {
            std::string device_name(rawname);
            for (auto it : product_name_) {
                if (device_name.find(it) != std::string::npos) {
                    vid_ = dev_info.vendor;
                    pid_ = dev_info.product;
                    found = true;
                    break;
                }
            }
        }
        if (found) { break; }

        close(dev_fd_);
        dev_fd_ = -1;
    }

    if (dev_fd_ < 0) {
        device_state_ = DeviceState::DEVICE_STATE_EXITED;
        PUMA_LOG_FATAL("Raw driver open failed, errno = {}, error = {}", errno, strerror(errno));
        return false;
    }
    return true;
}

bool HidrawDriver::setProperties(const uint8_t* cmd, uint32_t cmd_size) {
    assert(cmd && cmd_size > 0);
    int32_t len = 0;
    
    if (device_state_ == DeviceState::DEVICE_STATE_DEACTIVATED) {
        std::cv_status status = std::cv_status::no_timeout;
        std::unique_lock <std::mutex> lck(hid_init_mutex_);
        do {
            if (device_state_ == DeviceState::DEVICE_STATE_EXITED) return false;
            status = hid_init_cond_.wait_for(lck, std::chrono::milliseconds(50));
        } while(status == std::cv_status::timeout);
    }
    
    if (dev_fd_) {
        len = write(dev_fd_, cmd, cmd_size);
        if ((uint32_t)len != cmd_size) {
            PUMA_LOG_ERROR("Generic Control write length({}) != excepted({}), errno = {}, {}", 
                len, cmd_size, errno, strerror(errno));
        }
    } else {
        PUMA_LOG_WARN("No device handle available");
    }
    return ((uint32_t)len == cmd_size);
}

int32_t HidrawDriver::setFeatureReport(const uint8_t* data, uint32_t length) {
    assert(data && length > 0);
    int32_t len = 0;
    
    if (device_state_ == DeviceState::DEVICE_STATE_DEACTIVATED) {
        std::cv_status status = std::cv_status::no_timeout;
        std::unique_lock <std::mutex> lck(hid_init_mutex_);
        do {
            if (device_state_ == DeviceState::DEVICE_STATE_EXITED) return false;
            status = hid_init_cond_.wait_for(lck, std::chrono::milliseconds(50));
        } while(status == std::cv_status::timeout);
    }

    if (dev_fd_) {
        len = ioctl(dev_fd_, HIDIOCSFEATURE(length), data);
        if (len < 0) {
            PUMA_LOG_ERROR("Generic Control Set Feature Failed, errno = {}, {}", 
                errno, strerror(errno));
        }
    } else {
        PUMA_LOG_WARN("No device handle available");
    }
    return len;
}

int32_t HidrawDriver::getFeatureReport(uint8_t* data, uint32_t length) {
    assert(data && length > 0);
    int32_t len = 0;
    
    if (device_state_ == DeviceState::DEVICE_STATE_DEACTIVATED) {
        std::cv_status status = std::cv_status::no_timeout;
        std::unique_lock <std::mutex> lck(hid_init_mutex_);
        do {
            if (device_state_ == DeviceState::DEVICE_STATE_EXITED) return false;
            status = hid_init_cond_.wait_for(lck, std::chrono::milliseconds(50));
        } while(status == std::cv_status::timeout);
    }
    
    if (dev_fd_) {
        len = ioctl(dev_fd_, HIDIOCGFEATURE(length), data);
        if (len < 0) {
            PUMA_LOG_ERROR("Generic Control Get Feature Failed, errno = {}, {}", 
                errno, strerror(errno));
        }
    } else {
        PUMA_LOG_WARN("No device handle available");
    }
    return len;
}

bool HidrawDriver::getPidVid(uint16_t& pid, uint16_t& vid) {
    if (vid_ == 0 || pid_ == 0) {
        PUMA_LOG_ERROR("vid and pid must be not zero");
        return false;
    } else {
        vid = vid_;
        pid = pid_;
        return true;
    }
}

void HidrawDriver::run() {
    setStartTime();
    
    pthread_setname_np(pthread_self(), "sensor_hi");    

    hid_data_t hid_data;
    hid_data.device_start_time = startTime();
    while (running_) {
        device_state_ = DeviceState::DEVICE_STATE_ACTIVATED;
        hid_init_cond_.notify_all();
        hid_data.type = HidFactory::HidDataType::HID_DATA_RAW;
        ssize_t len = read(dev_fd_, hid_data.padding, sizeof(hid_data.padding));
        if (len > 0) {
            hid_data.size = len;
            hid_data.sys_time_after_read_usb = framework::util::FMonotonicGetNs();
            if (frame_callback_)
                frame_callback_(hid_data);
        } else {
           // usleep(100);
           // PUMA_LOG_TRACE("hid cable off, plug_callback_:{},error={},errror_str={}",plug_callback_!=nullptr?true:false, errno, strerror(errno));
            if (!plug_executed && plug_callback_!= nullptr) {
                PUMA_LOG_INFO("mcu cable off callback,error={},error_str={}",errno, strerror(errno));
                HidStatusData status;
                status.status = HidStatusData::HidStatusType::HID_UNPLUG;
                plug_callback_(&status);
                plug_executed = true;
            }
            continue;
        }
    }

    device_state_ = DeviceState::DEVICE_STATE_EXITED;
    PUMA_LOG_INFO("Raw driver process stopped.");
}
#endif