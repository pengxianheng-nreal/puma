#pragma once
#include <puma/env/system_config.h>
#if defined(PUMA_SYSTEM_WINDOWS)
#include <pthread.h>
#include <puma/driver/camera/camera_driver.h>
#include <ocv_dshow/cap_dshow.hpp>

class CameraDshow : public CameraDriver {
public:
    CameraDshow(std::function<void (const GenericShmWrapper&)> _callback);

    ~CameraDshow();

    virtual bool start() override;

    virtual void stop() override;

    virtual void setAutoExposure(int _auto_exposure) override;

    virtual void setExposureTime(double _exposure_time) override;

    virtual void setGain(int _gain) override;

    virtual bool setVideoProperties(uint32_t prop, uint8_t value) override;

    virtual bool getVideoProperties(uint32_t prop, uint8_t& value) override;

protected:
    virtual void run() override;

    bool find_device();
    double getProperty( int property_id );
    bool setProperty( int property_id, double value );

    void listUsbDevices();
    int restartDevice();

protected:
    xreal_dshow::videoInput video_input_;
    int32_t video_input_device_{-1};
    uint8_t * video_input_buffer_{nullptr};
    pthread_t camera_read_thread_;
    int restart_flag_{0};
    const uint32_t OV_MAX_EXPOSURE_ADDR{0x8018FFF4};
    const uint32_t OV_LUMA_ADDR{0x8018FFE1};
    uint8_t max_exposure_time_{17}; /// 17 ms
    uint8_t luma_value_{50}; /// 50
};
#endif