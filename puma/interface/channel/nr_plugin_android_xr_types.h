#pragma once

#include "nr_plugin_types.h"

#ifdef NRAPP

#include "nr_plugin_android_xr_types.inc"

#else


NR_PLUGIN_ENUM32(NRAxrAudioAudioTransport) {
    AUDIO_TRANSPORT_UNDEFINED = 0,
    AUDIO_TRANSPORT_USB_AUDIO = 1,
    AUDIO_TRANSPORT_USES_PROTOCOL = 2,
};


NR_PLUGIN_ENUM32(NRAxrAudioSampleFormat) {
    SAMPLE_FORMAT_UNSPECIFIED = 0,
    SAMPLE_FORMAT_PCM_16 = 1,
    SAMPLE_FORMAT_PCM_32 = 2,
    SAMPLE_FORMAT_FLOAT_24 = 3,
    SAMPLE_FORMAT_FLOAT_32 = 4,
};


NR_PLUGIN_ENUM32(NRAxrAudioSampleRate) {
    SAMPLE_RATE_UNSPECIFIED = 0,
    SAMPLE_RATE_16KHZ = 1,
    SAMPLE_RATE_24KHZ = 2,
    SAMPLE_RATE_32KHZ = 3,
    SAMPLE_RATE_44_1KHZ = 4,
    SAMPLE_RATE_48KHZ = 5,
};


NR_PLUGIN_ENUM32(NRAxrBlobTable) {
    TABLE_UNKNOWN = 0,
    TABLE_BUGREPORTS = 3,
    TABLE_LOGS = 4,
    TABLE_CONFIGS = 5,
};


NR_PLUGIN_ENUM32(NRAxrCameraCameraTransport) {
    CAMERA_TRANSPORT_UNDEFINED = 0,
    CAMERA_TRANSPORT_TCP_IP = 1,
    CAMERA_TRANSPORT_USES_PROTOCOL = 2,
};


NR_PLUGIN_ENUM32(NRAxrCompositionLSR) {
    LSR_UNDEFINED = 0,
    LSR_UNSUPPORTED = 1,
    LSR_3DOF_REPROJECTION = 2,
    LSR_6DOF_REPROJECTION = 3,
};


NR_PLUGIN_ENUM32(NRAxrCompositionLSRLocation) {
    LSR_LOCATION_UNDEFINED = 0,
    LSR_LOCATION_HOST = 1,
    LSR_LOCATION_FRAMES = 2,
};


NR_PLUGIN_ENUM32(NRAxrCompositionLensCorrection) {
    LENS_CORRECTION_UNDEFINED = 0,
    LENS_CORRECTION_NOT_NEEDED = 1,
    LENS_CORRECTION_ON_FRAMES_UNSUPPORTED = 2,
    LENS_CORRECTION_ON_FRAMES_SUPPORTED = 3,
};


NR_PLUGIN_ENUM32(NRAxrCompositionType) {
    HEAD_TRACKING_UNDEFINED = 0,
    HEAD_TRACKING_3DOF = 1,
    HEAD_TRACKING_6DOF = 2,
    IMU_ONLY = 3,
};


NR_PLUGIN_ENUM32(NRAxrCompositionHeadTrackingTransport) {
    HEAD_TRACKING_TRANSPORT_UNDEFINED = 0,
    HEAD_TRACKING_TRANSPORT_TCP_IP = 1,
    HEAD_TRACKING_TRANSPORT_USES_PROTOCOL = 2,
};


NR_PLUGIN_ENUM32(NRAxrCompositionHeadTrackingReadMethod) {
    HEAD_TRACKING_READ_UNDEFINED = 0,
    HEAD_TRACKING_READ_PUSH = 1,
    HEAD_TRACKING_READ_PULL = 2,
};


