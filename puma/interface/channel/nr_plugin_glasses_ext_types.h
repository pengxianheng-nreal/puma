#pragma once

#include "nr_plugin_types.h"

#include "nr_plugin_android_xr_types.h"
#ifdef NRAPP

#include "nr_plugin_glasses_ext_types.inc"

#else


NR_PLUGIN_ENUM32(NRControlTransport) {
    CONTROL_TRANSPORT_UNDEFINED = 0,
    CONTROL_TRANSPORT_TCP_IP = 1,
    CONTROL_TRANSPORT_USES_PROTOCOL = 2,
};


NR_PLUGIN_ENUM32(NRImuTransport) {
    IMU_TRANSPORT_UNDEFINED = 0,
    IMU_TRANSPORT_TCP_IP = 1,
    IMU_TRANSPORT_USES_PROTOCOL = 2,
};

#pragma pack(1)

typedef struct NRDataTransportParams {
    union {
        struct {
            NRControlTransport control_transport_mode;
            NRAxrAudioAudioTransport audio_transport_playback_mode;
            NRAxrAudioAudioTransport audio_transport_capture_mode;
            NRAxrCompositionHeadTrackingTransport head_tracking_transport_mode;
            NRAxrCameraCameraTransport grayscale_camera_transport_mode;
            NRAxrCameraCameraTransport rgb_camera_transport_mode;
            NRImuTransport imu_transport_mode;
            NREnableValue normal_tcp_enable;
        };
        uint8_t padding[128];
    };

} NRDataTransportParams;

#pragma pack()

#endif // NRAPP