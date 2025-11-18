#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest/doctest.h>

#include <framework/util/util.h>

#include <puma/source/log.h>
#include <puma/interface/puma/nr_plugin_Puma.h>
#include <puma/env/export.h>

/// mock NRSDK
static NRInterfaces s_interfaces;
static NRPluginLifecycleProvider s_lifecycle_provider;
static const NRPluginHandle s_handle = 1;
static NRPumaInterface s_Puma_interface;
static NRPumaProvider s_Puma_provider;

EXAMPLE_EXTERN_C_BEGIN
extern void NRPluginLoad_Puma(NRInterfaces* interfaces);
EXAMPLE_EXTERN_C_END

NRPluginResult RegisterLifecycleProvider(
		const char *plugin_name, 
		const char *id, 
		const NRPluginLifecycleProvider *provider,
        uint32_t provider_size) 
{
    UNUSED(plugin_name);
    UNUSED(id);
    CHECK(provider_size == sizeof(NRPluginLifecycleProvider));
    s_lifecycle_provider = *provider;
    std::cout << __func__ << " success" << std::endl;
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult RegisterProvider(
		NRPluginHandle handle, 
		const NRPumaProvider *provider, 
		uint32_t provider_size) {
	CHECK(s_handle == handle);
    CHECK(provider_size == sizeof(NRPumaProvider));
    s_Puma_provider = *provider;
    std::cout << __func__ << " success" << std::endl;
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult SetVariable(NRPluginHandle handle, uint32_t variable) {
	CHECK(s_handle == handle);
    std::cout << __func__ << " = " << variable << std::endl;
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRInterface *GetInterface(NRInterfaceGUID guid, unsigned long long* out_interface_size) {
    UNUSED(guid);
    std::cout << __func__ << " success" << std::endl;
	*out_interface_size = sizeof(s_Puma_interface);
    return &s_Puma_interface;
}

TEST_CASE("Puma Test") {
    /// mock NRSDK
    s_interfaces.GetInterface = GetInterface;
    s_Puma_interface.RegisterLifecycleProvider = RegisterLifecycleProvider;
	s_Puma_interface.RegisterProvider = RegisterProvider;
    s_Puma_interface.SetVariable = SetVariable;

    /// call load function
    NRPluginLoad_Puma(&s_interfaces);

    s_lifecycle_provider.Register(s_handle);
    s_lifecycle_provider.Initialize(s_handle);
    s_lifecycle_provider.Start(s_handle);
    s_lifecycle_provider.Pause(s_handle);
    s_lifecycle_provider.Resume(s_handle);
    s_lifecycle_provider.Stop(s_handle);
    s_lifecycle_provider.Release(s_handle);
    s_lifecycle_provider.Unregister(s_handle);
}