NR_PLUGIN_ENUM32(NRAxrDeviceStateWellKnownKey) {
    WELL_KNOWN_KEY_UNSPECIFIED = 0,
    WELL_KNOWN_KEY_BATTERY_CHARGE = 1,
    WELL_KNOWN_KEY_PRODUCT_INFO = 2,
    WELL_KNOWN_KEY_DISPLAY_BRIGHTNESS = 3,
    WELL_KNOWN_KEY_DISPLAY_DIMMING = 9,
    WELL_KNOWN_KEY_DISPLAY_POWER_MODE = 11,
    WELL_KNOWN_KEY_AUDIO_VOLUME = 4,
    WELL_KNOWN_KEY_AUDIO_MUTE = 5,
    WELL_KNOWN_KEY_DEVICE_WEAR_STATE = 6,
    WELL_KNOWN_KEY_SOFTWARE_INFO = 7,
    WELL_KNOWN_KEY_AUTO_BRIGHTNESS_MODE = 8,
    WELL_KNOWN_KEY_PRIVACY_LED_STATE = 10,
    WELL_KNOWN_KEY_DEVICE_POWER_STATUS = 12,
    WELL_KNOWN_KEY_MIN_DEVICE_SPECIFIC = 100000,
};


NR_PLUGIN_ENUM32(NRAxrDisplayPowerMode) {
    DISPLAY_ENABLED = 0,
    DISPLAY_DISABLED = 1,
};


NR_PLUGIN_ENUM32(NRAxrDisplayDimmingType) {
    DIMMING_UNDEFINED = 0,
    DIMMING_GLOBAL = 1,
};


NR_PLUGIN_ENUM32(NRAxrDisplayColorSpace) {
    COLOR_SPACE_UNSPECIFIED = 0,
    COLOR_SPACE_RGB_888 = 1,
    COLOR_SPACE_YUV_422 = 2,
    COLOR_SPACE_YUV_420 = 3,
};


NR_PLUGIN_ENUM32(NRAxrDisplayDisplayTransport) {
    DISPLAY_TRANSPORT_UNSPECIFIED = 0,
    DISPLAY_PORT_ALT_MODE = 1,
    USB_VIDEO_ENCODED = 2,
};


NR_PLUGIN_ENUM32(NRAxrDisplayFormat) {
    FORMAT_UNSPECIFIED = 0,
    FORMAT_V1_CBC = 1,
};


NR_PLUGIN_ENUM32(NRAxrEnvelopeStatus) {
    UNKNOWN = 0,
    SUCCESS = 1,
    CANCELLED = 2,
    INTERNAL_ERROR = 3,
    INVALID_INPUT = 4,
    PERMISSION_DENIED = 5,
    RESOURCE_EXHAUSTED = 6,
    TIMED_OUT = 7,
    UNAVAILABLE = 8,
    UNIMPLEMENTED = 9,
};


NR_PLUGIN_ENUM32(NRAxrEnvelopeCommand) {
    QUERY = 0,
    ACTIVATE = 1,
};


NR_PLUGIN_ENUM32(NRAxrEnvelopeVersions) {
    UNDEFINED_VERSION = 0,
    PROTOCOL_VERSION = 1,
};


NR_PLUGIN_ENUM32(NRAxrFirmwareStatus) {
    AXR_FIRMWARE_ERROR_UNKNOWN = 0,
    AXR_FIRMWARE_STATUS_SUCCESS = 1,
    AXR_FIRMWARE_ERROR_CHECKSUM_MISMATCH = 2,
    AXR_FIRMWARE_ERROR_FILE_NOT_FOUND = 3,
};


NR_PLUGIN_ENUM32(NRAxrInputAction) {
    ACTION_UNSPECIFIED = 0,
    ACTION_DOWN = 1,
    ACTION_UP = 2,
};


NR_PLUGIN_ENUM32(NRAxrLoggingLogSeverity) {
    LOG_SEVERITY_UNSPECIFIED = 0,
    LOG_SEVERITY_VERBOSE = 1,
    LOG_SEVERITY_DEBUG = 2,
    LOG_SEVERITY_INFO = 3,
    LOG_SEVERITY_WARNING = 4,
    LOG_SEVERITY_ERROR = 5,
    LOG_SEVERITY_FATAL = 6,
};


