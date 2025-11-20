#pragma once

#ifdef NRAPP

#include "nr_plugin_types_ext.inc"

#else


NR_PLUGIN_ENUM(NRNetworkType) {
    NR_NETWORK_TYPE_SINGLE = 0,
    NR_NETWORK_TYPE_SERVER = 1,
    NR_NETWORK_TYPE_CLIENT = 2,
    NR_NETWORK_TYPE_QVR = 3,
};


NR_PLUGIN_ENUM(NRWorkingMode) {
    NR_WORKING_MODE_HEAVY = 0,
    NR_WORKING_MODE_LIGHT = 1,
};


NR_PLUGIN_ENUM(NRDeviceType) {
    NR_DEVICE_TYPE_INVALID = 0,
    NR_DEVICE_TYPE_LIGHT = 1,
    NR_DEVICE_TYPE_AIR = 2,
    NR_DEVICE_TYPE_P55E = 3,
    NR_DEVICE_TYPE_P55F = 4,
    NR_DEVICE_TYPE_FLORA = 5,
    NR_DEVICE_TYPE_GINA_FLORA = 10,
    NR_DEVICE_TYPE_GINA_M = 11,
    NR_DEVICE_TYPE_GINA_L = 12,
    NR_DEVICE_TYPE_HYLLA_M = 13,
    NR_DEVICE_TYPE_HYLLA_L = 14,
    NR_DEVICE_TYPE_GS = 15,
    NR_DEVICE_TYPE_HONOR_AIR = 1001,
    NR_DEVICE_TYPE_HISENSE = 1002,
    NR_DEVICE_TYPE_ROG_M = 1003,
    NR_DEVICE_TYPE_ROG_L = 1004,
};


NR_PLUGIN_ENUM(NRComponent) {
    NR_COMPONENT_INVALID = -1,
    NR_COMPONENT_DISPLAY_LEFT = 0,
    NR_COMPONENT_DISPLAY_RIGHT,
    NR_COMPONENT_RGB_CAMERA,
    NR_COMPONENT_GRAYSCALE_CAMERA_LEFT,
    NR_COMPONENT_GRAYSCALE_CAMERA_RIGHT,
    NR_COMPONENT_MAGNETIC,
    NR_COMPONENT_HEAD,
    NR_COMPONENT_IMU,
    NR_COMPONENT_NECK,
    NR_COMPONENT_NUM,
    NR_COMPONENT_DISPLAY_NUM = 2,
};


NR_PLUGIN_ENUM(NRChannelType) {
    NR_CHANNEL_TYPE_INVALID = -1,
    NR_CHANNEL_TYPE_MCU = 0,
    NR_CHANNEL_TYPE_RGB_CAMERA,
    NR_CHANNEL_TYPE_IMU,
    NR_CHANNEL_TYPE_GRAYSCALE_CAMERA,
    NR_CHANNEL_TYPE_GLASSES_CONFIG,
    NR_CHANNEL_TYPE_VSYNC,
    NR_CHANNEL_TYPE_DP,
    NR_CHANNEL_TYPE_EXTERNAL_SENSOR,
    NR_CHANNEL_TYPE_AMBIENT_LIGHT,
    NR_CHANNEL_TYPE_NUM,
};


NR_PLUGIN_ENUM64(NRChannelDataType) {
    NR_CHANNEL_DATA_TYPE_NONE = 0x00LL,
    NR_CHANNEL_DATA_TYPE_GLASSES_GRAYSCALE_CAMERA = 0x01LL,
    NR_CHANNEL_DATA_TYPE_GLASSES_IMU_MAIN = 0x02LL,
    NR_CHANNEL_DATA_TYPE_ANDROID_SYSTEM_IMU = 0x04LL,
    NR_CHANNEL_DATA_TYPE_EXTERNAL_SENSOR = 0x08LL,
    NR_CHANNEL_DATA_TYPE_GLASSES_IMU_MINOR = 0x10LL,
    NR_CHANNEL_DATA_TYPE_GLASSES_IMU_MAG = 0x20LL,
    NR_CHANNEL_DATA_TYPE_GLASSES_AMBIENT_LIGHT = 0x40LL,
};


NR_PLUGIN_ENUM(NRLatencyType) {
    NR_LATENCY_TYPE_IMU_POSE = 0,
};


NR_PLUGIN_ENUM(NRTrackingPoseType) {
    NR_TRACKING_POSE_TYPE_NORMAL = 0,
    NR_TRACKING_POSE_TYPE_LOW_LATENCY = 1,
};

	/*
		
		useless
		
	*/
NR_PLUGIN_ENUM(NRDisplay2D3DMode) {
    NR_DISPLAY_MODE_UNKNOWN = 0,
    NR_DISPLAY_MODE_2D = 1,
    NR_DISPLAY_MODE_3D = 2,
};


NR_PLUGIN_ENUM(NRProperty) {
    NR_PROPERTY_UNKNOWN = 0,
    NR_PROPERTY_START = 1,
    NR_PROPERTY_LOADER = NR_PROPERTY_START,
    NR_PROPERTY_HARDWARE_TYPE,
    NR_PROPERTY_SINGLE_BUFFER,
    NR_PROPERTY_NUM,
};


NR_PLUGIN_ENUM(NRMetricsType) {
    NR_METRICS_TYPE_NULL = 0x00,
    NR_METRICS_TYPE_FRAME_PRESENT_COUNT = 0x01,
    NR_METRICS_TYPE_TEARED_FRAME_COUNT = 0x02,
    NR_METRICS_TYPE_EARLY_FRAME_COUNT = 0x04,
    NR_METRICS_TYPE_DROPPED_FRAME_COUNT = 0x08,
    NR_METRICS_TYPE_FRAME_COMPOSITE_TIME = 0x10,
    NR_METRICS_TYPE_MOTION_TO_PHOTON = 0x20,
    NR_METRICS_TYPE_EXTENDED_FRAME_COUNT = 0x40,
    NR_METRICS_TYPE_TW_FPS = 0x80,
};


