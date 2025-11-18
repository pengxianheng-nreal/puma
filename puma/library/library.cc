#include <puma/env/export.h>
#include <puma/interface/public/nr_plugin_interface.h>
#include <framework/util/log.h>

void ShutdownLog()
{
#ifdef EXAMPLE_SHARED_LIBS
    framework::util::log::Logger::shutdown();
#endif
}

#ifdef EXAMPLE_SHARED_LIBS
EXAMPLE_EXTERN_C_BEGIN
void NRPluginLoad_Puma(NRInterfaces* interfaces);
void NRPluginUnload_Puma();

void NR_INTERFACE_EXPORT NR_INTERFACE_API
NRPluginLoad(NRInterfaces* interfaces) {
	NRPluginLoad_Puma(interfaces);
}
void NR_INTERFACE_EXPORT NR_INTERFACE_API
NRPluginUnload() {
	NRPluginUnload_Puma();
}
EXAMPLE_EXTERN_C_END
#endif
