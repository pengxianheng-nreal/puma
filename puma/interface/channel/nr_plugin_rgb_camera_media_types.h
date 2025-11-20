#pragma once

#include "nr_plugin_types.h"

#ifdef NRAPP

#include "nr_plugin_rgb_camera_media_types.inc"

#else

#pragma pack(1)
typedef struct NRMediaInfo {
    union {
        struct {
            NRResolutionInfo resolution;
        };
        uint8_t padding[96];
    };

} NRMediaInfo;

#pragma pack()

#endif // NRAPP