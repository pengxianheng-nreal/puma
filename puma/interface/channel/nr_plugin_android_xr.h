#pragma once

#include "nr_plugin_interface.h"
#include "nr_plugin_android_xr_types.h"
#include "nr_plugin_glasses_types.h"
typedef struct NRAxrProvider {

    NRPluginResult(NR_INTERFACE_API *GetLogCallback)(
        NRPluginHandle handle,
        void ** callback
    );

    NRPluginResult(NR_INTERFACE_API *NotifyDeviceStateValue)(
        NRPluginHandle handle,
        const NRAxrDeviceStateValue * value
    );

    NRPluginResult(NR_INTERFACE_API *NotifySensorSample)(
        NRPluginHandle handle,
        const NRAxrSensorSensorSample * sample,
        uint32_t sample_size
    );

    NRPluginResult(NR_INTERFACE_API *ResetImuDataRequest)(
        NRPluginHandle handle,
        const NRAxrImuDataRequestInfo * request_info
    );
} NRAxrProvider;

NR_DECLARE_INTERFACE(NRAxrInterface) {

    NRPluginResult(NR_INTERFACE_API *RegisterProvider)(
        NRPluginHandle handle,
        const NRAxrProvider * provider,
        uint32_t provider_size
    );

    NRPluginResult(NR_INTERFACE_API *SubmitAxrStatus)(
        NRPluginHandle handle,
        NREnableValue status
    );

    NRPluginResult(NR_INTERFACE_API *AxrBlobReceiveBlob)(
        NRPluginHandle handle,
        const NRAxrBlobBlobId * id,
        NRAxrBlobBlobInfo * out_blob_info,
        const char ** out_payload_data,
        uint32_t* out_payload_size
    );

    NRPluginResult(NR_INTERFACE_API *AxrBlobDeleteBlobs)(
        NRPluginHandle handle,
        const NRAxrBlobBlobId * ids_data,
        uint32_t ids_size
    );

    NRPluginResult(NR_INTERFACE_API *AxrDeviceStateGetCapabilities)(
        NRPluginHandle handle,
        NRAxrDeviceStateKeyMetadataMapEntry * out_keys_data,
        uint32_t * in_out_keys_size
    );

    NRPluginResult(NR_INTERFACE_API *AxrDeviceStateObserveState)(
        NRPluginHandle handle
    );

    NRPluginResult(NR_INTERFACE_API *AxrDisplayGetCapabilities)(
        NRPluginHandle handle,
        NRAxrDisplayDisplayProfile * out_display_profiles_data,
        uint32_t * in_out_display_profiles_size,
        bool * out_adjustable_brightness,
        bool * out_auto_brightness,
        NRAxrDisplayDimmingSupport * out_dimming_support
    );

    NRPluginResult(NR_INTERFACE_API *AxrDisplaySetBrightness)(
        NRPluginHandle handle,
        uint32_t brightness
    );

    NRPluginResult(NR_INTERFACE_API *AxrDisplaySetAutoBrightnessMode)(
        NRPluginHandle handle,
        bool is_enabled
    );

    NRPluginResult(NR_INTERFACE_API *AxrLoggingGetCapabilities)(
        NRPluginHandle handle,
        int32_t * out_supported_log_severities_data,
        uint32_t * in_out_supported_log_severities_size
    );

    NRPluginResult(NR_INTERFACE_API *AxrAudioGetCapabilities)(
        NRPluginHandle handle,
        NRAxrAudioAudioTransport * out_audio_transport_playback,
        NRAxrAudioAudioTransport * out_audio_transport_capture,
        NRAxrAudioMicCapability * out_mic_capabilities,
        NRAxrAudioSpeakerCapability * out_speaker_capabilities
    );

    NRPluginResult(NR_INTERFACE_API *AxrCameraGetCapabilities)(
        NRPluginHandle handle,
        NRAxrCameraCameraTransport * out_camera_transport
    );

    NRPluginResult(NR_INTERFACE_API *AxrCompositionGetCapabilities)(
        NRPluginHandle handle,
        NRAxrCompositionLSR * out_lsr,
        NRAxrCompositionLensCorrection * out_lens_correction,
        NRAxrCompositionHeadTracking * out_head_tracking_data,
        uint32_t * in_out_head_tracking_size,
        NRAxrCompositionLSRLocation * out_lsr_location,
        bool * out_spatial_reprojection_supported,
        bool * out_eye_tracking_supported
    );

    NRPluginResult(NR_INTERFACE_API *AxrSensorGetCapabilities)(
        NRPluginHandle handle,
        NRAxrSensorSensorCapability * out_sensors_data,
        uint32_t * in_out_sensors_size
    );

    NRPluginResult(NR_INTERFACE_API *AxrSensorReceiveSensorStream)(
        NRPluginHandle handle,
        const NRAxrSensorSensorStreamRequestParametersMapEntry * sensors_data,
        uint32_t sensors_size
    );

    NRPluginResult(NR_INTERFACE_API *AxrCalibrationGetCalibration)(
        NRPluginHandle handle,
        NRAxrBlobBlobInfo * out_device_profile
    );

    NRPluginResult(NR_INTERFACE_API *AxrAudioSetAudioConfig)(
        NRPluginHandle handle,
        const NRAxrAudioMicConfig * mic_config,
        const NRAxrAudioSpeakerConfig * speaker_config
    );

    NRPluginResult(NR_INTERFACE_API *AxrDisplaySetDimmingLevel)(
        NRPluginHandle handle,
        int32_t level
    );

    NRPluginResult(NR_INTERFACE_API *AxrDisplaySetPowerMode)(
        NRPluginHandle handle,
        NRAxrDisplayPowerMode power_mode
    );

    NRPluginResult(NR_INTERFACE_API *AxrLoggingGetStatusReport)(
        NRPluginHandle handle,
        NRAxrLoggingStatusReport * out_reports_data,
        uint32_t * in_out_reports_size
    );

    NRPluginResult(NR_INTERFACE_API *AxrBlobStreamBlob)(
        NRPluginHandle handle,
        const NRAxrBlobBlobId * id,
        int32_t max_chunk_size,
        NRAxrBlobBlobInfo * out_blob_info,
        int64_t * out_offset,
        const char ** out_chunk_data,
        uint32_t* out_chunk_size
    );

    NRPluginResult(NR_INTERFACE_API *AxrLoggingGetLogFiles)(
        NRPluginHandle handle,
        NRAxrBlobBlobInfo * out_log_files_data,
        uint32_t * in_out_log_files_size
    );

    NRPluginResult(NR_INTERFACE_API *AxrCompositionGetHeadsetConfig)(
        NRPluginHandle handle,
        NRAxrBlobBlobInfo * out_headset_configuration
    );

    NRPluginResult(NR_INTERFACE_API *AxrSettingsSetPrivacyLed)(
        NRPluginHandle handle,
        bool enable
    );

    NRPluginResult(NR_INTERFACE_API *AxrSettingsSetDevicePowerStatus)(
        NRPluginHandle handle,
        NRAxrSettingsPowerStatus power_status
    );

    NRPluginResult(NR_INTERFACE_API *AxrCrashReportWatchCrashReports)(
        NRPluginHandle handle,
        int64_t * out_timestamp_ms,
        NRAxrBlobBlobInfo * out_blobs_data,
        uint32_t * in_out_blobs_size
    );
};

NR_REGISTER_INTERFACE_GUID(0xC693ABBDC84348AFULL, 0xB3B6AFD77B642FEFULL,
                            NRAxrInterface)