NR_PLUGIN_ENUM(NRCameraModel) {
    NR_CAMERA_MODEL_RADIAL = 1,
    NR_CAMERA_MODEL_FISHEYE = 2,
    NR_CAMERA_MODEL_FISHEYE624 = 3,
};


NR_PLUGIN_ENUM(NRSceneMode) {
    NR_SCENE_MODE_SPACE_SCREEN = 0,
    NR_SCENE_MODE_WITH_NEBULA = 1,
};


NR_PLUGIN_ENUM(NRPerceptionType) {
    NR_PERCEPTION_TYPE_6DOF = 0,
    NR_PERCEPTION_TYPE_3DOF,
    NR_PERCEPTION_TYPE_0DOF,
    NR_PERCEPTION_TYPE_EIS,
    NR_PERCEPTION_TYPE_OSD_EIS,
    NR_PERCEPTION_TYPE_MOTION,
    NR_PERCEPTION_TYPE_SENSOR_CALIBRATION,
    NR_PERCEPTION_TYPE_3DOF_OPTI,
    NR_PERCEPTION_TYPE_AUTO_BRIGHTNESS,
    NR_PERCEPTION_TYPE_REMOTE = 100,
};


NR_PLUGIN_ENUM(NRKeyEvent) {
    NR_KEY_EVENT_NULL = 0,
    NR_KEY_EVENT_CLICK,
    NR_KEY_EVENT_DOUBLE_CLICK,
    NR_KEY_EVENT_PRESS,
};


NR_PLUGIN_ENUM(NRFrameBufferFormat) {
    NR_FRAME_BUFFER_FORMAT_RGBA_PLANAR = 0,
    NR_FRAME_BUFFER_FORMAT_ARGB_PLANAR = 1,
    NR_FRAME_BUFFER_FORMAT_YUV420_PLANAR = 2,
    NR_FRAME_BUFFER_FORMAT_BGRA_8888 = 3,
    NR_FRAME_BUFFER_FORMAT_BGRA_4444 = 4,
};


NR_PLUGIN_ENUM(NREnableValue) {
    NR_ENABLE_VALUE_DISABLE = 0,
    NR_ENABLE_VALUE_ENABLE = 1,
};


NR_PLUGIN_ENUM(NRResolution) {
    NR_RESOLUTION_UNKNOWN = 0,
    NR_RESOLUTION_1920_1080_60 = 1,
    NR_RESOLUTION_1920_1080_72 = 2,
    NR_RESOLUTION_1920_1080_90 = 3,
    NR_RESOLUTION_1920_1080_120 = 4,
    NR_RESOLUTION_3840_1080_60 = 5,
    NR_RESOLUTION_3840_1080_72 = 6,
    NR_RESOLUTION_3840_1080_90 = 7,
    NR_RESOLUTION_3840_1080_120 = 8,
    NR_RESOLUTION_1920_1200_60 = 21,
    NR_RESOLUTION_1920_1200_72 = 22,
    NR_RESOLUTION_1920_1200_90 = 23,
    NR_RESOLUTION_1920_1200_120 = 24,
    NR_RESOLUTION_3840_1200_60 = 25,
    NR_RESOLUTION_3840_1200_72 = 26,
    NR_RESOLUTION_3840_1200_90 = 27,
    NR_RESOLUTION_3840_1200_120 = 28,
    NR_RESOLUTION_2560_1080_60 = 41,
    NR_RESOLUTION_2560_1080_72 = 42,
    NR_RESOLUTION_2560_1080_75 = 43,
    NR_RESOLUTION_2560_1080_90 = 44,
    NR_RESOLUTION_2560_1080_120 = 45,
    NR_RESOLUTION_2560_1200_60 = 61,
    NR_RESOLUTION_2560_1200_72 = 62,
    NR_RESOLUTION_2560_1200_75 = 63,
    NR_RESOLUTION_2560_1200_90 = 64,
    NR_RESOLUTION_2560_1200_120 = 65,
    NR_RESOLUTION_1920_540_240 = 80,
};


NR_PLUGIN_ENUM(NRDirection) {
    NR_DIRECTION_UP = 0,
    NR_DIRECTION_DOWN = 1,
    NR_DIRECTION_LEFT = 2,
    NR_DIRECTION_RIGHT = 3,
};

	/*
		
		/// @brief 左右屏幕
		/// \n NR_DISPLAY_USAGE_LEFT: 左屏幕
		/// \n NR_DISPLAY_USAGE_RIGHT: 右屏幕
		
	*/
NR_PLUGIN_ENUM(NRDisplayUsage) {
    NR_DISPLAY_USAGE_LEFT = 0,
    NR_DISPLAY_USAGE_RIGHT,
};


NR_PLUGIN_ENUM(NRStepType) {
    NR_STEP_TYPE_NORMAL = 0,
    NR_STEP_TYPE_EXTENDED = 1,
};

	/*
		
		NR_IMAGE_FORMAT_YUV_420_888
		1 表示一种抽象的 YUV 4:2:0 格式，不具体指定存储的细节
		2 不具体定义存储方式，实际存储可以是多种具体格式（如 NV12、NV21、YV12）
		NR_IMAGE_FORMAT_YV12：
		1 Y（亮度）分量单独存储，紧跟其后是 V（Cr）和 U（Cb）分量
		2 V（Cr）分量存储在 U（Cb）分量之前
		3 排列方式是 YYYYYYYY VV UU
		NR_IMAGE_FORMAT_NV21：
		1 Y（亮度）分量单独存储
		2 紧跟在 Y 分量之后，V（Cr）和 U（Cb）交错存储。
		3 排列方式是 YYYYYYYY VUVU。
		NR_IMAGE_FORMAT_NV12：
		1 Y（亮度）分量单独存储
		2 紧跟在 Y 分量之后，U（Cb）和 V（Cr）交错存储。
		3 排列方式是 YYYYYYYY UVUV。
		
	*/
