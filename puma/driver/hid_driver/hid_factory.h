#pragma once

#include <memory>
#include "hid_driver.h"

class HidFactory {
public:
    enum class HidType {
        HIDRAW = 0,
        HIDAPI = 1
    };
    enum HidDataType { // for client callback
        HID_DATA_RAW = 0,
        HID_DATA_IMU = 1,
        HID_DATA_MAG = 2,
        HID_DATA_VSYNC = 3,
        HID_DATA_MCU_STATUS = 4,
        HID_DATA_TEMPERATURE = 5,
        HID_DATA_HEARTBEAT = 6,
        HID_DATA_KEY_CLICK = 7,
        HID_DATA_GET_SET_DISPLAY_MODE_ACK = 8,
        HID_DATA_EVENTS_REPORT = 9,
    };

public:
    HidFactory() = default;
    virtual ~HidFactory() = default;

    static std::shared_ptr<HidDriver> 
    createDevice(HidType type, std::function<void (const hid_data_t&)> _callback);
};