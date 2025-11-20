#include "hidapi_driver.h"
#include "hid_factory.h"
#include <functional>
#include <memory>
#include <thread>
#include <atomic>


#include <framework/util/os_time.h>

#include <puma/util/log.h>
#include <spdlog/fmt/bin_to_hex.h>
#include <puma/env/system_config.h>

extern write_hid_log_t* write_hid_log_function;
extern bool log_hid_all_level;
HidapiDriver::HidapiDriver(std::function<void (const hid_data_t&)> _callback) :
    HidDriver(_callback) {
    if(!write_hid_log_function) {
        write_hid_log_function =  &HidapiDriver::Log;
    }
    if (puma::Logger::GetInstance()->IsLogAllLevel())  {
        log_hid_all_level = true;
    }
}

void HidapiDriver::Log(HidLogLevel log_level,const char *log) {
    switch (log_level)
    {
    case HID_LOG_LEVEL_TRACE:
    	PUMA_LOG_TRACE("[HID_LOG] {}", log)
        break;
    case HID_LOG_LEVEL_DEBUG:
    	PUMA_LOG_DEBUG("[HID_LOG] {}", log)
        break;
    case HID_LOG_LEVEL_INFO:
    	PUMA_LOG_INFO("[HID_LOG] {}", log)
        break;
    case HID_LOG_LEVEL_WARN:
    	PUMA_LOG_WARN("[HID_LOG] {}", log)
        break;
    case HID_LOG_LEVEL_ERROR:
    	PUMA_LOG_ERROR("[HID_LOG] {}", log)
        break;
    case HID_LOG_LEVEL_FATAL:
    	PUMA_LOG_FATAL("[HID_LOG] {}", log)
        break;
    default:
        break;
    }
}

HidapiDriver::~HidapiDriver() {
    HidDriver::stop();
    if (dev_fd_ != nullptr) {
#if defined(PUMA_SYSTEM_ANDROID)|| defined(PUMA_SYSTEM_LINUX)
        hid_callback_deregister(dev_fd_);
#endif
        hid_close(dev_fd_);
        hid_exit(&context_);
        dev_fd_ = nullptr;
    }
}

bool HidapiDriver::init() {
    if (!initDevice()) {
        return false;
    }
    return true;
}

bool HidapiDriver::start() {
#if defined(PUMA_SYSTEM_WINDOWS) || defined(PUMA_SYSTEM_MACOS)
    use_callback_ = false;
#else
    use_callback_ = info_.use_callback;
#endif
    bool success = false;
    PUMA_LOG_INFO("Generic Control callback {}", use_callback_);
    if (use_callback_) {
#if defined(PUMA_SYSTEM_ANDROID)|| defined(PUMA_SYSTEM_LINUX)
        success = hid_callback_register(dev_fd_, callback, this) == 0;
        device_state_ = DeviceState::DEVICE_STATE_ACTIVATED;
        hid_init_cond_.notify_all();
        setStartTime();
#endif
    } else {
        success = HidDriver::start();
    }
    return success;
}

void HidapiDriver::stop() {
    if (use_callback_) {
        if (dev_fd_ != nullptr) {
#if defined(PUMA_SYSTEM_ANDROID)|| defined(PUMA_SYSTEM_LINUX)
            hid_callback_deregister(dev_fd_);
#endif            
            device_state_ = DeviceState::DEVICE_STATE_EXITED;
            PUMA_LOG_INFO("Generic Control process stopped.");
        }
    } else {
        HidDriver::stop();
    }
}