NR_PLUGIN_ENUM(NRImageFormat) {
    NR_IMAGE_FORMAT_UNKNOWN = 0,
    NR_IMAGE_FORMAT_BGR_888_packed = 1,
    NR_IMAGE_FORMAT_RGB_888_packed,
    NR_IMAGE_FORMAT_BGRA_8888_packed,
    NR_IMAGE_FORMAT_RGBA_8888_packed,
    NR_IMAGE_FORMAT_BGR_888_planar,
    NR_IMAGE_FORMAT_RGB_888_planar,
    NR_IMAGE_FORMAT_BGRA_8888_planar,
    NR_IMAGE_FORMAT_RGBA_8888_planar,
    NR_IMAGE_FORMAT_ARGB_8888_planar,
    NR_IMAGE_FORMAT_GRAY_8,
    NR_IMAGE_FORMAT_YUV_420_888,
    NR_IMAGE_FORMAT_YV12,
    NR_IMAGE_FORMAT_NV21,
    NR_IMAGE_FORMAT_NV12,
    NR_IMAGE_FORMAT_BGRA_4444_packed,
};

	/*
		
		/// @brief NREdid 枚举
		/// \n 规则: 使用该 EDID 默认的分辨率/刷新率 做为枚举值
		/// \n 如果是复合分辨率（即一个 EDID 对应多个分辨率/刷新率），
		/// \n 则 在默认的分辨率/刷新率 后面加上 _XXX，XXX 为用途。
		/// \n 例如: NR_EDID_1920_1080_60_DEFAULT : 表示开机上电后，使用NR_EDID_1920_1080_60_DEFAULT作为默认的 EDID
		
	*/
NR_PLUGIN_ENUM(NREdid) {
    NR_EDID_UNKNOWN = 0,
    NR_EDID_1920_1080_60 = 1,
    NR_EDID_1920_1080_72 = 2,
    NR_EDID_1920_1080_90 = 3,
    NR_EDID_1920_1080_120 = 4,
    NR_EDID_3840_1080_60 = 5,
    NR_EDID_3840_1080_72 = 6,
    NR_EDID_3840_1080_90 = 7,
    NR_EDID_3840_1080_120 = 8,
    NR_EDID_1920_1080_90_DEFAULT = 9,
    NR_EDID_1920_1080_240 = 10,
    NR_EDID_1920_1080_30 = 11,
    NR_EDID_1920_1080_120_DEFAULT = 12,
    NR_EDID_1920_1200_60 = 21,
    NR_EDID_1920_1200_72 = 22,
    NR_EDID_1920_1200_90 = 23,
    NR_EDID_1920_1200_120 = 24,
    NR_EDID_3840_1200_60 = 25,
    NR_EDID_3840_1200_72 = 26,
    NR_EDID_3840_1200_90 = 27,
    NR_EDID_3840_1200_120 = 28,
    NR_EDID_1920_1200_90_DEFAULT = 29,
    NR_EDID_2560_1080_60 = 41,
    NR_EDID_2560_1080_72 = 42,
    NR_EDID_2560_1080_75 = 43,
    NR_EDID_2560_1080_90 = 44,
    NR_EDID_2560_1080_120 = 45,
    NR_EDID_2560_1080_90_DEFAULT = 46,
    NR_EDID_2560_1200_60 = 61,
    NR_EDID_2560_1200_72 = 62,
    NR_EDID_2560_1200_75 = 63,
    NR_EDID_2560_1200_90 = 64,
    NR_EDID_2560_1200_120 = 65,
    NR_EDID_1920_2160_60 = 80,
    NR_EDID_4092_1280_72 = 100,
    NR_EDID_1280_960_120_DEFAULT = 120,
    NR_EDID_1280_1024_120_DEFAULT = 140,
    NR_EDID_1080_1920_120_DEFAULT = 160,
};

	/*
		
		/// @brief DP 数据过滤模式
		/// \n NR_DP_DATA_FILTER_MODE_UNKNOWN 未知模式
		/// \n NR_DP_DATA_FILTER_MODE_DIRECT 直通模式 (不做任何过滤)
		/// \n NR_DP_DATA_FILTER_MODE_DEFAULT 默认模式
		
	*/
NR_PLUGIN_ENUM(NRDpDataFilterMode) {
    NR_DP_FILTER_MODE_UNKNOWN = 0,
    NR_DP_FILTER_MODE_DIRECT = 1,
    NR_DP_FILTER_MODE_DEFAULT = 2,
    NR_DP_FILTER_MODE_1 = 1,
    NR_DP_FILTER_MODE_2 = 2,
    NR_DP_FILTER_MODE_3 = 3,
    NR_DP_FILTER_MODE_4 = 4,
    NR_DP_FILTER_MODE_5 = 5,
};

	/*
		<enum name="NRDpInputScaleType">
		<enum_unit name="NR_DP_INPUT_SCALE_TYPE_NORMAL"                                   value="0"/>
		<enum_unit name="NR_DP_INPUT_SCALE_TYPE_ULTRA_WIDE"                                   value="1"/>
		</enum> 
	*/
NR_PLUGIN_ENUM(NRSpaceMode) {
    NR_SPACE_MODE_HOVER = 0,
    NR_SPACE_MODE_FOLLOW = 1,
    NR_SPACE_MODE_THUMBNAIL = 2,
    NR_SPACE_MODE_ULTRA_WIDE = 3,
};


NR_PLUGIN_ENUM(NRAvailableValue) {
    NR_AVAILABLE_VALUE_NOT_AVAILABLE = 0,
    NR_AVAILABLE_VALUE_AVAILABLE = 1,
};


NR_PLUGIN_ENUM(NROperationType) {
    NR_OPERATION_INCREASE = 0,
    NR_OPERATION_DECREASE = 1,
};


NR_PLUGIN_ENUM(NRThumbnailPositionType) {
    NR_THUMBNAIL_POSE_TYPE_LEFT_TOP = 0,
    NR_THUMBNAIL_POSE_TYPE_RIGHT_TOP = 1,
};


