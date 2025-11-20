#pragma once

#include <puma/common/device.h>
#include <puma/driver/hid_driver/hid_utils.h>
#include <mutex>
#include <string.h>
#include <condition_variable>
#include <puma/common/threadsafe-queue.h>

class HidDriver : public Device {
public:
    HidDriver(std::function<void (const hid_data_t&)> _callback) :
        frame_callback_(std::move(_callback)) {
        memset(&info_, 0, sizeof(info_));
    }
    virtual ~HidDriver(){ stop(); };

    virtual bool start() override {
        return Device::start();
    }

    virtual void stop() override {
        Device::stop();
        device_state_ = DeviceState::DEVICE_STATE_EXITED;
    }

    virtual inline void setHidInfo(const HidInfo& _info) { info_ = _info; }
    virtual inline const HidInfo& hidInfo() const { return info_; }

    /// validate only after device started
    virtual inline uint64_t threadId() {
        std::unique_lock <std::mutex> lck(thread_id_mutex_);
        while (thread_id_ == 0ULL) { thread_id_cond_.wait(lck); }
        return thread_id_;
    }

    virtual inline void hotplug_register(std::function<int32_t(void*)> _callback) {
        plug_callback_ = _callback;
    }

    virtual int32_t setFeatureReport(const uint8_t* , uint32_t ) {
        return -1;
    };

    virtual int32_t getFeatureReport(uint8_t* , uint32_t ) {
        return -1;
    }

    virtual bool getPidVid(uint16_t& , uint16_t& ) {
        return false;
    }

protected:
    std::function<void (const hid_data_t&)> frame_callback_{nullptr};
    HidInfo info_;
    uint64_t thread_id_{0};
    std::mutex thread_id_mutex_;
    std::condition_variable thread_id_cond_;

    std::mutex hid_init_mutex_;
    std::condition_variable hid_init_cond_;

    std::function<int32_t(void*)> plug_callback_{nullptr};
    std::atomic_bool plug_executed{false};
};