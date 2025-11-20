#include <puma/env//export.h>
#include <puma/interface/public/nr_plugin_interface.h>
#include <framework/util/log.h>

void ShutdownLog()
{
#ifdef PUMA_SHARED_LIBS
    framework::util::log::Logger::shutdown();
#endif
}

#ifdef PUMA_SHARED_LIBS
PUMA_EXTERN_C_BEGIN
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
PUMA_EXTERN_C_END
#endif
