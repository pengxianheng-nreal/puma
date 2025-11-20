#include <puma/provider/grayscale_camera_plugin.h>
#include <puma/env/version.h>
#include <puma/env/export.h>
#include <puma/util/util.h>
#include <puma/util/types.h>

//CV:    ./uvctest -d /dev/video4 -s 640x513 -r 120 -v 1 -f GREY
//RGB:  ./uvctest -d /dev/video2 -s 1920x1080 -r 30 -f HEVC
//      ./uvctest -d /dev/video2 -s 2016x1512 -r 30 -f HEVC   

using namespace puma::plugin;

PUMA_EXTERN_C_BEGIN
void NRPluginLoad_Puma_GrayscaleCamera(NRInterfaces* interfaces)
{
    GrayscaleCameraPlugin::GetInstance()->InitInterface(interfaces);
}

void NRPluginUnload_Puma_GrayscaleCamera()
{
}
PUMA_EXTERN_C_END

NRPluginResult GrayscaleCameraRegister( NRPluginHandle handle ) {
    return GrayscaleCameraPlugin::GetInstance()->Register(handle);
}

NRPluginResult GrayscaleCameraUnregister( NRPluginHandle handle ) {
    return GrayscaleCameraPlugin::GetInstance()->Unregister(handle);
}

NRPluginResult GrayscaleCameraInitialize( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma GrayscaleCamera Initializing.");
    NRPluginResult retcode = GrayscaleCameraPlugin::GetInstance()->Initialize(handle);
    PUMA_LOG_INFO("Puma GrayscaleCamera Initialized. {}", retcode);
	return retcode;
}

NRPluginResult GrayscaleCameraStart( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma GrayscaleCamera Starting.");
    NRPluginResult retcode = GrayscaleCameraPlugin::GetInstance()->Start(handle);
    PUMA_LOG_INFO("Puma GrayscaleCamera Started. {}", retcode);
    return retcode;
}

NRPluginResult GrayscaleCameraPause( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma GrayscaleCamera Pausing.");
    NRPluginResult retcode = GrayscaleCameraPlugin::GetInstance()->Pause(handle);
    PUMA_LOG_INFO("Puma GrayscaleCamera Paused. {}", retcode);
    return retcode;
}

NRPluginResult GrayscaleCameraResume( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma GrayscaleCamera Resuming.");
    NRPluginResult retcode = GrayscaleCameraPlugin::GetInstance()->Resume(handle);
    PUMA_LOG_INFO("Puma GrayscaleCamera Resumed. {}", retcode);
    return retcode;
}

NRPluginResult GrayscaleCameraStop( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma GrayscaleCamera Stopping.");
    NRPluginResult retcode = GrayscaleCameraPlugin::GetInstance()->Stop(handle);
    PUMA_LOG_INFO("Puma GrayscaleCamera Stopped. {}", retcode);
    return retcode;
}

NRPluginResult GrayscaleCameraRelease( NRPluginHandle handle )
{
    PUMA_LOG_INFO("Puma GrayscaleCamera Releasing.");
    NRPluginResult retcode = GrayscaleCameraPlugin::GetInstance()->Release(handle);
    PUMA_LOG_INFO("Puma GrayscaleCamera Released. {}", retcode);
    return retcode;
}

GrayscaleCameraPlugin::GrayscaleCameraPlugin() : BasePlugin(), frame_buffer_(kFrameBufferSize){
    camera_info_ = std::make_shared<CameraInfo>();
    camera_info_->usb_fd = -1;
    camera_info_->height_from_config = false; 

    camera_info_->width = 640;
    camera_info_->height = 513;
    camera_info_->channels = 1;
    camera_info_->uvc_fmt = 0;
    camera_info_->frame_rate = 120;
    camera_info_->height_from_config = false;
}
bool GrayscaleCameraPlugin::InitInterface(NRInterfaces* interfaces) {
    if (!BasePlugin<NRGrayscaleCameraInterface>::InitInterface(interfaces)) {
        return false;
    }

    NRPluginLifecycleProvider provider =
    {
        &GrayscaleCameraRegister,
        &GrayscaleCameraInitialize,
        &GrayscaleCameraStart,
        nullptr,
        &GrayscaleCameraPause,
        &GrayscaleCameraResume,
        &GrayscaleCameraStop,
        &GrayscaleCameraRelease,
        &GrayscaleCameraUnregister,
    };

    if (interface_) {
        interface_->RegisterLifecycleProvider("nr_puma_grayscale_camera_id", PUMA_VERSION_STRING, &provider, sizeof(provider));
    }

    return true;
}