NR_PLUGIN_ENUM32(NRAxrSensorSensorType) {
    SENSOR_TYPE_NONE = 0,
    ACCELEROMETER = 1,
    MAGNETOMETER = 2,
    GYROSCOPE = 4,
    LIGHT = 5,
    PRESSURE = 6,
    TEMPERATURE = 7,
    PROXIMITY = 8,
    GRAVITY = 9,
    ROTATION_VECTOR = 11,
    AMBIENT_TEMPERATURE = 13,
    MAGNETOMETER_UNCALIBRATED = 14,
    GAME_ROTATION_VECTOR = 15,
    GYROSCOPE_UNCALIBRATED = 16,
    STEP_DETECTOR = 18,
    GEOMAGNETIC_ROTATION_VECTOR = 20,
    ADDITIONAL_INFO = 33,
    LOW_LATENCY_OFFBODY_DETECT = 34,
    ACCELEROMETER_UNCALIBRATED = 35,
    HINGE_ANGLE = 36,
    INTER_PUPILLARY_DISTANCE = 33171007,
};


NR_PLUGIN_ENUM32(NRAxrSensorSensorAccuracy) {
    SENSOR_ACCURACY_UNSPECIFIED = 0,
    SENSOR_ACCURACY_LOW = 1,
    SENSOR_ACCURACY_MEDIUM = 2,
    SENSOR_ACCURACY_HIGH = 3,
};


NR_PLUGIN_ENUM32(NRAxrSensorSensorAdditionalInfoType) {
    SENSOR_ADDITIONAL_INFO_TYPE_UNSPECIFIED = 0,
    SENSOR_ADDITIONAL_INFO_TYPE_INTERNAL_TEMPERATURE = 65537,
    SENSOR_ADDITIONAL_INFO_TYPE_LOCAL_VELOCITY = 65538,
    SENSOR_ADDITIONAL_INFO_TYPE_UWB_RANGE = 65568,
};


NR_PLUGIN_ENUM32(NRAxrSettingsPowerStatus) {
    NORMAL = 0,
    SLEEP = 1,
};


NR_PLUGIN_ENUM32(NRAxrTimeRequestType) {
    TIME_REQUEST_TYPE_NONE = 0,
    SYNC_REQUEST = 1,
    DELAY_REQUEST = 2,
};


NR_PLUGIN_ENUM32(NRAxrTimeResponseType) {
    SYNC_RESPONSE = 0,
    CONVERGED = 3,
    NOT_CONVERGED = 4,
};


NR_PLUGIN_ENUM32(NRAxrDeviceStateValueSubMessageType) {
    NR_AXR_DEVICE_STATE_VALUE_NULL_SUB_MESSAGE_TYPE = 0,
    NR_AXR_DEVICE_STATE_DEVICE_VALUE_SUB_MESSAGE_TYPE = 1,
    NR_AXR_DEVICE_STATE_BATTERY_CHARGE_VALUE_SUB_MESSAGE_TYPE = 2,
    NR_AXR_DEVICE_STATE_PRODUCT_INFO_VALUE_SUB_MESSAGE_TYPE = 3,
    NR_AXR_DEVICE_STATE_DISPLAY_BRIGHTNESS_VALUE_SUB_MESSAGE_TYPE = 4,
    NR_AXR_DEVICE_STATE_AUDIO_VOLUME_VALUE_SUB_MESSAGE_TYPE = 5,
    NR_AXR_DEVICE_STATE_AUDIO_MUTE_VALUE_SUB_MESSAGE_TYPE = 6,
    NR_AXR_DEVICE_STATE_DEVICE_WEAR_STATE_VALUE_SUB_MESSAGE_TYPE = 7,
    NR_AXR_DEVICE_STATE_SOFTWARE_INFO_VALUE_SUB_MESSAGE_TYPE = 8,
    NR_AXR_DEVICE_STATE_AUTO_BRIGHTNESS_MODE_VALUE_SUB_MESSAGE_TYPE = 9,
    NR_AXR_DEVICE_STATE_DISPLAY_DIMMING_VALUE_SUB_MESSAGE_TYPE = 10,
    NR_AXR_DEVICE_STATE_DISPLAY_POWER_MODE_VALUE_SUB_MESSAGE_TYPE = 11,
    NR_AXR_DEVICE_STATE_PRIVACY_LED_VALUE_SUB_MESSAGE_TYPE = 12,
    NR_AXR_DEVICE_STATE_DEVICE_POWER_STATUS_VALUE_SUB_MESSAGE_TYPE = 13,
};

