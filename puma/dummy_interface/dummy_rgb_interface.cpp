#include "nr_plugin_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

void
#ifdef RGB_SHARED_LIBS
    NR_INTERFACE_EXPORT NR_INTERFACE_API
    NRPluginLoad
#else
NRPluginLoad_RGB_CAMERA
#endif
    (NRInterfaces* interfaces) {
    (void)interfaces;
}

void
#ifdef RGB_SHARED_LIBS
    NR_INTERFACE_EXPORT NR_INTERFACE_API
    NRPluginUnload
#else
NRPluginUnload_RGB_CAMERA
#endif
    () {
}

#ifdef __cplusplus
}
#endif