NR_PLUGIN_ENUM(NRCanvasAreaType) {
    NR_CANVAS_AREA_UNKNOWN = 0,
    NR_CANVAS_AREA_INSIDE_IMMERSIVE_BOUND = 1,
    NR_CANVAS_AREA_BETWEEN_IMMERSIVE_AND_INTERACTION_BOUND = 2,
    NR_CANVAS_AREA_OUTSIDE_INTERACTION_BOUND = 3,
};


NR_PLUGIN_ENUM(NRCalibrationTriggerType) {
    NR_CALIBRATION_TRIGGER_TYPE_UNKNOWN = 0,
    NR_CALIBRATION_TRIGGER_TYPE_SLIENT = 1,
    NR_CALIBRATION_TRIGGER_TYPE_MANUAL = 2,
};


NR_PLUGIN_ENUM(NRCalibrationResult) {
    NR_CALIBRATION_RESULT_FAILED = -1,
    NR_CALIBRATION_RESULT_SUCCESS = 0,
    NR_CALIBRATION_RESULT_DATA_ERROR = 1,
    NR_CALIBRATION_RESULT_BIAS_ERROR = 2,
    NR_CALIBRATION_RESULT_TIMEOUT = 3,
    NR_CALIBRATION_RESULT_NOT_STILL = 4,
};


NR_PLUGIN_ENUM(NRDeviceConnectMode) {
    NR_DEVICE_CONNECT_MODE_USB = 0,
    NR_DEVICE_CONNECT_MODE_SOCKET = 1,
};


NR_PLUGIN_ENUM(NRStatsCategoryID) {
    NR_STATS_CATEGORY_ID_NORMAL = 5,
};


NR_PLUGIN_ENUM(NRStatsEventID) {
    NR_STATS_EVENT_ID_EC_LEVEL = 0x0030,
    NR_STATS_EVENT_ID_CLICK_SPACE_MODE = 0x0047,
    NR_STATS_EVENT_ID_DOUBLE_CLICK_ENTER_OSD_MENU = 0x0048,
    NR_STATS_EVENT_ID_BRIGHTNESS_ADJUST = 0x0049,
    NR_STATS_EVENT_ID_PRESS_RECENTER = 0x004A,
    NR_STATS_EVENT_ID_OSD_VOLUME_ADJUST = 0x004B,
    NR_STATS_EVENT_ID_SWITCH_AUDIO_MODE = 0x0028,
    NR_STATS_EVENT_ID_OSD_CANVAS_DEPTH_ADJUSTMENT = 0x004C,
    NR_STATS_EVENT_ID_OSD_CANVAS_DIAGONAL_SIZE_ADJUST = 0x004D,
    NR_STATS_EVENT_ID_ENABLE_3D_MODE = 0x004E,
    NR_STATS_EVENT_ID_OSD_SWITCH_DISPLAY_COLOR_CALIBRATION = 0x004F,
    NR_STATS_EVENT_ID_OSD_COLOR_TEMPERATURE_ADJUST = 0x0050,
    NR_STATS_EVENT_ID_OSD_CLICK_HOST_KEY = 0x0051,
    NR_STATS_EVENT_ID_OSD_PRESS_HOST_KEY = 0x0052,
    NR_STATS_EVENT_ID_TRANSPARENT_STATE_BEGIN = 0x0053,
    NR_STATS_EVENT_ID_RGB_CAMERA_PLUGIN = 0x0054,
    NR_STATS_EVENT_ID_RGB_CAMERA_PLUGOUT = 0x0055,
    NR_STATS_EVENT_ID_STARTUP_BEGIN = 0x0059,
    NR_STATS_EVENT_ID_STARTUP_END = 0x005A,
    NR_STATS_EVENT_ID_TRANSPARENT_STATE_END = 0x005B,
    NR_STATS_EVENT_ID_EXIT_OSD = 0x005C,
    NR_STATS_EVENT_ID_ENABLE_ULTRA_WIDE = 0x005D,
    NR_STATS_EVENT_ID_CHANGE_ULTRA_WIDE = 0x005E,
    NR_STATS_EVENT_ID_AUTO_SLEEP_DURATION_SETTING = 0x005F,
    NR_STATS_EVENT_ID_OSD_PUPIL_ADJUST = 0x0060,
    NR_STATS_EVENT_ID_CLICK_TAKE_PICTURE = 0x0061,
    NR_STATS_EVENT_ID_PRESS_TAKE_VIDEO = 0x0062,
    NR_STATS_EVENT_ID_END_TAKE_VIDEO = 0x0063,
    NR_STATS_EVENT_ID_OSD_SET_VIDEO_DURATION = 0x0064,
    NR_STATS_EVENT_ID_OSD_SET_VIDEO_FRAME = 0x0065,
    NR_STATS_EVENT_ID_OSD_CLEANUP_STORAGE = 0x0066,
    NR_STATS_EVENT_ID_OSD_KEY_TUTORIAL_ENTRY = 0x0067,
    NR_STATS_EVENT_ID_OSD_STATIC_CALIBRATION_ENTRY = 0x0068,
    NR_STATS_EVENT_ID_OSD_MANUAL_STATIC_CALIBRATION_END = 0x0069,
    NR_STATS_EVENT_ID_AUTO_STATIC_CALIBRATION_END = 0x0070,
    NR_STATS_EVENT_ID_OSD_LANGUAGE_SETTING = 0x0071,
    NR_STATS_EVENT_ID_DISPLAY_SCREEN_ENABLE_DURATION = 0x0072,
    NR_STATS_EVENT_ID_HIGH_TEMPERATURE = 0x0073,
    NR_STATS_EVENT_ID_OVER_TEMPERATURE = 0x0074,
    NR_STATS_EVENT_ID_OSD_BRIGHTNESS_ENHANCE_TOGGLE = 0x0075,
    NR_STATS_EVENT_ID_OSD_FOLLOW_EIS_TOGGLE = 0x0076,
    NR_STATS_EVENT_ID_OSD_THUMBNAIL_MODE_TOGGLE = 0x0077,
    NR_STATS_EVENT_ID_OSD_THUMBNAIL_POSITION_SELECTION = 0x0078,
    NR_STATS_EVENT_ID_OSD_AUTO_EC_TOGGLE = 0x0079,
    NR_STATS_EVENT_ID_TEXT_OPTIMIZATION_TOGGLE = 0x007A,
    NR_STATS_EVENT_ID_OSD_6DOF_TOGGLE = 0x007C,
    NR_STATS_EVENT_ID_OSD_SOUND_EFFECT_SELECT = 0x0080,
    NR_STATS_EVENT_ID_OSD_NO_DP_SCREEN_SHOW = 0x0081,
    NR_STATS_EVENT_ID_OSD_MENU_DIRECTION_REVERSED = 0x0082,
    NR_STATS_EVENT_ID_OSD_TCP_IP_TOGGLE = 0x0083,
    NR_STATS_EVENT_ID_OSD_DOUBLE_CLICK_HOST_KEY = 0x0084,
    NR_STATS_EVENT_ID_OSD_CANVAS_OPTIMAL = 0x0085,
    NR_STATS_EVENT_ID_OSD_LAB_DEFAULT_AUDIO_DP_TOGGLE = 0x0086,
    NR_STATS_EVENT_ID_OSD_LAB_DEFAULT_ULTRA_WIDE_MODE = 0x0087,
    NR_STATS_EVENT_ID_OSD_LAB_UVC_TOGGLE = 0x0088,
    NR_STATS_EVENT_ID_OSD_HOVER_OPTIMAL_3DOF_TOGGLE = 0x0089,
    NR_STATS_EVENT_ID_OSD_PLUS_MINUS_PANEL_ADJUST = 0x008C,
    NR_STATS_EVENT_ID_OSD_GAME_VISUAL_SELECT = 0x008D,
    NR_STATS_EVENT_ID_OSD_COUNTDOWN_START = 0x008E,
    NR_STATS_EVENT_ID_OSD_TIMER_START = 0x008F,
    NR_STATS_EVENT_ID_OSD_GAMMA_ADJUST = 0x0090,
    NR_STATS_EVENT_ID_OSD_LOW_BLUE_MODE_TOGGLE = 0x0091,
    NR_STATS_EVENT_ID_OSD_LIGHT_EFFECT_SELECT = 0x0092,
    NR_STATS_EVENT_ID_OSD_CONFIG_SAVE_CLICK = 0x0093,
    NR_STATS_EVENT_ID_OSD_CONFIG_APPLY_SELECT = 0x0094,
    NR_STATS_EVENT_ID_OSD_2D_TO_3D = 0x0095,
    NR_STATS_EVENT_ID_ALG_3DOF_INTERMEDIATE = 0x0096,
    NR_STATS_EVENT_ID_ALG_TCO_BG_CALIB_INFO = 0x0097,
    NR_STATS_EVENT_ID_ALG_TCO_USER_CALIB_INFO = 0x0098,
    NR_STATS_EVENT_ID_OSD_OVER_CLOCKING = 0x0099,
    NR_STATS_EVENT_ID_OSD_FRAME_RATE_BOOST = 0x009A,
};