bool HidapiDriver::setProperties(const uint8_t* cmd, uint32_t cmd_size) {
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
    PUMA_LOG_TRACE("Generic Control initialization completed");

    int hid_write_maximum_retry_time = 5;
    int hid_write_retry_time = 0;
    if (dev_fd_) {
        while (hid_write_retry_time < hid_write_maximum_retry_time) {
            len = hid_write(dev_fd_, cmd, cmd_size);
            if (len > 0) break; // success
#ifdef PUMA_SYSTEM_WINDOWS
            //On windows ,errno is always 0 ,so need call GetLastError to process
            const DWORD dwLastError = GetLastError();
            if (ERROR_DEVICE_NOT_CONNECTED == dwLastError) {
                if (!set_property_error_no_device_output_) {
                    PUMA_LOG_ERROR("device not connected!,no need retry,len:{}",len);
                    set_property_error_no_device_output_ = true;
                }
                break;
            }
            else {
                PUMA_LOG_INFO("going to retry,last_error:{}... {}/{}", dwLastError, hid_write_retry_time, hid_write_maximum_retry_time);
                hid_write_retry_time++;
            }
#else
            if (errno == ENODEV) { //ENODEV:Operation not supported by device 
                if (!set_property_error_no_device_output_) {
                    PUMA_LOG_ERROR("Generic Control write error len = {}, error({}), {}, ENODEV={}", 
                        len, errno, strerror(errno),ENODEV);
                    set_property_error_no_device_output_ = true;
                }
                break; // no need to retry
            } else {
                PUMA_LOG_ERROR("Generic Control write error len = {}, error({}), {}, other error", 
                        len, errno, strerror(errno));
                if (errno == EDEADLK) {//Resource deadlock avoided
                    hid_write_retry_time ++;
                    PUMA_LOG_INFO("going to retry... {}/{},EDEADLK={}", hid_write_retry_time, hid_write_maximum_retry_time,EDEADLK);
                } else {
                    break; // no need to retry
                }
            }
#endif
        }
    } else {
        PUMA_LOG_WARN("Generic Control has no handle available");
    }
    return (len > 0);
}

int32_t HidapiDriver::setFeatureReport(const uint8_t* data, uint32_t length) {
    return hid_send_feature_report(dev_fd_, data, length);
}

int32_t HidapiDriver::getFeatureReport(uint8_t* data, uint32_t length) {
    return hid_get_feature_report(dev_fd_, data, length);
}

bool HidapiDriver::getPidVid(uint16_t& pid, uint16_t& vid) {
    return hid_get_pid_vid(dev_fd_, &pid, &vid) == 0;
}

bool HidapiDriver::initDevice() {
#if defined(PUMA_SYSTEM_WINDOWS)
    context_.lib_handle = nullptr;
    context_.initialized = FALSE;
#else
    context_ = NULL;
#endif

    if (info_.usb_fd > 0) {
#if defined(PUMA_SYSTEM_LINUX) || defined(PUMA_SYSTEM_ANDROID)
        dev_fd_ = hid_open_fd(&context_, info_.usb_fd, info_.interface_number);
        PUMA_LOG_INFO("Generic Control open with handle");
#endif
    } else {
        if (hid_init(&context_)) {
            device_state_ = DeviceState::DEVICE_STATE_EXITED;
            PUMA_LOG_FATAL("Generic Control init failed");
            return false;
        }

        if (!device_node_name_.empty()) {
            dev_fd_ = hid_open_path(&context_, device_node_name_.c_str());
            if (!dev_fd_) {
                PUMA_LOG_ERROR(
                    "Open device node {} failed, errno = {}, error = {}",
                    device_node_name_, errno, strerror(errno));
            }
        }

        // TODO: we've already enumerated in finddevice()... dev_fd_ released thow.
        // so, why should we release it and enumerate again?
        if (!dev_fd_) {
            for (auto it : product_id_) {
#if defined(PUMA_SYSTEM_WINDOWS)
                SetLastError(0);
#endif
                dev_fd_ = hid_open(&context_, std::get<0>(it), std::get<1>(it),
                                   std::get<2>(it), NULL);
                PUMA_LOG_TRACE("HidapiDriver::initDevice, dev_fid_={},vid=0x{:x},pid=0x{:x},interface_number={}",!!dev_fd_,std::get<0>(it), std::get<1>(it), std::get<2>(it));
                if (NULL != dev_fd_) {
                    PUMA_LOG_TRACE("HidapiDriver::initDevice, open device success,vid=0x{:x},pid=0x{:x},interface_number={}",std::get<0>(it), std::get<1>(it), std::get<2>(it));
                    break;
                } else {
#if defined(PUMA_SYSTEM_WINDOWS)
                    int win_error = GetLastError();
                    PUMA_LOG_TRACE("HidapiDriver::initDevice cannot open device, win_error={}, errno = {}, error = {}",win_error, errno,strerror(errno));
#else
                    PUMA_LOG_TRACE("HidapiDriver::initDevice cannot open device, errno = {}, error = {}",errno,strerror(errno));
#endif
                }
            }
        }
    }

    if (!dev_fd_) {
        device_state_ = DeviceState::DEVICE_STATE_EXITED;
#if defined(PUMA_SYSTEM_WINDOWS)
        int win_error = GetLastError();
        PUMA_LOG_FATAL("Generic Control open failed, win_error={}, errno = {}, error = {}", win_error, errno, strerror(errno));
#else
        PUMA_LOG_FATAL("Generic Control open failed, errno = {}, error = {}", errno, strerror(errno));
#endif

        return false;
    }

#if defined(PUMA_SYSTEM_ANDROID) || defined(PUMA_SYSTEM_LINUX)
    {
        std::unique_lock <std::mutex> lck(thread_id_mutex_);
        thread_id_ = hid_get_threadId(dev_fd_);
        thread_id_cond_.notify_all();
        PUMA_LOG_TRACE("Generic Control thread_id_ = {}", thread_id_);
    }
#endif
    return true;
}

