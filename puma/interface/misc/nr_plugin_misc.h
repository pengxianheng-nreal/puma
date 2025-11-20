#pragma once

#include "nr_plugin_interface.h"
#include "nr_plugin_types.h"
#include "nr_plugin_glasses_types.h"
typedef struct NRMiscProvider {

    NRPluginResult(NR_INTERFACE_API *SetGlassesNetLogEnable)(
        NRPluginHandle handle,
        NREnableValue enable_value,
        const char * ip,
        uint32_t ip_size,
        uint32_t port
    );

    NRPluginResult(NR_INTERFACE_API *GetSystemStatus)(
        NRPluginHandle handle,
        const char ** sub_system_key,
        uint32_t * sub_system_key_size,
        const char ** sub_system_status,
        uint32_t * sub_system_status_size
    );
} NRMiscProvider;

NR_DECLARE_INTERFACE(NRMiscInterface) {

    NRPluginResult(NR_INTERFACE_API *RegisterProvider)(
        NRPluginHandle handle,
        const NRMiscProvider * provider,
        uint32_t provider_size
    );
};

NR_REGISTER_INTERFACE_GUID(0x470980265202F1B0ULL, 0x4F0DF08E9129CE0EULL,
                            NRMiscInterface)

