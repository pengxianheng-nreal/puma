#pragma once

#include <puma/env/system_config.h>

// Use int32_t for ABI compatibility.
#if __cplusplus >= 201100 || defined(PUMA_SYSTEM_WINDOWS)
#define PUMA_ENUM8(__TYPE__) enum __TYPE__ : int8_t
#define PUMA_ENUM16(__TYPE__) enum __TYPE__ : int16_t
#define PUMA_ENUM32(__TYPE__) enum __TYPE__ : int32_t
#define PUMA_ENUM64(__TYPE__) enum __TYPE__ : int64_t
#define PUMA_ENUM(__TYPE__) PUMA_ENUM32(__TYPE__)
#else
#error "C++ versions less than C++11 are not supported."
#endif

namespace puma {
    // device type
PUMA_ENUM(DeviceType){
    DEVICE_TYPE_INVALID = 0,
    DEVICE_TYPE_LIGHT = 1,
    DEVICE_TYPE_AIR = 2,
    DEVICE_TYPE_P55E = 3,
    DEVICE_TYPE_P55F = 4,
    DEVICE_TYPE_FLORA = 5,
    DEVICE_TYPE_GINA_FLORA = 10,
    DEVICE_TYPE_GINA_M = 11,
    DEVICE_TYPE_GINA_L = 12,
    DEVICE_TYPE_HYLLA_M = 13,
    DEVICE_TYPE_HYLLA_L = 14,
    DEVICE_TYPE_GS = 15,
    DEVICE_TYPE_HONOR_AIR = 1001,
    DEVICE_TYPE_HISENSE = 1002,
    DEVICE_TYPE_ROG_M = 1003,
    DEVICE_TYPE_ROG_L = 1004,
};

} // namespace puma