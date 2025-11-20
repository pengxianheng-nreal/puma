#pragma once

#include "nr_plugin_types.h"

#ifdef NRSDK

#include "nr_plugin_ambient_light_types.inc"

#else

#pragma pack(1)

typedef struct NRAmbientLightData {
    union {
        struct {
            uint64_t hmd_time_nanos_system;
            uint64_t hmd_time_nanos_device;
            uint32_t frame_id;
            uint32_t lux;
            uint32_t f_value;
            uint32_t r_value;
            uint32_t g_value;
            uint32_t b_value;
            uint32_t c_value;
        };
        uint8_t padding[64];
    };

} NRAmbientLightData;

#pragma pack()

#endif // NRSDK