#pragma pack(1)

typedef struct NRAxrStringEntry {
    const char * data_data;
    uint32_t data_size;

} NRAxrStringEntry;


typedef struct NRAxrImuDataRequestInfo {
    union {
        struct {
            uint32_t sensor_info_count;
            const NRSensorId * sensor_id;
            const int32_t * sampling_period_us;
        };
        uint8_t padding[64];
    };

} NRAxrImuDataRequestInfo;


typedef struct NRAxrAudioMicConfig {
    NRAxrAudioSampleRate sample_rate_hz;
    uint32_t num_channels;
    NRAxrAudioSampleFormat format;
    bool enable_denoiser;
    bool enable_linear_aec;
    bool enable_nonlinear_aec;
    bool enable_agc;

} NRAxrAudioMicConfig;


typedef struct NRAxrAudioSpeakerConfig {
    NRAxrAudioSampleRate sample_rate_hz;
    uint32_t num_channels;
    NRAxrAudioSampleFormat format;
    bool enable_eq;

} NRAxrAudioSpeakerConfig;


typedef struct NRAxrAudioMicCapability {
    int32_t * supported_sample_rates_hz_data;
    uint32_t supported_sample_rates_hz_size;
    uint32_t * supported_num_channels_data;
    uint32_t supported_num_channels_size;
    int32_t * supported_sample_formats_data;
    uint32_t supported_sample_formats_size;
    bool supports_denoiser;
    bool supports_linear_aec;
    bool supports_nonlinear_aec;
    bool supports_agc;

} NRAxrAudioMicCapability;


typedef struct NRAxrAudioSpeakerCapability {
    int32_t * supported_sample_rates_hz_data;
    uint32_t supported_sample_rates_hz_size;
    uint32_t * supported_num_channels_data;
    uint32_t supported_num_channels_size;
    int32_t * supported_sample_formats_data;
    uint32_t supported_sample_formats_size;
    bool supports_speaker_eq;

} NRAxrAudioSpeakerCapability;


typedef struct NRAxrBlobBlobId {
    NRAxrBlobTable table;
    int64_t local_id;

} NRAxrBlobBlobId;


typedef struct NRAxrBlobBlobInfo {
    NRAxrBlobBlobId id;
    const char * name_data;
    uint32_t name_size;
    int64_t size;
    int64_t created_timestamp_ms;
    int64_t modified_timestamp_ms;
    const char * mime_type_data;
    uint32_t mime_type_size;
    const char * extra_data;
    uint32_t extra_size;

} NRAxrBlobBlobInfo;


typedef struct NRAxrCompositionHeadTracking {
    NRAxrCompositionType type;
    int64_t minimum_period_us;
    int64_t maximum_period_us;
    NRAxrCompositionHeadTrackingTransport head_tracking_transport;
    NRAxrCompositionHeadTrackingReadMethod head_tracking_read_method;

} NRAxrCompositionHeadTracking;


typedef struct NRAxrCompositionSetHeadTrackingReadMethod {
    NRAxrCompositionHeadTrackingReadMethod head_tracking_read_method;
    int64_t sampling_period_us;

} NRAxrCompositionSetHeadTrackingReadMethod;


