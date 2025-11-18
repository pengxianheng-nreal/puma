#include <puma/env/system_config.h>
#include <puma/env/export.h>
#include <puma/env/version.h>

#include <framework/util/android_globals.h>
#include <framework/util/util.h>
#include <framework/util/plugin_util.h>

#include <puma/interface/common/nr_plugin_generic.h>
#include <puma/interface/puma/nr_plugin_puma.h>
#include <puma/source/log.h>

using namespace framework::util;

void ShutdownLog();

/// interface
static NRPluginResult PumaRegister( NRPluginHandle handle );
static NRPluginResult PumaUnregister( NRPluginHandle handle );
static NRPluginResult PumaInitialize( NRPluginHandle handle );
static NRPluginResult PumaStart( NRPluginHandle handle );
static NRPluginResult PumaPause( NRPluginHandle handle );
static NRPluginResult PumaResume( NRPluginHandle handle );
static NRPluginResult PumaStop( NRPluginHandle handle );
static NRPluginResult PumaRelease( NRPluginHandle handle );

static NRPluginResult GetVariable( uint32_t* out_variable );
static NRPluginResult UpdateVariable( NRPluginHandle handle, uint32_t in_varible );

/// global variables
static NRPluginHandle s_handle;
static std::unique_ptr<NRGenericInterface> s_generic_interface = nullptr;
static std::unique_ptr<NRPumaInterface> s_interface = nullptr;
static uint32_t s_Puma_variable;

EXAMPLE_EXTERN_C_BEGIN
void NRPluginLoad_Puma(NRInterfaces* interfaces)
{
	s_generic_interface = framework::util::GetInterface<NRGenericInterface>(interfaces);
    s_interface = framework::util::GetInterface<NRPumaInterface>(interfaces);
    NRPluginLifecycleProvider provider = 
    {
        &PumaRegister,
        &PumaInitialize,
        &PumaStart,
        nullptr,
        &PumaPause,
        &PumaResume,
        &PumaStop,
        &PumaRelease,
        &PumaUnregister,
    };

    s_interface->RegisterLifecycleProvider("nr_puma_id", EXAMPLE_VERSION_STRING, &provider, sizeof(provider));
}

void NRPluginUnload_Puma()
{
}
EXAMPLE_EXTERN_C_END

/*
 * All log function must be execute after this function has been called.
 * Because android log require android environment.
 */
void InitAndroidEnvironment() {
#ifdef EXAMPLE_SYSTEM_ANDROID
	_JavaVM* java_vm = nullptr;
	void* obj_activity = nullptr;
	void* extra_class_loader = nullptr;
    s_generic_interface->GetActivityInfo(
                                    reinterpret_cast<void**>(&java_vm),
                                    reinterpret_cast<void**>(&obj_activity),
                                    reinterpret_cast<void**>(&extra_class_loader));

    if(framework::util::android::GetJavaVM() == nullptr){
        framework::util::android::StoreJavaVM(java_vm);
    }

    if(framework::util::android::GetActivity() == nullptr){
        framework::util::android::StoreActivity(obj_activity);
    }

    if(framework::util::android::GetExtraClassLoader() == nullptr) {
        framework::util::android::StoreExtraClassLoader(extra_class_loader);
    }
#endif
}

NRPluginResult PumaRegister( NRPluginHandle handle ) {
	InitAndroidEnvironment();
	// Pay attention: Do not log anything in this function. 
	// If Use log, must use framework::util::Logger::shutdown in PumaUnregister function. 
	// Because log has alloc thread resource, and should be recovered at right place.
    s_handle = handle;

    NRPumaProvider puma_provider =
    {
        &GetVariable,
        &UpdateVariable
    };

    NRPluginResult retcode = s_interface->RegisterProvider(s_handle, &puma_provider, sizeof(puma_provider));
    return retcode;
}
NRPluginResult PumaUnregister( NRPluginHandle handle ) {
    UNUSED(handle);
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult PumaInitialize( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma Initializing.");
    UNUSED(handle);

    PUMA_LOG_INFO("Puma Initialized.");
	return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult PumaStart( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma Starting.");
    UNUSED(handle);
    PUMA_LOG_INFO("Puma Started.");
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult PumaPause( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma Pausing.");
    UNUSED(handle);
    PUMA_LOG_INFO("Puma Paused.");
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult PumaResume( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma Resuming.");
    UNUSED(handle);
    PUMA_LOG_INFO("Puma Resumed.");
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult PumaStop( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma Stopping.");
    UNUSED(handle);
    PUMA_LOG_INFO("Puma Stopped.");
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult PumaRelease( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma Releasing.");
    UNUSED(handle);
    PUMA_LOG_INFO("Puma Released.")
	ShutdownLog();
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult GetVariable( uint32_t* out_variable ) {
    *out_variable = s_Puma_variable;
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult UpdateVariable( NRPluginHandle handle, uint32_t in_varible ) {
    s_Puma_variable = in_varible;
    if (s_interface && s_interface->SetVariable) {
        s_interface->SetVariable(handle, s_Puma_variable);
    } else {
        PUMA_LOG_ERROR("Invalid Interface: The interface does not exist or it does not have a method to set the variable.");
        return NR_PLUGIN_RESULT_UNSUPPORTED;
    }
    return NR_PLUGIN_RESULT_SUCCESS;
}
