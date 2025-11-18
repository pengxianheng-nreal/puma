#pragma once

#include "nr_plugin_lifecycle.h"

typedef struct NRPumaProvider {
    NRPluginResult(NR_INTERFACE_API * GetVariable)(uint32_t* out_variable);

    NRPluginResult(NR_INTERFACE_API * UpdateVarible)(NRPluginHandle handle, uint32_t in_varible);
} NRPumaProvider;

NR_DECLARE_INTERFACE(NRPumaInterface) {
    NRPluginResult(NR_INTERFACE_API * RegisterLifecycleProvider)(
        const char* plugin_id, const char* plugin_version,
        const NRPluginLifecycleProvider* provider, uint32_t provider_size);

    NRPluginResult(NR_INTERFACE_API * RegisterProvider)(
        NRPluginHandle handle, const NRPumaProvider *provider, uint32_t provider_size);

    NRPluginResult(NR_INTERFACE_API * GetActivityInfoWithLoader)(
            NRPluginHandle handle, void** java_vm, void** context, void** extra_class_loader);

    NRPluginResult(NR_INTERFACE_API * SetVariable)(
            NRPluginHandle handle, uint32_t variable);
};

NR_REGISTER_INTERFACE_GUID(0x0298DA17FFF944B2ULL, 0xBB0B08DEB52096CDULL,
                           NRPumaInterface)
