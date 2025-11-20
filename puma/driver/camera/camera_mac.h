#pragma once
#include <puma/env/system_config.h>
#if defined(PUMA_SYSTEM_MACOS)
#include <memory>
#include <puma/driver/camera/camera_driver.h>

class CvCaptureCAM;

class CameraMac : public CameraDriver {
public:
    CameraMac(std::function<void (const GenericShmWrapper&)> _callback);

    ~CameraMac();

    virtual bool start() override;

    virtual void stop() override;

    virtual void setAutoExposure(int _auto_exposure) override;

    virtual void setExposureTime(double _exposure_time) override;

    virtual void setGain(int _gain) override;

    virtual bool setVideoProperties(uint32_t prop, uint8_t value) override;

    virtual bool getVideoProperties(uint32_t prop, uint8_t& value) override;

protected:
    virtual void run() override;

    std::shared_ptr<CvCaptureCAM> mac_cam_;
};
#endif