NR_PLUGIN_ENUM(NRTrackingReason) {
    NR_TRACKING_REASON_NONE = 0,
    NR_TRACKING_REASON_INITIALIZING = 1,
    NR_TRACKING_REASON_EXCESSIVE_MOTION = 2,
    NR_TRACKING_REASON_INSUFFICIENT_FEATURES = 3,
    NR_TRACKING_REASON_RELOCALIZING = 4,
    NR_TRACKING_REASON_LOW_LIGHT = 5,
    NR_TRACKING_REASON_SENSOR_ABNORML = 6,
    NR_TRACKING_REASON_NOT_ENOUGH_SENSOR = 7,
    NR_TRACKING_REASON_SCANING = 8,
};

	/*
		
		NR_OSD_POP_UP_TYPE_NORMAL: 普通弹窗
		NR_OSD_POP_UP_TYPE_SELECT: 选择弹窗 
		
	*/
NR_PLUGIN_ENUM(NROsdPopupType) {
    NR_OSD_POP_UP_TYPE_UNKNOWN = 0,
    NR_OSD_POP_UP_TYPE_NORMAL = 1,
    NR_OSD_POP_UP_TYPE_SELECT = 2,
};

	/*
		
		/// @brief 屏显优化开着时的类型
		/// \n NR_DISPLAY_COLOR_CALIBRATION_ON_TYPE_STANDER 标准
		/// \n NR_DISPLAY_COLOR_CALIBRATION_ON_TYPE_SPORT 体育
		/// \n NR_DISPLAY_COLOR_CALIBRATION_ON_TYPE_MOVIE 电影
		/// \n NR_DISPLAY_COLOR_CALIBRATION_ON_TYPE_GAME 游戏
		
	*/
NR_PLUGIN_ENUM32(NRDisplayColorCalibrationOnType) {
    NR_DISPLAY_COLOR_CALIBRATION_ON_TYPE_STANDER = 0,
    NR_DISPLAY_COLOR_CALIBRATION_ON_TYPE_SPORT = 1,
    NR_DISPLAY_COLOR_CALIBRATION_ON_TYPE_MOVIE = 2,
    NR_DISPLAY_COLOR_CALIBRATION_ON_TYPE_GAME = 3,
};


NR_PLUGIN_ENUM(NRClockDeviceType) {
    NR_CLOCK_DEVICE_TYPE_UNKNOWN = 0,
    NR_CLOCK_DEVICE_TYPE_HOST,
    NR_CLOCK_DEVICE_TYPE_GLASSES,
};

	/*
		
		/// @brief Sensor ID
		
	*/
