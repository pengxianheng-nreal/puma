#include <puma/env/system_config.h>
#include <puma/util/log.h>
#include "camera_factory.h"
#include "camera_dshow.h"
#include "camera_mac.h"
#include "camera_uvc.h"
#include "camera_v4l2.h"

std::shared_ptr<CameraDriver> 
CameraFactory::createDevice(CameraType type, 
             std::function<void (const GenericShmWrapper&)> _callback) {
    std::shared_ptr<CameraDriver> camera = nullptr;
    switch(type) {
#if defined(PUMA_SYSTEM_LINUX) || defined(PUMA_SYSTEM_ANDROID)
        case CameraType::CAMERA_UVC:
            camera = std::make_shared<CameraUvc>(_callback);
            break;

        case CameraType::CAMERA_V4L2:
            camera = std::make_shared<CameraV4l2>(_callback);
            break;
#endif
        default:
#if defined(PUMA_SYSTEM_WINDOWS)
            camera = std::make_shared<CameraDshow>(_callback);
#elif defined(PUMA_SYSTEM_MACOS)
            camera = std::make_shared<CameraMac>(_callback);
#else
            PUMA_LOG_ERROR("Unknown camera type");
#endif
            break;
    }
    return camera;
}
