#pragma once

#include <framework/util/os_time.h>
#include <puma/driver/camera/camera_utils.h>
#include <puma/util/log.h>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>
#include <string>

class Device {
public:
    enum DeviceState {
        DEVICE_STATE_DEACTIVATED = -1,
        DEVICE_STATE_ACTIVATED = 0,
        DEVICE_STATE_EXITED = 1,
    };

public:
    Device() = default;

    virtual ~Device() { stop(); }

public:
    /// set product vid & pid
    virtual void addProductID(int16_t _vid, int16_t _pid, int16_t _interface_number = 0) {
        product_id_.emplace_back(std::tuple<int16_t, int16_t, int16_t>(_vid, _pid, _interface_number));
    }

    /// set product name which can be obtained by capabilities card info
    virtual void addProductName(const std::vector<std::string>& _product_name) {
        PUMA_LOG_TRACE("addProductName input name size:{}", _product_name.size());
        for (const auto & product: _product_name) {
            product_name_.emplace_back(product);
        }        
    }

    /// set device node, such as /dev/video0
    virtual void setDeviceNode(const std::string& _node) {
        device_node_name_ = _node;
    }

    virtual void setStartTime() {
        device_start_time_ = framework::util::FMonotonicGetNs();
    }

    virtual int64_t startTime() const { return device_start_time_; }

    virtual bool init() { return true; }

    virtual bool start() {
        running_ = true;
        work_thread_.reset(new std::thread(std::bind(&Device::run, this)));
        return running_;
    }

    virtual void stop() {
        if (running_) {
            running_ = false;
            if (work_thread_ && work_thread_->joinable()) {
                if (work_thread_->get_id() != std::this_thread::get_id()) {
                    work_thread_->join();
                } else {
                    work_thread_->detach();
                }
            }
            work_thread_.reset();
        }
    }

    /// set offline directory
    virtual void setOfflinePath(const std::string& _path) {
        offline_path = _path;
        if (offline_path.at(offline_path.length()-1) == '/') {
            offline_path.pop_back();
        }
    }

    /// set properties
    virtual bool setProperties(int32_t , int32_t ){ return false; }
    virtual bool setProperties(int32_t , double ){ return false; }
    virtual bool setProperties(const uint8_t* , uint32_t ){ return false; }

    virtual void clear() {
        product_id_.clear();
        product_name_.clear();
    }

protected:
    virtual void run(){}

protected:
    /// VID & PID list
    std::vector<std::tuple<int16_t, int16_t, int16_t>> product_id_;

    /// product name list
    std::vector<std::string> product_name_;

    /// device node
    std::string device_node_name_;

    /// work thread related to
    std::atomic_bool running_{false};
    std::shared_ptr<std::thread> work_thread_;

    /// offline directory
    std::string offline_path{""};

    /// device setup time through system time
    int64_t device_start_time_{-1};  // us

    std::atomic<DeviceState> device_state_{
        DeviceState::DEVICE_STATE_DEACTIVATED};
};