NR_PLUGIN_ENUM(NRSensorId) {
    NR_SENSOR_ID_UNKNOWN = 0,
    NR_SENSOR_ID_IMU_MAIN_ACCELEROMETER = 1,
    NR_SENSOR_ID_IMU_MAIN_GYROSCOPE = 2,
    NR_SENSOR_ID_IMU_MINOR_ACCELEROMETER = 3,
    NR_SENSOR_ID_IMU_MINOR_GYROSCOPE = 4,
    NR_SENSOR_ID_IMU_MAGNETOMETER = 5,
    NR_SENSOR_ID_TEMPERATURE = 6,
    NR_SENSOR_ID_LIGHT = 7,
    NR_SENSOR_ID_ROTATION_VECTOR = 8,
    NR_SENSOR_ID_PROXIMITY = 9,
    NR_SENSOR_ID_LOW_LATENCY_OFFBODY_DETECT = 10,
    NR_SENSOR_ID_IMU_MAIN_TEMPERATURE = 11,
    NR_SENSOR_ID_IMU_MINOR_TEMPERATURE = 12,
    NR_SENSOR_ID_IMU_ID_1_ACCELEROMETER = 1,
    NR_SENSOR_ID_IMU_ID_1_GYROSCOPE = 2,
    NR_SENSOR_ID_IMU_ID_2_ACCELEROMETER = 3,
    NR_SENSOR_ID_IMU_ID_2_GYROSCOPE = 4,
    NR_SENSOR_ID_MAG_ID_1_MAGNETOMETER = 5,
};


NR_PLUGIN_ENUM(NRUltraWideMode) {
    NR_ULTRA_WIDE_MODE_NONE = 0,
    NR_ULTRA_WIDE_MODE_32_9 = 1,
    NR_ULTRA_WIDE_MODE_21_9,
    NR_ULTRA_WIDE_MODE_16_18,
    NR_ULTRA_WIDE_MODE_4_3,
    NR_ULTRA_WIDE_MODE_5_4,
    NR_ULTRA_WIDE_MODE_9_16,
};


NR_PLUGIN_ENUM(NRGlassesMode) {
    NR_GLASSES_MODE_UNKNOWN = 0,
    NR_GLASSES_MODE_DISPLAY = 1,
    NR_GLASSES_MODE_LSR = 2,
    NR_GLASSES_MODE_BYPASS = 3,
};


NR_PLUGIN_ENUM(NRDofType) {
    NR_DOF_TYPE_UNKNOWN = 0,
    NR_DOF_TYPE_ROTATION_X = 1,
    NR_DOF_TYPE_ROTATION_Y = 2,
    NR_DOF_TYPE_ROTATION_Z = 3,
    NR_DOF_TYPE_TRANSLATION_X = 4,
    NR_DOF_TYPE_TRANSLATION_Y = 5,
    NR_DOF_TYPE_TRANSLATION_Z = 6,
};


NR_PLUGIN_ENUM32(NRKeyType) {
    NR_KEY_TYPE_UNKNOWN = 0,
    NR_KEY_TYPE_SELECT = 1,
    NR_KEY_TYPE_INCREASE = 2,
    NR_KEY_TYPE_DECREASE = 3,
    NR_KEY_TYPE_MENU = 4,
    NR_KEY_TYPE_ALL = 1000,
};


NR_PLUGIN_ENUM(NRKeyFunction) {
    NR_KEY_FUNCTION_UNKNOWN = 0,
    NR_KEY_FUNCTION_TRANSPARENT = 1,
    NR_KEY_FUNCTION_TRACKING_MODE_ADJUST = 2,
    NR_KEY_FUNCTION_ULTRA_WIDE_32_9 = 3,
    NR_KEY_FUNCTION_DISPLAY_MODE_ADJUST = 4,
    NR_KEY_FUNCTION_AUDIO_PROTOCOL_ADJUST = 5,
    NR_KEY_FUNCTION_EC_ADJUST = 6,
    NR_KEY_FUNCTION_VOICE_ASSISTANT = 7,
    NR_KEY_FUNCTION_HALF_DISPLAY_MODE_ADJUST = 8,
    NR_KEY_FUNCTION_TAKE_PICTURE = 9,
    NR_KEY_FUNCTION_TAKE_VIDEO = 10,
    NR_KEY_FUNCTION_ULTRA_WIDE_21_9 = 11,
    NR_KEY_FUNCTION_DEEP_SLEEP = 12,
    NR_KEY_FUNCTION_CANVAS_OPTIMAL_3DOF = 13,
    NR_KEY_FUNCTION_ULTRA_WIDE_16_18 = 14,
    NR_KEY_FUNCTION_CUSTOM_MODE = 15,
    NR_KEY_FUNCTION_GAME_VISUAL = 16,
    NR_KEY_FUNCTION_SYNTH_STEREO = 17,
    NR_KEY_FUNCTION_ULTRA_WIDE_4_3 = 18,
    NR_KEY_FUNCTION_ULTRA_WIDE_5_4 = 19,
    NR_KEY_FUNCTION_ULTRA_WIDE_9_16 = 20,
};


NR_PLUGIN_ENUM(NRDepthShiftedMode) {
    NR_DEPTH_SHIFTED_MODE_NONE = 0,
    NR_DEPTH_SHIFTED_MODE_GENTLE = 1,
    NR_DEPTH_SHIFTED_MODE_BALANCED = 2,
    NR_DEPTH_SHIFTED_MODE_STRONG = 3,
    NR_DEPTH_SHIFTED_MODE_BOOST = 4,
};


NR_PLUGIN_ENUM(NRPlaneMode) {
    NR_PLANE_MODE_NONE = 0,
    NR_PLANE_MODE_CURVE_HORIZONTAL = 1,
    NR_PLANE_MODE_CURVE_VERTICAL = 2,
};

	/*
		
		/// @brief 频率
		
	*/
