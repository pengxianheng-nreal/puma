#pragma once

#include "nr_plugin_lifecycle.h"
#include "nr_plugin_glasses_types.h"
typedef struct NRStreamingProvider {

    NRPluginResult(NR_INTERFACE_API *ReleaseDpFrame)(
        NRPluginHandle handle,
        const void * dp_frame_data
    );
} NRStreamingProvider;

NR_DECLARE_INTERFACE(NRStreamingInterface) {

    NRPluginResult(NR_INTERFACE_API *RegisterLifecycleProvider)(
        const char * plugin_id,
        const char * plugin_version,
        const NRPluginLifecycleProvider * provider,
        uint32_t provider_size
    );

    NRPluginResult(NR_INTERFACE_API *RegisterProvider)(
        NRPluginHandle handle,
        const NRStreamingProvider * provider,
        uint32_t provider_size
    );

    NRPluginResult(NR_INTERFACE_API *SetNativeWindow)(
        NRPluginHandle handle,
        void * native_window
    );

    NRPluginResult(NR_INTERFACE_API *NotifyDpFrameAvailable)(
        NRPluginHandle handle,
        const void * dp_frame_data
    );
};

NR_REGISTER_INTERFACE_GUID(0x01D8737BB605EA43ULL, 0xE23F11A99659AEC4ULL,
                            NRStreamingInterface)

