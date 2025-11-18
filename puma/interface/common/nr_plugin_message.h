#pragma once

#include "nr_plugin_interface.h"
#include "nr_plugin_types.h"
typedef struct NRMessageProvider {
    NRPluginResult(NR_INTERFACE_API *OnMessage)(
        NRPluginHandle handle,
        int32_t connid,
        const void * data,
        uint32_t size
    );
} NRMessageProvider;

NR_DECLARE_INTERFACE(NRMessageInterface) {
    NRPluginResult(NR_INTERFACE_API *RegisterProvider)(
        NRPluginHandle handle,
        const NRMessageProvider * provider,
        uint32_t provider_size
    );
    NRPluginResult(NR_INTERFACE_API *SendMessage)(
        NRPluginHandle handle,
        int32_t connid,
        const void * data,
        uint32_t size
    );
    NRPluginResult(NR_INTERFACE_API *BroadcastMessage)(
        NRPluginHandle handle,
        const void * data,
        uint32_t size
    );
};

NR_REGISTER_INTERFACE_GUID(0x3F56FCD851564C84ULL, 0x65B827D0530384CFULL,
                            NRMessageInterface)

