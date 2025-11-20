#pragma once

#include "nr_plugin_interface.h"
#include "nr_plugin_glasses_ext_types.h"
#include "nr_plugin_glasses_types.h"
typedef struct NRGlassesExtProvider {

    NRPluginResult(NR_INTERFACE_API *StartAllServer)(
        NRPluginHandle handle,
        const NRDataTransportParams * data_transport_params
    );

    NRPluginResult(NR_INTERFACE_API *StopAllServer)(
        NRPluginHandle handle
    );
	/*
		
		clock_sync_time = to_device_time - from_device_time 
		
	*/
    NRPluginResult(NR_INTERFACE_API *GetClockSync)(
        NRPluginHandle handle,
        NRClockDeviceType from_device_type,
        NRClockDeviceType to_device_type,
        int64_t * clock_sync_time
    );
    NRPluginResult(NR_INTERFACE_API *SendReport)(
        NRPluginHandle handle,
        const char * data_in_data,
        uint32_t data_in_size,
        char * data_out_data,
        uint32_t * data_out_size
    );
} NRGlassesExtProvider;

NR_DECLARE_INTERFACE(NRGlassesExtInterface) {

    NRPluginResult(NR_INTERFACE_API *RegisterProvider)(
        NRPluginHandle handle,
        const NRGlassesExtProvider * provider,
        uint32_t provider_size
    );
};

NR_REGISTER_INTERFACE_GUID(0xE5FA6D0D9ED4482BULL, 0xBE08ED5ABAC4020BULL,
                            NRGlassesExtInterface)

