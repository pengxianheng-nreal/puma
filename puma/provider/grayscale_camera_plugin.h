#pragma once
#include <framework/util/singleton.h>
#include <puma/interface/public/nr_plugin_interface.h>
#include <puma/interface/public/nr_plugin_types.h>
#include <puma/interface/common/nr_plugin_generic.h>
#include <puma/interface/channel/nr_plugin_grayscale_camera.h>
#include <puma/provider/base_plugin.h>

#include <puma/driver/camera/camera_factory.h>


using GrayscaleCameraFrameCallback = std::function<void(const NRGrayscaleCameraFrameData * data, uint32_t data_size)>;

namespace puma::plugin {

class GrayscaleCameraPlugin :
        public BasePlugin<NRGrayscaleCameraInterface>,
        public framework::util::Singleton<GrayscaleCameraPlugin> {
public:
    GrayscaleCameraPlugin();
    ~GrayscaleCameraPlugin() override = default;

    bool InitInterface(NRInterfaces* interfaces) override;

    /// interface
    NRPluginResult Register( NRPluginHandle handle ) override;
    NRPluginResult Unregister( NRPluginHandle handle ) override;
    NRPluginResult Initialize( NRPluginHandle handle ) override;
    NRPluginResult Start( NRPluginHandle handle ) override;
    NRPluginResult Pause( NRPluginHandle handle ) override;
    NRPluginResult Resume( NRPluginHandle handle ) override;
    NRPluginResult Stop( NRPluginHandle handle ) override;
    NRPluginResult Release( NRPluginHandle handle ) override;
public:
    bool ParseGlobalConfig(const char *config, uint32_t size) override;

    void SetFrameCallback(GrayscaleCameraFrameCallback callback) {
        frame_callback_ = callback;
    }

public:
    int32_t StartCamera();
    int32_t StopCamera();
protected:
    int32_t StartGrayscaleCamera();
    int32_t StopGrayscaleCamera();

    RingBufferRW<DataBuffer<>> * GetFrameBuffer() {
        return &frame_buffer_;
    }

private:
    NRPluginResult OnStart( NRPluginHandle handle ){ return NRPluginResult::NR_PLUGIN_RESULT_SUCCESS;}
    NRPluginResult OnStop( NRPluginHandle handle ){ return NRPluginResult::NR_PLUGIN_RESULT_SUCCESS;}

private:
    std::mutex mutex_{};

    int32_t start_counter_{0};
    std::atomic<uint64_t> submit_counter_{0};
    std::string dump_path_{""};

    RingBufferRWLocker<DataBuffer<>> frame_buffer_;

    std::shared_ptr<CameraInfo> camera_info_;
    std::shared_ptr<CameraDriver> driver_;
    CameraFactory::CameraType camera_type_{CameraFactory::CameraType::CAMERA_UVC};

    GrayscaleCameraFrameCallback frame_callback_;

protected:
    void process(const GenericShmWrapper&);

};

    
} //  namespace puma::plugin
