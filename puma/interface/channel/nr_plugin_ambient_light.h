#pragma once

#include "nr_plugin_lifecycle.h"
#include "nr_plugin_ambient_light_types.h"
#include "nr_plugin_glasses_types.h"
NR_DECLARE_INTERFACE(NRAmbientLightInterface) {

    NRPluginResult(NR_INTERFACE_API *RegisterLifecycleProvider)(
        const char * plugin_id,
        const char * plugin_version,
        const NRPluginLifecycleProvider * provider,
        uint32_t provider_size
    );

    NRPluginResult(NR_INTERFACE_API *NotifyData)(
        NRPluginHandle handle,
        NRChannelDataType channel_data_type,
        const NRAmbientLightData * data,
        uint32_t data_size
    );
};

NR_REGISTER_INTERFACE_GUID(0xBC82CD49754F4C4EULL, 0xB1BA31259ACDEB37ULL,
                            NRAmbientLightInterface)