typedef struct NRAxrDeviceStateKeyMetadata {
    bool is_constant;

} NRAxrDeviceStateKeyMetadata;


typedef struct NRAxrDeviceStateKeyMetadataMapEntry {
    uint32_t key;
    NRAxrDeviceStateKeyMetadata value;

} NRAxrDeviceStateKeyMetadataMapEntry;


typedef struct NRAxrDeviceStateBatteryChargeValue {
    uint32_t charge_level;
    bool is_charging;

} NRAxrDeviceStateBatteryChargeValue;


typedef struct NRAxrDeviceStateProductInfoValue {
    const char * product_name_data;
    uint32_t product_name_size;
    const char * serial_number_data;
    uint32_t serial_number_size;
    const char * model_name_data;
    uint32_t model_name_size;
    const char * fingerprint_data;
    uint32_t fingerprint_size;
    const char * device_manufacturer_data;
    uint32_t device_manufacturer_size;
    int32_t hardware_revision_number;

} NRAxrDeviceStateProductInfoValue;


typedef struct NRAxrDeviceStateDisplayBrightnessValue {
    uint32_t value;

} NRAxrDeviceStateDisplayBrightnessValue;


typedef struct NRAxrDeviceStateAudioVolumeValue {
    uint32_t value;

} NRAxrDeviceStateAudioVolumeValue;


typedef struct NRAxrDeviceStateAudioMuteValue {
    bool value;

} NRAxrDeviceStateAudioMuteValue;


typedef struct NRAxrDeviceStateDeviceWearStateValue {
    bool is_worn;

} NRAxrDeviceStateDeviceWearStateValue;


typedef struct NRAxrDeviceStateSoftwareInfoValue {
    const char * os_build_id_data;
    uint32_t os_build_id_size;

} NRAxrDeviceStateSoftwareInfoValue;


typedef struct NRAxrDeviceStateAutoBrightnessModeValue {
    bool is_enabled;

} NRAxrDeviceStateAutoBrightnessModeValue;


typedef struct NRAxrDeviceStateDisplayDimmingValue {
    uint32_t level;

} NRAxrDeviceStateDisplayDimmingValue;


typedef struct NRAxrDeviceStateDisplayPowerModeValue {
    NRAxrDisplayPowerMode power_mode;

} NRAxrDeviceStateDisplayPowerModeValue;


typedef struct NRAxrDeviceStatePrivacyLedValue {
    bool is_enabled;

} NRAxrDeviceStatePrivacyLedValue;


typedef struct NRAxrDeviceStateDevicePowerStatusValue {
    NRAxrSettingsPowerStatus power_status;

} NRAxrDeviceStateDevicePowerStatusValue;


typedef struct NRAxrDeviceStateDeviceValue {
    const char * type_url_data;
    uint32_t type_url_size;
    const char * value_data;
    uint32_t value_size;

} NRAxrDeviceStateDeviceValue;


typedef struct NRAxrDeviceStateValue {
    NRAxrDeviceStateValueSubMessageType sub_message_type;
    void * sub_message;
    uint32_t sub_message_size;

} NRAxrDeviceStateValue;


typedef struct NRAxrDisplayDimmingLevel {
    int32_t level;
    float percentage;

} NRAxrDisplayDimmingLevel;


typedef struct NRAxrDisplayDimmingSupport {
    NRAxrDisplayDimmingType dimming_type;
    int32_t supported_dimming_levels_data[8];
    uint32_t supported_dimming_levels_size;
    NRAxrDisplayDimmingLevel dimming_levels_data[8];
    uint32_t dimming_levels_size;

} NRAxrDisplayDimmingSupport;


typedef struct NRAxrDisplayDisplayPortAltModeTransport {
    const char * version_data;
    uint32_t version_size;
    bool display_stream_compression_support;

} NRAxrDisplayDisplayPortAltModeTransport;


