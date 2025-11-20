#pragma once

#include <puma/common/device.h>
#include <puma/driver/camera/camera_utils.h>
#include <puma/common/common.h>
#include <mutex>
#include <cmath>
#include <condition_variable>
#include <puma/util/log.h>
#include <puma/util/types.h>

using namespace puma;

class CameraDriver : public Device{
public:
    enum class IO_Method {
        IO_METHOD_READ,
        IO_METHOD_MMAP,
        IO_METHOD_USERPTR,
    };
    enum class AEAGMode {
        AE_MODE_OFF = 0,
        AE_MODE_ISP = 1,
        AE_MODE_SOFT = 2
    };

public:
    CameraDriver(std::function<void (const GenericShmWrapper&)> _callback) :
        frame_callback_(std::move(_callback)) {}

    virtual ~CameraDriver() { stop(); }

public:
    inline virtual void setCameraProperties(const CameraInfo& info) { info_ = info; }
    inline virtual const CameraInfo& cameraProperties() const { return info_; }

    /// validate only after device started
    virtual inline uint64_t threadId() {
        std::unique_lock <std::mutex> lck(thread_id_mutex_);
        while (thread_id_ == 0ULL) { thread_id_cond_.wait(lck); }
        return thread_id_;
    }

    virtual void setAutoExposure(int _auto_exposure) {
        AE_mode_ = static_cast<AEAGMode>(_auto_exposure);
        PUMA_LOG_TRACE("set ae enable :{}", static_cast<int>(AE_mode_));
    }

    virtual void setExposureTime(double _exposure_time) {
        if (AE_mode_ == AEAGMode::AE_MODE_ISP) {
            return;
        }
        //on flora,exposure_time range is [0.1ms,15.9ms]. Date 20231122
        exposure_time_ = _exposure_time;
        PUMA_LOG_TRACE("set exposure time :{}", exposure_time_);
    }
    virtual void setGain(int _gain) {
        if (AE_mode_ == AEAGMode::AE_MODE_ISP) {
            return;
        }
        if(device_type_ == DEVICE_TYPE_LIGHT) {
            //only light need check gain validation, on flora do not check, becase the min_max of flora is not=[1,255]
            //on flora,the gain range is [100,787] ,date 20231122
            if (_gain < min_gain_) {
                _gain = min_gain_;
            }
            if (_gain > max_gain_) {
                _gain = max_gain_;
            }
        }
        gain_ = _gain;
        PUMA_LOG_TRACE("set gain :{}", gain_);
    }
    virtual void setGamma(const std::vector<uint8_t>& gamma) {
        gamma_coeffs_ = gamma;
    }
    virtual const std::vector<uint8_t>& getGamma() const {
        return gamma_coeffs_;
    }

    virtual bool setVideoProperties(uint32_t , uint8_t ) { return false; }

    virtual bool getVideoProperties(uint32_t , uint8_t& ) { return false; }

    virtual bool setVideoProperties(uint32_t , const std::vector<uint8_t>& ) { return false; }

    virtual bool getVideoProperties(uint32_t , std::vector<uint8_t>& ) { return false; }

    /// set user space memory, which will be mmap to kernel memory
    virtual bool setOutsideMem(const void *, size_t ) { return false; }
    virtual void setIOMethod(IO_Method _io_method) { io_method_ = _io_method; }
    virtual bool valid() { return true; }

    void SetCopyFrameFunction(CopyFrameFunction* copy_frame_fuc) {
        copy_frame_ = copy_frame_fuc;
    }

    void SetDeviceType(DeviceType device_type) {
        device_type_ = device_type;
    }

protected:
    void softAEAG(frame_t &frame) {
        if (AE_mode_ != AEAGMode::AE_MODE_SOFT || frame.data == nullptr) {
            PUMA_LOG_TRACE("softAEAG frame.data = {:p} OR AE_mode_ = {} which is not equal to AE_MODE_SOFT({})",
                (void*)frame.data, static_cast<int>(AE_mode_), static_cast<int>(AEAGMode::AE_MODE_SOFT));
            return;
        }
        ++current_frame_index_;

        if (current_frame_index_ > 5 && current_frame_index_ % 8 == 0) {
            double image_mean, mean_diff;
            double graySum = 0;
            for (size_t i=0; i<frame.data_bytes; ++i) {
                graySum += (double)frame.data[i] * (1 + (double)frame.data[i] * (double)frame.data[i] / 200000);
            }
            image_mean = graySum / frame.data_bytes;

            double gain_set = gain_;
            double exp_set = exposure_time_;

            mean_diff = 60 - image_mean;

            if (std::abs(mean_diff) > 20) {
                exp_set += mean_diff / 30;

                exp_set = exp_set > max_valid_exposure_time_ ? max_valid_exposure_time_ : exp_set;
                exp_set = exp_set < min_valid_exposure_time_ ? min_valid_exposure_time_ : exp_set;

                if (mean_diff < 0 && gain_ > min_valid_gain_) {
                    exp_set = exposure_time_;
                }

                if (exposure_time_ != exp_set) {
                    setExposureTime(exp_set);
                } else {
                    double k = (mean_diff / 20) * ((max_valid_gain_ - min_valid_gain_ + 1) / 15.0);
                    gain_set += k > 0 ? std::ceil(k) : std::floor(k);
                    gain_set = gain_set > max_valid_gain_ ? max_valid_gain_ : gain_set;
                    gain_set = gain_set < min_valid_gain_ ? min_valid_gain_ : gain_set;
                    if (gain_ != (uint8_t)gain_set) {
                        setGain((uint8_t)gain_set);
                    }
                }
            }
        }
    }


protected:
    /// image properties
    CameraInfo info_;

    /// the exposure time of frame being processed in ns
    double exposure_time_{3};
    /// image sensor gain
    uint16_t gain_{2};
    /// image AEAG mode
    AEAGMode AE_mode_{AEAGMode::AE_MODE_ISP};

    /// image maximum gain
    uint16_t max_gain_{255};

    /// image minimum gain
    uint16_t min_gain_{1};

    /// maximum valid gain
    uint16_t max_valid_gain_{256*3/8};

    /// minimum valid gain
    uint16_t min_valid_gain_{1};

    /// image maximum exposure time
    int16_t max_valid_exposure_time_{8};

    /// image minimum exposure time
    int16_t min_valid_exposure_time_{1};

    /// callback functions
    std::function<void (const GenericShmWrapper&)> frame_callback_{nullptr};

    /// current frame information
    GenericShmWrapper curr_frame_;

    IO_Method io_method_{IO_Method::IO_METHOD_MMAP};
    uint64_t current_frame_index_;

    uint64_t thread_id_{0};
    std::mutex thread_id_mutex_;
    std::condition_variable thread_id_cond_;

    std::vector<uint8_t> gamma_coeffs_;

    CopyFrameFunction* copy_frame_{ nullptr };

    DeviceType device_type_{ DeviceType::DEVICE_TYPE_INVALID };

    std::vector<CameraFrameInfo> support_frame_infos_;
};