NR_PLUGIN_ENUM8(NRFrequency) {
    NR_FREQUENCY_1000 = 0,
    NR_FREQUENCY_500 = 1,
    NR_FREQUENCY_250 = 2,
    NR_FREQUENCY_125 = 3,
};

	/*
		
		/// @brief Led 的 灯的颜色
		/// \n NR_LED_EFFECT_COLOR_RED: 红色
		/// \n NR_LED_EFFECT_COLOR_GREEN: 绿色
		/// \n NR_LED_EFFECT_COLOR_BLUE: 蓝色
		/// \n NR_LED_EFFECT_COLOR_CYAN: 青色
		/// \n NR_LED_EFFECT_COLOR_MAGENT: 铁黑色或暗蓝靛色
		/// \n NR_LED_EFFECT_COLOR_YELLOW
		
	*/
NR_PLUGIN_ENUM32(NRLedEffectColor) {
    NR_LED_EFFECT_COLOR_UNKNOWN = 0,
    NR_LED_EFFECT_COLOR_RED = 1,
    NR_LED_EFFECT_COLOR_GREEN = 2,
    NR_LED_EFFECT_COLOR_BLUE = 3,
    NR_LED_EFFECT_COLOR_CYAN = 4,
    NR_LED_EFFECT_COLOR_MAGENT = 5,
    NR_LED_EFFECT_COLOR_YELLOW = 6,
};

	/*
		
		/// @brief Led 的 灯的效果
		/// \n NR_LED_EFFECT_TYPE_TURN_OFF: 关灯
		/// \n NR_LED_EFFECT_TYPE_RAINBOW : 彩虹效果
		/// \n NR_LED_EFFECT_TYPE_COLOR_CYCLE : 色彩循环
		/// \n NR_LED_EFFECT_TYPE_STATIC_COLOR : 静态颜色
		/// \n NR_LED_EFFECT_TYPE_BREATHING : 呼吸效果
		/// \n NR_LED_EFFECT_TYPE_STROBING : 闪烁效果
		
	*/
NR_PLUGIN_ENUM32(NRLedEffectType) {
    NR_LED_EFFECT_TYPE_TURN_OFF = 0,
    NR_LED_EFFECT_TYPE_RAINBOW = 1,
    NR_LED_EFFECT_TYPE_COLOR_CYCLE = 2,
    NR_LED_EFFECT_TYPE_STATIC_COLOR = 3,
    NR_LED_EFFECT_TYPE_BREATHING = 4,
    NR_LED_EFFECT_TYPE_STROBING = 5,
};

	/*
		屏幕显示模式
		0: 普通模式
		1: 交错模式，需要隔行扫描
		
	*/
NR_PLUGIN_ENUM32(NRDisplayPresentMode) {
    NR_DISPLAY_PRESENT_MODE_NORMAL = 0,
    NR_DISPLAY_PRESENT_MODE_INTERLANCED = 1,
};

	/*
		cpu warp模式
		0: none 不warp，直接内存拷贝
		1: cpu warp 需要warp
		
	*/
NR_PLUGIN_ENUM32(NRQuadWarpMode) {
    NR_QUAD_WARP_MODE_NONE = 0,
    NR_QUAD_WARP_MODE_CPU_WARP = 1,
};


NR_PLUGIN_ENUM(NRScreenAspectRatio) {
    NR_SCREEN_ASPECT_RATIO_UNKNOWN = 0,
    NR_SCREEN_ASPECT_RATIO_16_9 = 1,
    NR_SCREEN_ASPECT_RATIO_32_9,
    NR_SCREEN_ASPECT_RATIO_21_9,
    NR_SCREEN_ASPECT_RATIO_16_10,
    NR_SCREEN_ASPECT_RATIO_32_10,
    NR_SCREEN_ASPECT_RATIO_21_10,
    NR_SCREEN_ASPECT_RATIO_16_18,
    NR_SCREEN_ASPECT_RATIO_4_3,
    NR_SCREEN_ASPECT_RATIO_5_4,
    NR_SCREEN_ASPECT_RATIO_9_16,
};

	/*
		
		/// @brief 场景设置类型
		
	*/
NR_PLUGIN_ENUM32(NRCustomModeType) {
    NR_CUSTOM_MODE_TYPE_UNKNOWN = 0,
    NR_CUSTOM_MODE_TYPE_1 = 1,
    NR_CUSTOM_MODE_TYPE_2 = 2,
    NR_CUSTOM_MODE_TYPE_3 = 3,
};


NR_PLUGIN_ENUM32(NRDpStereoMode) {
    NR_DP_STEREO_MODE_NONE = 0,
    NR_DP_STEREO_MODE_FULL = 1,
    NR_DP_STEREO_MODE_HALF = 2,
};


NR_PLUGIN_ENUM(NREcAdjustMode) {
    NR_EC_ADJUST_MODE_UNKNOWN = 0,
    NR_EC_ADJUST_MODE_NORMAL = 1,
    NR_EC_ADJUST_MODE_AUTO,
};


NR_PLUGIN_ENUM(NRMediaStorageType) {
    NR_MEDIA_STORAGE_TYPE_UNKNOWN = 0,
    NR_MEDIA_STORAGE_TYPE_ALL = 1,
    NR_MEDIA_STORAGE_TYPE_LEFT,
    NR_MEDIA_STORAGE_TYPE_USED,
};

#pragma pack(1)

typedef struct NRDisplayTimingInfo {
    union {
        struct {
            uint64_t hardware_display_interval;
            uint64_t last_frame_time;
            uint64_t next_frame_time;
            uint64_t cur_time;
        };
        uint8_t padding[64];
    };

} NRDisplayTimingInfo;


typedef struct NRCameraDistortion {
    NRCameraModel camera_model;
    float camera_distortion[16];

} NRCameraDistortion;


typedef struct NRGUID {
    uint64_t high;
    uint64_t low;

} NRGUID;


typedef struct NRPluginMessage {
    uint64_t plugin_category;
    const char * plugin_description_data;
    uint32_t plugin_description_len;
    const void * plugin_message_data;
    uint32_t plugin_message_len;

} NRPluginMessage;