NRPluginResult GrayscaleCameraPlugin::Register( NRPluginHandle handle ) {
    InitAndroidEnvironment();
    handle_ = handle;
    NRPluginResult retcode = interface_->RegisterProvider(handle_, reinterpret_cast<const NRGrayscaleCameraProvider*>(providers_.data()), providers_.size() * sizeof(void*));

    if (generic_interface_->GetGlobalConfig){
        const char *global_config;
        uint32_t global_config_size = 0;
        NRPluginResult ret = generic_interface_->GetGlobalConfig(handle_, &global_config, &global_config_size);
        ParseGlobalConfig(global_config, global_config_size);
    }
    return retcode;
}

NRPluginResult GrayscaleCameraPlugin::Unregister( NRPluginHandle handle ) {
    UNUSED(handle);
    return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult GrayscaleCameraPlugin::Initialize( NRPluginHandle handle )
{
    if(handle_ != handle) {
        PUMA_LOG_ERROR("GrayscaleCamera Initialize handle");
        return NR_PLUGIN_RESULT_FAILURE;
    }

	return NR_PLUGIN_RESULT_SUCCESS;
}

NRPluginResult GrayscaleCameraPlugin::Start( NRPluginHandle handle )
{
    if(handle_ != handle) {
        PUMA_LOG_ERROR("GrayscaleCamera Start handle");
        return NR_PLUGIN_RESULT_FAILURE;
    }

    return OnStart(handle);
}

NRPluginResult GrayscaleCameraPlugin::Stop( NRPluginHandle handle )
{
    if(handle_ != handle) {
        PUMA_LOG_ERROR("GrayscaleCamera Stop handle");
        return NR_PLUGIN_RESULT_FAILURE;
    }

    return OnStop(handle);
}

NRPluginResult GrayscaleCameraPlugin::Pause( NRPluginHandle handle )
{
    if(handle_ != handle) {
        PUMA_LOG_ERROR("GrayscaleCamera Pause handle");
        return NR_PLUGIN_RESULT_FAILURE;
    }

    return OnStop(handle);
}

NRPluginResult GrayscaleCameraPlugin::Resume( NRPluginHandle handle )
{
    if(handle_ != handle) {
        PUMA_LOG_ERROR("GrayscaleCamera Resume handle");
        return NR_PLUGIN_RESULT_FAILURE;
    }

    return OnStart(handle);
}

NRPluginResult GrayscaleCameraPlugin::Release( NRPluginHandle handle )
{
    if(handle_ != handle) {
        PUMA_LOG_ERROR("GrayscaleCamera Release handle");
        return NR_PLUGIN_RESULT_FAILURE;
    }

    return NR_PLUGIN_RESULT_SUCCESS;
}
bool GrayscaleCameraPlugin::ParseGlobalConfig(const char *config, uint32_t size) {
    if (size == 0) {
        return true;
    }
    Json::Value root;
    Json::CharReaderBuilder json_builder;
    json_builder["collectComments"] = false;
    JSONCPP_STRING json_errs;
    std::istringstream json_stream(std::string(config, size));
    if (!parseFromStream(json_builder, json_stream, &root, &json_errs)) {
        PUMA_LOG_ERROR("Parse global config sdk_global error, json_errs = {}", json_errs.c_str());
        return false;
    }
    if (!root.isMember("driver")) {
        return true;
    }
    return true;
}

int32_t GrayscaleCameraPlugin::StartCamera() {
    return StartGrayscaleCamera();
}

int32_t GrayscaleCameraPlugin::StopCamera() {
    dump_path_ = "";
    return StopGrayscaleCamera();
}



int32_t GrayscaleCameraPlugin::StartGrayscaleCamera() {
    std::lock_guard<std::mutex> guard(mutex_);
    if(++start_counter_ > 1) {
        return NR_PLUGIN_RESULT_SUCCESS;
    }

#if defined(PUMA_SYSTEM_ANDROID) || defined(PUMA_SYSTEM_LINUX)
    camera_type_ = CameraFactory::CameraType::CAMERA_V4L2;
    //camera_type_ = CameraFactory::CameraType::CAMERA_UVC;
#elif defined(PUMA_SYSTEM_MACOS)
    camera_type_ = CameraFactory::CameraType::CAMERA_MAC;
#else
    camera_type_ = CameraFactory::CameraType::CAMERA_DSHOW;
#endif

    driver_ = CameraFactory::createDevice(camera_type_, std::bind(&GrayscaleCameraPlugin::process, this, std::placeholders::_1));
    if(driver_ == nullptr) {
        PUMA_LOG_ERROR("StartGrayscaleCamera createDevice failed!");
        return  NR_PLUGIN_RESULT_FAILURE;
    }

    driver_->SetDeviceType(DEVICE_TYPE_HYLLA_M);
    std::vector<std::string> gray_camera_name;
    gray_camera_name.emplace_back("UVC Camera 1"); //gray camera name
    driver_->addProductName(gray_camera_name);
    //driver_->addProductID(0x3318, 0x043a); //hylla
    driver_->setCameraProperties(*camera_info_);
    driver_->setIOMethod(CameraDriver::IO_Method::IO_METHOD_MMAP);

    if (!driver_->init()) {
        PUMA_LOG_ERROR("GrayscaleCameraPlugin driver init failed");
        return  NR_PLUGIN_RESULT_FAILURE;
    }

    if (driver_->start()) {
        return NR_PLUGIN_RESULT_SUCCESS;
    }
    return  NR_PLUGIN_RESULT_FAILURE;
}

int32_t GrayscaleCameraPlugin::StopGrayscaleCamera() {
    std::lock_guard<std::mutex> guard(mutex_);
    if(--start_counter_ >= 1) {
        return NR_PLUGIN_RESULT_SUCCESS;
    }

    if (driver_) {
        driver_->stop();
    }
    return NR_PLUGIN_RESULT_SUCCESS;
}

void GrayscaleCameraPlugin::process(const GenericShmWrapper& frame){
    frame_t * ptr = (frame_t *)frame.reserved;

    uint32_t uvc_width = ptr->width;
    uint32_t uvc_height = ptr->height;
    uint32_t uvc_camera_count = ptr->bytes_per_pixel;
    uint32_t data_bytes = ptr->data_bytes;
    PUMA_LOG_TRACE("GrayscaleCameraPlugin::process uvc metadata: {}x{}x{}, data_bytes:{}", uvc_width, uvc_height, uvc_camera_count, data_bytes);

    UNIVERSAL_META_DATA* meta_data = (UNIVERSAL_META_DATA*)(ptr->data + uvc_width*(uvc_height-1)*uvc_camera_count);
    PUMA_LOG_TRACE("GrayscaleCameraPlugin::process meta_data: {}x{} sensor_index:{},frame_id:{},timestamp:{}, stride:{}", 
        meta_data->width, meta_data->height, meta_data->sensor_index, meta_data->frame_id, meta_data->timestamp, meta_data->stride);

    uint64_t sys_time = framework::util::FMonotonicGetNs();

    NRGrayscaleCameraFrameData nrframe;
    nrframe.data = ptr->data;
    nrframe.data_bytes = meta_data->width*meta_data->height*uvc_camera_count;
    nrframe.camera_count = uvc_camera_count;
    nrframe.frame_id = meta_data->frame_id;
    //nrframe.hmd_hw_time_nanos = meta_data->timestamp_system + meta_data->exposure_time_ns/2;

    for (int32_t i = 0; i < nrframe.camera_count; ++i) {
        nrframe.cameras[i].offset = 0;
        nrframe.cameras[i].camera_id =  meta_data->sensor_index==0? NR_GRAYSCALE_CAMERA_ID_0:NR_GRAYSCALE_CAMERA_ID_1;
        nrframe.cameras[i].width = meta_data->width;
        nrframe.cameras[i].height = meta_data->height;
        nrframe.cameras[i].stride = meta_data->stride;
        nrframe.cameras[i].exposure_start_time_device = meta_data->timestamp_system;
        nrframe.cameras[i].exposure_duration = meta_data->exposure_time_ns;
        nrframe.cameras[i].rolling_shutter_time = meta_data->rolling_shutter;
        nrframe.cameras[i].gain = meta_data->gain_value;
        nrframe.cameras[i].exposure_start_time_system = sys_time - meta_data->exposure_time_ns;
    }

    if(frame_callback_){
        frame_callback_(&nrframe,sizeof(nrframe));
    }

    PUMA_LOG_INFO("GrayscaleCameraPlugin::process");
}

