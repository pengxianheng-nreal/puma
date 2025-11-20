#include <puma/env/system_config.h>
#include "hid_factory.h"
#include "hidraw_driver.h"
#include "hidapi_driver.h"
#include <puma/util/log.h>


std::shared_ptr<HidDriver> 
HidFactory::createDevice(HidFactory::HidType type, std::function<void (const hid_data_t&)> _callback) {
    std::shared_ptr<HidDriver> hid_device = nullptr;
    switch(type) {   
#if defined(PUMA_SYSTEM_LINUX) || defined(PUMA_SYSTEM_ANDROID)
    case HidFactory::HidType::HIDRAW:
        hid_device = std::make_shared<HidrawDriver>(_callback);
#endif
        break;

    case HidFactory::HidType::HIDAPI:
        hid_device = std::make_shared<HidapiDriver>(_callback);
        break;

    default:
        PUMA_LOG_ERROR("Unknown hid type");
        break;
    }
    return hid_device;
}