typedef struct NRFrameBufferAllocateInfo {
    union {
        struct {
            uint32_t width;
            uint32_t height;
            NRFrameBufferFormat format;
        };
        uint8_t padding[96];
    };

} NRFrameBufferAllocateInfo;


typedef struct NRFrameBuffer {
    union {
        struct {
            void* left_buffer;
            void* right_buffer;
        };
        uint8_t padding[48];
    };

} NRFrameBuffer;


typedef struct NRFrameBufferQueue {
    union {
        struct {
            uint32_t buffer_count;
            void** buffer_queue;
        };
        uint8_t padding[96];
    };

} NRFrameBufferQueue;

	/*
		
		/// @brief 眼镜分辨率/刷新率
		
	*/
typedef struct NRResolutionInfo {
    union {
        struct {
            int32_t width;
            int32_t height;
            int32_t refresh_rate;
        };
        uint8_t padding[32];
    };

} NRResolutionInfo;


typedef struct NRDisplayInfo {
    NRSize2i resolution;
    uint32_t refresh_rate;
    NRDisplay2D3DMode mode;

} NRDisplayInfo;


typedef struct NRDevicePose {
    union {
        struct {
            NRTransform transform;
            uint64_t hmd_time_nanos_system;
            NRTrackingReason tracking_reason;
            NRVector3f linear_velocity;
            NRVector3f angular_velocity;
            NRVector3f acc_bias;
            NRVector3f gyro_bias;
            uint64_t hmd_time_nanos_device;
            NRVector3f linear_accl;
        };
        uint8_t padding[128];
    };

} NRDevicePose;

	/*
		
		width: 图片宽
		height: 图片高
		format: 图片格式,目前只支持NR_IMAGE_FORMAT_BGRA_8888_packed,即内存排布顺序是B,G,R,A,B,G,R,A,....(地址由低到高)
		image_data: 图片数据吗，若为nullptr，表示不指定图标，使用默认值。
		image_data_size: 图片数据大小,单位字节
		
	*/
typedef struct NROsdImage {
    uint32_t width;
    uint32_t height;
    NRImageFormat format;
    const uint8_t * image_data;
    uint32_t image_data_size;

} NROsdImage;

	/*
		
		popup_type: 弹窗类型
		title: 标题字符串，若没有则为nullptr
		title_size: 标题字符串的大小
		content: 内容字符串，不能为nullptr
		content_size: 内容字符串的大小
		back_ground_color: 窗口背景颜色,RGB格式，0x00RRGGBB。-1表示不指定背景颜色，使用默认值。
		icon: 通知图标。
		click_to_hide: 是否支持单击隐藏
		time_out_ms: 超时隐藏时间,单位毫秒,-1表示不超时隐藏，0:表示使用默认值15000ms
		option_1: button1文案，默认选中，不能为nullptr
		option_1_size: button1文案的大小
		option_2: button2文案，默认选中，若没有则为nullptr
		option_2_size: button2文案的大小
		option_3: button3文案，默认选中，若没有则为nullptr
		option_3_size: button3文案的大小
		
	*/
typedef struct NROsdPopupRequest {
    NROsdPopupType popup_type;
    const char * title;
    uint32_t title_size;
    const char * content;
    uint32_t content_size;
    int32_t back_ground_color;
    NROsdImage icon;
    bool click_to_hide;
    int32_t time_out_ms;
    const char * option_1;
    uint32_t option_1_size;
    const char * option_2;
    uint32_t option_2_size;
    const char * option_3;
    uint32_t option_3_size;

} NROsdPopupRequest;

	/*
		
		result: 弹窗结果,-1表示未操作，超时隐藏了。0表示点击了X键消失(普通弹窗会有, 1表示点击了button1,2表示点击了button2,以此类推 
		
	*/
typedef struct NROsdPopupResponse {
    int32_t result;

} NROsdPopupResponse;

	/*
		
		/// @brief Sensor 的 信息
		/// \n  vendor_name: 传感器厂商名称
		/// \n  range: 传感器范围
		/// \n  resolution: 传感器分辨率
		/// \n  min_sampling_period_us: 传感器最小采样周期 ( 0 表示变化了才上报 )
		/// \n  max_sampling_period_us: 传感器最大采样周期 ( 0 表示变化了才上报  )
		
	*/
typedef struct NRSensorCapabilityInfo {
    union {
        struct {
            char vendor_name[128];
            float range;
            float resolution;
            int32_t min_sampling_period_us;
            int32_t max_sampling_period_us;
        };
        uint8_t padding[192];
    };

} NRSensorCapabilityInfo;


typedef struct NRDpFrameData {
    union {
        struct {
            uint32_t frame_id;
            uint32_t width;
            uint32_t height;
            NRFrameBufferFormat pixel_format;
            NRVector3u64 data;
            NRVector3u64 data_ext;
            NRVector3u64 header;
            NRVector3u64 header_ext;
            NRVector3u strides;
            uint64_t pts;
            uint64_t ar_frame_handle;
        };
        uint8_t padding[256];
    };

} NRDpFrameData;

	/*
		
		CPU warp for quad 的初始化数据
		input_resolution: 屏幕的物理分辨率，目前可能的值是1920x1080, 1920x1200
		output_resolution: 屏幕的逻辑分辨率，目前可能的值是1920x1080, 1920x1200, 1920x540
		quad_distance: 虚拟屏在center camera坐标系下的深度值，单位是米。
		input_roi:  输入图的有效区域，基于屏幕的物理分辨率
		
	*/
typedef struct NRQuadWarpInitData {
    union {
        struct {
            NRSize2i input_resolution;
            NRSize2i output_resolution;
            float quad_distance;
            NRRectf input_roi;
            NRQuadWarpMode warp_mode;
        };
        uint8_t padding[256];
    };

} NRQuadWarpInitData;

#pragma pack()

#endif // NRAPP