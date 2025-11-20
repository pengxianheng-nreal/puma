#pragma once
#include <puma/env/system_config.h>

#if defined(PUMA_SYSTEM_LINUX) || defined(PUMA_SYSTEM_ANDROID)
#include <puma/driver/camera/camera_driver.h>
#include <libuvc/libuvc.h>

enum UVC_CAM_PROC_STATUS {
    UVC_CAM_PROC_NONE = 0,
    UVC_CAM_PROC_INIT,
    UVC_CAM_PROC_FIND_DEV,
    UVC_CAM_PROC_OPEN,
    UVC_CAM_PROC_START_STREAM,
    UVC_CAM_PROC_STOP_STREAM,
    UVC_CAM_PROC_CLOSE,
    UVC_CAM_PROC_UNREF,
    UVC_CAM_PROC_EXIT,
};

class CameraUvc : public CameraDriver {
public:
    CameraUvc(std::function<void (const GenericShmWrapper&)> _callback);

    virtual ~CameraUvc();

    virtual bool init() override;

    virtual bool start() override;

    virtual void stop() override;

    virtual void setAutoExposure(int _auto_exposure) override;

    virtual void setExposureTime(double _exposure_time) override;

    virtual void setGain(int _gain) override;

    virtual bool setVideoProperties(uint32_t prop, uint8_t value) override;

    virtual bool getVideoProperties(uint32_t prop, uint8_t& value) override;

    virtual bool setVideoProperties(uint32_t , const std::vector<uint8_t>&) override;

    virtual bool getVideoProperties(uint32_t , std::vector<uint8_t>& ) override;

    static void uvc_callback(uvc_frame_t *frame, void *ptr);

private:
    bool usbSetGain(const uint8_t _gain);
    bool usbInit();

    bool initGainRange();

    int uvcOpen();
    int uvcStartStream();
    void GetSupportFrameInfo(const uvc_format_desc_t *format_descs);
protected:
    uvc_device_t *dev_{NULL};

    uvc_context_t *contex_;

    uvc_device_handle_t *dev_handle_;

    uvc_stream_ctrl_t ctrl_;

    UVC_CAM_PROC_STATUS cam_proc_status_{UVC_CAM_PROC_NONE};

    int uvc_is_callback_{0};


    const uint32_t OV_MAX_EXPOSURE_ADDR{0x8018FFF4};
    const uint32_t OV_LUMA_ADDR{0x8018FFE1};
    const uint32_t OV_ISP0_GAMMA_ENABLE_ADDR{0x80181000};
    const uint32_t OV_ISP1_GAMMA_ENABLE_ADDR{0x80181800};

    const uint32_t OV_ISP0_GAMMA_ADDR{0x80181341};
    const uint32_t OV_ISP1_GAMMA_ADDR{0x80181B41};
    const uint32_t OV_ISP_GAMMA_COEFFS_NUM{15};

    const uint32_t OV_TARGET_BRIGHTNESS_ADDR{0x8018FFE1};
};

#endif
