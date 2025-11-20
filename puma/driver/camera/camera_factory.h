#pragma once

#include <memory>
#include "camera_driver.h"

class CameraFactory {
public:
    enum class CameraType {
        CAMERA_UVC = 1,
        CAMERA_V4L2,
        CAMERA_DSHOW,
        CAMERA_MAC
    };

public:
    CameraFactory() = default;
    virtual ~CameraFactory() = default;

    static std::shared_ptr<CameraDriver> 
    createDevice(CameraType type, std::function<void (const GenericShmWrapper&)> _callback);

};