typedef struct NRAxrDisplayDisplayAngularOffset {
    float horizontal_offset_degrees;
    float vertical_offset_degrees;

} NRAxrDisplayDisplayAngularOffset;


typedef struct NRAxrDisplayDistortionMesh {
    NRAxrDisplayFormat format;
    NRAxrBlobBlobInfo blob_info;

} NRAxrDisplayDistortionMesh;


typedef struct NRAxrDisplayDisplayProfile {
    int32_t rank;
    int32_t width_px;
    int32_t height_px;
    int32_t dpi;
    float horizontal_fov_degrees;
    float vertical_fov_degrees;
    int32_t ipd_mm;
    int32_t focal_length_mm;
    NRAxrDisplayDisplayAngularOffset left_eye_offset;
    NRAxrDisplayDisplayAngularOffset right_eye_offset;
    NRAxrDisplayDistortionMesh left_eye_distortion_mesh;
    NRAxrDisplayDistortionMesh right_eye_distortion_mesh;
    int32_t refresh_rates_data[16];
    uint32_t refresh_rates_size;
    NRAxrDisplayColorSpace color_space;
    NRAxrDisplayDisplayTransport transport;
    NRAxrDisplayDisplayPortAltModeTransport dp_alt_mode;
    int32_t illumination_start_offset_ns_data[16];
    uint32_t illumination_start_offset_ns_size;
    int32_t illumination_end_offset_ns_data[16];
    uint32_t illumination_end_offset_ns_size;

} NRAxrDisplayDisplayProfile;


typedef struct NRAxrInputKeyEvent {
    NRAxrInputAction action;
    int32_t key_code;
    int64_t timestamp_us;

} NRAxrInputKeyEvent;


typedef struct NRAxrLoggingLogEntry {
    const char * tag_data;
    uint32_t tag_size;
    int64_t timestamp_ms;
    NRAxrLoggingLogSeverity log_severity;
    const char * msg_data;
    uint32_t msg_size;

} NRAxrLoggingLogEntry;


typedef struct NRAxrLoggingStatusReport {
    const char * subsystem_data;
    uint32_t subsystem_size;
    const char * status_report_data;
    uint32_t status_report_size;

} NRAxrLoggingStatusReport;


typedef struct NRAxrMonitorPingRequestData {
    int64_t request_timestamp_ns;
    const char * sample_payload_data;
    uint32_t sample_payload_size;

} NRAxrMonitorPingRequestData;


typedef struct NRAxrSensorSensorStreamRequestParameters {
    int32_t sampling_period_us;
    int32_t batch_size_us;
    int32_t * additional_info_types_data;
    uint32_t additional_info_types_size;

} NRAxrSensorSensorStreamRequestParameters;


typedef struct NRAxrSensorSensorStreamRequestParametersMapEntry {
    int32_t key;
    NRAxrSensorSensorStreamRequestParameters value;

} NRAxrSensorSensorStreamRequestParametersMapEntry;


typedef struct NRAxrSensorSensorSample {
    int32_t sensor_id;
    int64_t timestamp_ns;
    NRAxrSensorSensorAccuracy accuracy;
    float * values_data;
    uint32_t values_size;
    NRAxrSensorSensorAdditionalInfoType additional_info_type;

} NRAxrSensorSensorSample;


typedef struct NRAxrSensorSensorCapability {
    int32_t sensor_id;
    NRAxrSensorSensorType sensor_type;
    const char * name_data;
    uint32_t name_size;
    const char * vendor_data;
    uint32_t vendor_size;
    float range;
    float resolution;
    int32_t min_sampling_period_us;
    int32_t max_sampling_period_us;
    bool supports_additional_info;

} NRAxrSensorSensorCapability;


typedef struct NRAxrSettingsSystemTime {
    int64_t timestamp_ms;
    const char * time_zone_data;
    uint32_t time_zone_size;

} NRAxrSettingsSystemTime;

#pragma pack()

#endif // NRAPP