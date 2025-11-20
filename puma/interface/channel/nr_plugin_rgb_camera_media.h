#pragma once

#include "nr_plugin_interface.h"
#include "nr_plugin_rgb_camera_media_types.h"
typedef struct NRRgbCameraMediaProvider {

    NRPluginResult(NR_INTERFACE_API *RgbCameraStartVideo)(
        NRPluginHandle handle,
        const char * path,
        uint32_t path_size,
        const NRMediaInfo * media_info
    );

    NRPluginResult(NR_INTERFACE_API *RgbCameraStopVideo)(
        NRPluginHandle handle
    );

    NRPluginResult(NR_INTERFACE_API *RgbCameraStartPhoto)(
        NRPluginHandle handle,
        const char * path,
        uint32_t path_size,
        const NRMediaInfo * media_info
    );

    NRPluginResult(NR_INTERFACE_API *RgbCameraStopPhoto)(
        NRPluginHandle handle
    );

    NRPluginResult(NR_INTERFACE_API *RgbCameraIsOccupied)(
        NRPluginHandle handle,
        bool * occupied
    );
} NRRgbCameraMediaProvider;

NR_DECLARE_INTERFACE(NRRgbCameraMediaInterface) {

    NRPluginResult(NR_INTERFACE_API *RegisterMediaProvider)(
        NRPluginHandle handle,
        const NRRgbCameraMediaProvider * provider,
        uint32_t provider_size
    );

    NRPluginResult(NR_INTERFACE_API *RgbCameraFinishPhoto)(
        NRPluginHandle handle
    );
};

NR_REGISTER_INTERFACE_GUID(0x5010743BFF3449D8ULL, 0x8B1F484A7B6B849CULL,
                            NRRgbCameraMediaInterface)

