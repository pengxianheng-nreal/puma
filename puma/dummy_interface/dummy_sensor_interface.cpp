#include "nr_plugin_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

void NRPluginLoad_SENSOR_MCU(NRInterfaces* interfaces){
    (void)interfaces;
}

void NRPluginUnload_SENSOR_MCU() {}

void NRPluginLoad_GLASSES_CONFIG(NRInterfaces* interfaces){
   (void)interfaces;
}

void NRPluginUnload_GLASSES_CONFIG(){}

void NRPluginLoad_SENSOR_IMU (NRInterfaces* interfaces) {
   (void)interfaces;
}

void NRPluginUnload_SENSOR_IMU() {}

void NRPluginLoad_SENSOR_VSYNC(NRInterfaces* interfaces) {(void)interfaces;}

void NRPluginUnload_SENSOR_VSYNC() {}

void NRPluginLoad_SENSOR_CAMERA(NRInterfaces* interfaces){
    (void)interfaces;
}

void NRPluginUnload_SENSOR_CAMERA() {
}

void
#ifdef SENSOR_SHARED_LIBS
    NR_INTERFACE_EXPORT NR_INTERFACE_API
    NRPluginLoad
#else
NRPluginLoad_SENSOR
#endif
    (NRInterfaces* interfaces) {   
    (void)interfaces;
}

void
#ifdef SENSOR_SHARED_LIBS
    NR_INTERFACE_EXPORT NR_INTERFACE_API
    NRPluginUnload
#else
NRPluginUnload_SENSOR
#endif
    () {
}

#ifdef __cplusplus
}
#endif