void HidapiDriver::run() {
#if defined(PUMA_SYSTEM_ANDROID)
    pthread_setname_np(pthread_self(), "sensor_hi");
#endif
    setStartTime();
    hid_data_t hid_data;
    hid_data.device_start_time = startTime();
    while (running_) {
        hid_init_cond_.notify_all();
        hid_data.type = HidFactory::HidDataType::HID_DATA_RAW;
        int32_t len = hid_read_timeout(dev_fd_, hid_data.padding, sizeof(hid_data.padding), 100);
        //PUMA_LOG_TRACE("HidapiDriver::run  hid_read_timeout, result, len={}",len);
        if (len == 0) {
            continue;
        } else if (len < 0) {
            //PUMA_LOG_TRACE("hid cable off, plug_callback_:{},error={},errror_str={}",plug_callback_!=nullptr?true:false, errno, strerror(errno));
            device_state_ = DeviceState::DEVICE_STATE_EXITED;
            if (!plug_executed && plug_callback_!= nullptr) {
                PUMA_LOG_INFO("mcu cable off callback,error={},error_str={}",errno, strerror(errno));
                HidStatusData status;
                status.status = HidStatusData::HidStatusType::HID_UNPLUG;
                plug_callback_(&status);
                plug_executed = true;
            }
            continue;
        }

        device_state_ = DeviceState::DEVICE_STATE_ACTIVATED;
        
        hid_data.size = len;
        hid_data.sys_time_after_read_usb = framework::util::FMonotonicGetNs();
        if (frame_callback_)
            frame_callback_(hid_data);
    }
    device_state_ = DeviceState::DEVICE_STATE_EXITED;
    // hid_close(dev_fd_);
    // hid_exit(&contex_);
    // dev_fd_ = nullptr;
    PUMA_LOG_INFO("Generic Control process stopped.");
}

void HidapiDriver::callback(void* data, int32_t data_len, void* user_ptr) {
    uint64_t current_ts_nano = framework::util::FMonotonicGetNs();
    HidapiDriver* driver = (HidapiDriver*)user_ptr;
        
    if (driver->frame_callback_) {
        if (data_len > 0) {
            hid_data_t hid_data;
            hid_data.device_start_time = driver->startTime();
            hid_data.type = HidFactory::HidDataType::HID_DATA_RAW;
            hid_data.size = data_len;
            hid_data.sys_time_after_read_usb = current_ts_nano;
            if(data_len > (int32_t)(sizeof(hid_data.padding))) {
                PUMA_LOG_WARN("recv data len:{} > padding: {}!!!",data_len,sizeof(hid_data.padding));
            }
            memcpy(hid_data.padding, data, std::min((int32_t)sizeof(hid_data.padding), data_len));
            driver->frame_callback_(hid_data);
        } else {
            //PUMA_LOG_TRACE("hid cable off,is_imu:{},plug_callback_:{},data:{},data_len:{},user_ptr:{},error={},error_str={}",
            //driver->info_.is_imu,driver->plug_callback_!=nullptr?true:false, data,data_len,user_ptr,errno, strerror(errno));
            if (!driver->plug_executed && driver->plug_callback_!= nullptr) {
                PUMA_LOG_INFO("mcu cable off callback,data:{},data_len:{},user_ptr:{},error={},error_str={}", 
                data,data_len,user_ptr,errno, strerror(errno));
                HidStatusData status;
                status.status = HidStatusData::HidStatusType::HID_UNPLUG;
                driver->plug_callback_(&status);
                driver->plug_executed = true;
            }
        }
    }
}