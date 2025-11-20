#pragma once

#include "nr_plugin_types.h"

#ifdef NRAPP

#include "nr_plugin_glasses_types.inc"

#else

	/*
		
		/// @brief 眼镜音频算法 参考文档：https://xreal.feishu.cn/wiki/AAPIw6IFwir413k3RincYdt0nJe
		/// - 模式1：关闭上行音频算法（场景1、2和3上行算法），打开下行音效算法（默认状态）；
		/// - 模式2：关闭上/下行音频算法（即关闭上下行音频算法）；
		/// - 模式3：打开通话/近场语音算法和下行音效算法；
		/// - 模式4：打开远场语音算法和下行音效算法；
		/// - 模式5：打开立体声录音算法和下行音效算法；
		/// - 模式6：关闭所有音频算法，并使能右腿MIC1和MIC2采集音频数据，将MIC1和MIC2分别放在左右声道给到SoC，SoC通过UAC上传左右声道数据。
		/// - 模式7：关闭所有音频算法，并使能左腿MIC3和MIC4采集音频数据，将MIC3和MIC4分别放在左右声道给到SoC，SoC通过UAC上传左右声道数据。
		/// - 模式8：轻语模式/私密模式，本质就是声音变小了。和模式 1 都可以是默认。
		/// 模式 2 6 7 一般不用
		
	*/
NR_PLUGIN_ENUM32(NRAudioAlgorithmType) {
    NR_AUDIO_ALGORITHM_TYPE_NO_PICKUP = 1,
    NR_AUDIO_ALGORITHM_TYPE_NEAR_FIELD_VOICE_PICKUP = 3,
    NR_AUDIO_ALGORITHM_TYPE_FAR_FIELD_VOICE_PICKUP = 4,
    NR_AUDIO_ALGORITHM_TYPE_NEAR_FAR_FIELD_VOICE_PICKUP = 5,
    NR_AUDIO_ALGORITHM_TYPE_WHISPER = 8,
};

	/*
		
		/// @brief 眼睛音频通道
		/// \n NR_AUDIO_CHANNEL_INVALID 无效
		/// \n NR_AUDIO_CHANNEL_IN_MONO 单声道
		/// \n NR_AUDIO_CHANNEL_IN_STEORO 双声道
		
	*/
NR_PLUGIN_ENUM32(NRAudioChannelIn) {
    NR_AUDIO_CHANNEL_UNKNOWN = 0,
    NR_AUDIO_CHANNEL_IN_MONO = 0x01,
    NR_AUDIO_CHANNEL_IN_STEORO = 0x02,
};

	/*
		
		/// @brief 眼镜音频格式
		/// \n NR_AUDIO_FORMAT_PCM_16BIT 16bit pcm 格式
		/// \n NR_AUDIO_FORMAT_PCM_32BIT 32bit pcm 格式
		/// \n NR_AUDIO_FORMAT_PCM_FLOAT float pcm 格式
		
	*/
NR_PLUGIN_ENUM8(NRAudioFormat) {
    NR_AUDIO_FORMAT_UNKNOWN = 0,
    NR_AUDIO_FORMAT_PCM_16BIT = 1,
    NR_AUDIO_FORMAT_PCM_32BIT = 2,
    NR_AUDIO_FORMAT_PCM_FLOAT = 3,
};

	/*
		
		/// @brief 眼镜音频模式
		/// \n NR_AUDIO_MODE_UAC UAC模式
		/// \n NR_AUDIO_MODE_DP DP模式
		
	*/
NR_PLUGIN_ENUM32(NRAudioMode) {
    NR_AUDIO_MODE_UAC = 0,
    NR_AUDIO_MODE_DP = 1,
};

	/*
		
		/// @brief 播音类型
		/// \n NRAudioPlayTypeDefault 默认播音类型(跟随 Host 音量)
		/// \n NRAudioPlayTypeUseVolume 使用指定音量播音（恒定音量大小，不受 Host 音量大小控制）
		
	*/
NR_PLUGIN_ENUM32(NRAudioPlayType) {
    NR_AUDIO_PLAY_TYPE_DEFAULT = 0,
    NR_AUDIO_PLAY_TYPE_USE_VOLUME = 1,
};

	/*
		
		/// @brief 眼镜音频采样率
		
	*/
NR_PLUGIN_ENUM8(NRAudioSampleRate) {
    NR_AUDIO_SAMPLE_RATE_UNKNOWN = 0,
    NR_AUDIO_SAMPLE_RATE_44100 = 1,
    NR_AUDIO_SAMPLE_RATE_48000 = 2,
    NR_AUDIO_SAMPLE_RATE_96000 = 3,
};

	/*
		
		/// @brief 左右声道
		/// \n NR_AUDIO_USAGE_LEFT: 左声道
		/// \n NR_AUDIO_USAGE_RIGHT: 右声道
		
	*/
NR_PLUGIN_ENUM32(NRAudioUsage) {
    NR_AUDIO_USAGE_LEFT = 0,
    NR_AUDIO_USAGE_RIGHT,
};

	/*
		
		/// @brief 加密方式
		/// \n NR_CRYPTO_TYPE_AES_128_CBC
		/// \n NR_CRYPTO_TYPE_AES_256_CBC
		
	*/
NR_PLUGIN_ENUM32(NRCryptoType) {
    NR_CRYPTO_TYPE_UNKNOWN = 0,
    NR_CRYPTO_TYPE_AES_128_CBC = 1,
    NR_CRYPTO_TYPE_AES_256_CBC = 2,
};

	/*

	*/
NR_PLUGIN_ENUM32(NRDisplayType) {
    NR_DISPLAY_TYPE_UNKNOWN = 0,
    NR_DISPLAY_TYPE_348 = 1,
    NR_DISPLAY_TYPE_343E = 2,
    NR_DISPLAY_TYPE_343F = 3,
};

	/*
		
		/// @brief dpisp往内存写入dp数据时的frame模式
		/// \n NR_DP_BUFFER_MODE_SINGLE 内存中只有一个dp buffer， NRDpGetFrame时拿到的数据指针不变
		/// \n NR_DP_FRAME_MODE_MULTIPLE 内存中有多个dp buffer， NRDpGetFrame时拿到的数据指针会变化
		
	*/
NR_PLUGIN_ENUM32(NRDpBufferMode) {
    NR_DP_BUFFER_MODE_UNKNOWN = 0,
    NR_DP_BUFFER_MODE_SINGLE = 1,
    NR_DP_BUFFER_MODE_MULTIPLE = 2,
};

	/*
		
		/// @brief DP 数据传输模式
		/// \n NR_DP_DATA_TRANSMIT_MODE_NORMAL 普通模式 使用 GDC 上屏
		/// \n NR_DP_DATA_TRANSMIT_MODE_DIRECT 直通模式 sdk 用 singlebuffer 时使用，目前没有用
		
	*/
NR_PLUGIN_ENUM32(NRDpDataTransmitMode) {
    NR_DP_DATA_TRANSMIT_MODE_NORMAL = 0,
    NR_DP_DATA_TRANSMIT_MODE_DIRECT = 1,
};

	/*

	*/
NR_PLUGIN_ENUM32(NRDpLaneRateMode) {
    NR_DP_LANE_RATE_MODE_UNKNOWN = 0,
    NR_DP_LANE_RATE_MODE_1_62 = 1,
    NR_DP_LANE_RATE_MODE_2_7 = 2,
};

	/*
		
		/// @brief NRDpGetFrame返回时机的模式
		/// \n NR_DP_SUBMIT_TYPE_VSYNC NRDpGetFrame在vsync的时候返回
		/// \n NR_DP_SUBMIT_TYPE_INTERVAL_LINE NRDpGetFrame在当前帧写入指定行数的时候返回
		/// \n NR_DP_SUBMIT_TYPE_FRAME_DONE NRDpGetFrame在当前帧全部写完的时候返回
		
	*/
NR_PLUGIN_ENUM32(NRDpSubmitType) {
    NR_DP_SUBMIT_TYPE_UNKNOWN = 0,
    NR_DP_SUBMIT_TYPE_VSYNC = 1,
    NR_DP_SUBMIT_TYPE_INTERVAL_LINE = 2,
    NR_DP_SUBMIT_TYPE_FRAME_DONE = 3,
};

	/*
		
		/// @brief dp 工作模式
		/// \n NR_DP_WORKING_MODE_NORMAL 普通模式，兼容性最好
		/// \n NR_DP_WORKING_MODE_POWER_SAVE 低功耗模式，省电
		
	*/
NR_PLUGIN_ENUM32(NRDpWorkingMode) {
    NR_DP_WORKING_MODE_NORMAL = 0,
    NR_DP_WORKING_MODE_POWER_SAVE = 1,
};

	/*
		
		/// @brief DP 工作状态
		/// \n NR_DP_WORKING_STATE_UNKNOWN 未知
		/// \n NR_DP_WORKING_STATE_OK 正常
		/// \n NR_DP_WORKING_STATE_ERROR 错误
		/// \n NR_DP_WORKING_STATE_INIT 初始化
		
	*/
NR_PLUGIN_ENUM32(NRDpWorkingState) {
    NR_DP_WORKING_STATE_UNKNOWN = 0,
    NR_DP_WORKING_STATE_OK = 1,
    NR_DP_WORKING_STATE_ERROR = 2,
    NR_DP_WORKING_STATE_INIT = 3,
    NR_DP_WORKING_STATE_START_SWITCH = 4,
};

	/*
		
		/// @brief 按键状态
		
	*/
NR_PLUGIN_ENUM32(NRKeyState) {
    NR_KEY_STATE_UNKNOWN = 0,
    NR_KEY_STATE_BUTTON_DOWN = 1,
    NR_KEY_STATE_BUTTON_UP = 2,
};

	/*
		
		/// @brief Led 的ID
		/// NR_LED_ID_NONE: 无效或者出错的时候使用
		
	*/
NR_PLUGIN_ENUM32(NRLedID) {
    NR_LED_ID_UNKNOWN = -1,
    NR_LED_ID_0 = 0,
    NR_LED_ID_1 = 1,
    NR_LED_ID_2 = 2,
    NR_LED_ID_3 = 3,
    NR_LED_ID_DEFAULT = 0,
    NR_LED_ID_RGB = 0,
};

	/*
		
		/// @brief 眼镜类型，用于区分不同的眼镜，SDK sparrow 叫 device_type
		/// \n NR_MISC_DEVICE_TYPE_UNKNOWN 未知眼镜
		/// \n NR_MISC_DEVICE_TYPE_XREAL_ONE: 内部 叫 GF 眼镜
		/// \n NR_MISC_DEVICE_TYPE_XREAL_ONE_PRO_M: 内部 叫 Gina 眼镜 小瞳距版本
		/// \n NR_MISC_DEVICE_TYPE_XREAL_ONE_PRO_L : 内部 叫 Gina 眼镜 大瞳距版本
		/// \n NR_MISC_DEVICE_TYPE_XREAL_AURA_M: 内部 叫 Hylla 眼镜 小瞳距版本
		/// \n NR_MISC_DEVICE_TYPE_XREAL_AURA_L : 内部 叫 Hylla 眼镜 大瞳距版本
		/// \n NR_MISC_DEVICE_TYPE_XREAL_1S : 内部 叫 GS 眼镜 是GF的改款
		/// \n NR_MISC_DEVICE_TYPE_VIDDA_ONE_PRO: 内部 叫 海信 眼镜
		/// \n NR_MISC_DEVICE_TYPE_ROG_XREAL_R1_M: 内部 叫 Glory 眼镜 或者 华硕/ROG定制眼镜 小瞳距版本
		/// \n NR_MISC_DEVICE_TYPE_ROG_XREAL_R1_L: 内部 叫 Glory 眼镜 或者 华硕/ROG定制眼镜 大瞳距版本
		
	*/
NR_PLUGIN_ENUM32(NRMiscDeviceType) {
    NR_MISC_DEVICE_TYPE_UNKNOWN = 0,
    NR_MISC_DEVICE_TYPE_XREAL_ONE = 10,
    NR_MISC_DEVICE_TYPE_XREAL_ONE_PRO_M = 11,
    NR_MISC_DEVICE_TYPE_XREAL_ONE_PRO_L = 12,
    NR_MISC_DEVICE_TYPE_XREAL_AURA_M = 13,
    NR_MISC_DEVICE_TYPE_XREAL_AURA_L = 14,
    NR_MISC_DEVICE_TYPE_XREAL_1S = 15,
    NR_MISC_DEVICE_TYPE_VIDDA_ONE_PRO = 1002,
    NR_MISC_DEVICE_TYPE_ROG_XREAL_R1_M = 1003,
    NR_MISC_DEVICE_TYPE_ROG_XREAL_R1_L = 1004,
};

	/*
		
		/// @brief 获取眼镜连接的 Host 类型
		/// \n NR_MISC_HOST_TYPE_UNKNOWN 未知类型
		/// \n NR_MISC_HOST_TYPE_XREAL_HUB_WITH_SWITCH XReal Hub with Switch 通过Xreal Hub 连接 Switch设备
		/// \n NR_MISC_HOST_TYPE_XREAL_AURA google 设计的 PUCK / Halo 盒子 / Android XR
		/// \n NR_MISC_HOST_TYPE_XREAL_NEO 内部交core项目，为了连接switch2 的类似充电宝的产品
		
	*/
NR_PLUGIN_ENUM32(NRMiscHostType) {
    NR_MISC_HOST_TYPE_UNKNOWN = 0,
    NR_MISC_HOST_TYPE_XREAL_HUB_WITH_SWITCH = 1,
    NR_MISC_HOST_TYPE_XREAL_AURA = 2,
    NR_MISC_HOST_TYPE_XREAL_NEO = 3,
};

	/*
		
		/// @brief 线程的调度策略
		/// \n NR_MISC_SCHED_POLICY_FIFO 先进先出
		/// \n NR_MISC_SCHED_POLICY_RR 轮转
		
	*/
NR_PLUGIN_ENUM32(NRMiscSchedPolicy) {
    NR_MISC_SCHED_POLICY_NORMAL = 0,
    NR_MISC_SCHED_POLICY_FIFO = 1,
};

	/*
		
		/// @brief 眼镜当前的升级状态
		/// \n NR_MISC_SYSTEM_UPGRADE_STATE_NOT_RUNNING 升级未运行
		/// \n NR_MISC_SYSTEM_UPGRADE_STATE_RUNNING 升级运行中
		/// \n NR_MISC_SYSTEM_UPGRADE_STATE_START 升级开始
		/// \n NR_MISC_SYSTEM_UPGRADE_STATE_END 升级结束
		
	*/
NR_PLUGIN_ENUM32(NRMiscSystemUpgradeState) {
    NR_MISC_SYSTEM_UPGRADE_STATE_NOT_RUNNING = 0,
    NR_MISC_SYSTEM_UPGRADE_STATE_RUNNING = 1,
    NR_MISC_SYSTEM_UPGRADE_STATE_START = 2,
    NR_MISC_SYSTEM_UPGRADE_STATE_END = 3,
};

	/*
		
		/// @brief CPU 频率模式
		/// \n NR_POWER_CPU_FREQUENCY_MODE_UNKNOWN 未知
		/// \n NR_POWER_CPU_FREQUENCY_MODE_LOW 低
		/// \n NR_POWER_CPU_FREQUENCY_MODE_MIDDLE 中
		/// \n NR_POWER_CPU_FREQUENCY_MODE_HIGH 高
		/// \n NR_POWER_CPU_FREQUENCY_MODE_OVER 超高
		/// \n NR_POWER_CPU_FREQUENCY_MODE_NORMAL 正常 同 NR_POWER_CPU_FREQUENCY_MODE_MIDDLE
		/// \n NR_POWER_CPU_FREQUENCY_MODE_PERFORMANCE 性能 同 NR_POWER_CPU_FREQUENCY_MODE_OVER
		
	*/
NR_PLUGIN_ENUM32(NRPowerCpuFrequencyMode) {
    NR_POWER_CPU_FREQUENCY_MODE_UNKNOWN = 0,
    NR_POWER_CPU_FREQUENCY_MODE_LOW = 3,
    NR_POWER_CPU_FREQUENCY_MODE_MIDDLE = 1,
    NR_POWER_CPU_FREQUENCY_MODE_NORMAL = 1,
    NR_POWER_CPU_FREQUENCY_MODE_HIGH = 4,
    NR_POWER_CPU_FREQUENCY_MODE_OVER = 2,
    NR_POWER_CPU_FREQUENCY_MODE_PERFORMANCE = 2,
};

	/*
		
		/// @brief Proximity Sensor Chip Id
		/// NR_PROXIMITY_CHIP_ID_UNKNOWN: 未知
		/// NR_PROXIMITY_CHIP_ID_IQS323: IQS323 芯片
		/// NR_PROXIMITY_CHIP_ID_SX920X: SX920X 芯片
		
	*/
NR_PLUGIN_ENUM32(NRProximityChipId) {
    NR_PROXIMITY_CHIP_ID_UNKNOWN = 0,
    NR_PROXIMITY_CHIP_ID_IQS323 = 1,
    NR_PROXIMITY_CHIP_ID_SX920X = 2,
};

	/*
		
		/// @brief Proximity Sensor Data Channel
		
	*/
NR_PLUGIN_ENUM32(NRProximityDataChannel) {
    NR_PROXIMITY_DATA_CHANNEL_0 = 0,
    NR_PROXIMITY_DATA_CHANNEL_1 = 1,
    NR_PROXIMITY_DATA_CHANNEL_2 = 2,
    NR_PROXIMITY_DATA_CHANNEL_3 = 3,
    NR_PROXIMITY_DATA_CHANNEL_4 = 4,
    NR_PROXIMITY_DATA_CHANNEL_5 = 5,
    NR_PROXIMITY_DATA_CHANNEL_6 = 6,
    NR_PROXIMITY_DATA_CHANNEL_7 = 7,
};

	/*
		
		/// @brief 佩戴眼镜状态
		/// NR_PROXIMITY_WEARING_STATE_UNKNOWN: 未知
		/// NR_PROXIMITY_WEARING_STATE_WEARING: 佩戴
		/// NR_PROXIMITY_WEARING_STATE_NO_WEARING: 未佩戴
		
	*/
NR_PLUGIN_ENUM32(NRProximityWearingState) {
    NR_PROXIMITY_WEARING_STATE_UNKNOWN = 0,
    NR_PROXIMITY_WEARING_STATE_WEARING = 1,
    NR_PROXIMITY_WEARING_STATE_NO_WEARING = 2,
};

	/*
		
		/// @brief RGB Camera 插入/拔出状态
		
	*/
NR_PLUGIN_ENUM32(NRRgbCameraPluginState) {
    NR_RGB_CAMERA_PLUGIN_STATE_UNKNOWN = 0,
    NR_RGB_CAMERA_PLUGIN_STATE_PLUGIN = 1,
    NR_RGB_CAMERA_PLUGIN_STATE_PLUGOUT = 2,
};

	/*
		
		/// @brief 磁盘的对外传输模式，
		/// \n NR_STORAGE_MODE_NONE : 无模式，默认模式，无法传输文件
		/// \n NR_STORAGE_MODE_MTP : MTP模式
		/// \n NR_STORAGE_MODE_UMS : UMS模式
		
	*/
NR_PLUGIN_ENUM32(NRStorageMode) {
    NR_STORAGE_MODE_NONE = 0,
    NR_STORAGE_MODE_MTP = 1,
    NR_STORAGE_MODE_UMS = 2,
};

	/*
		
		/// @brief 温度传感器的ID
		/// NR_TEMPERATURE_ID_NONE: 无效或者出错的时候使用
		/// NR_TEMPERATURE_ID_0: SOC 温度
		/// NR_TEMPERATURE_ID_1: NTC 0
		/// NR_TEMPERATURE_ID_2: NTC 1
		
	*/
NR_PLUGIN_ENUM32(NRTemperatureID) {
    NR_TEMPERATURE_ID_NONE = -1,
    NR_TEMPERATURE_ID_0 = 0,
    NR_TEMPERATURE_ID_1 = 1,
    NR_TEMPERATURE_ID_2 = 2,
    NR_TEMPERATURE_ID_3 = 3,
};

	/*
		
		/// @brief 功能
		/// @note 1000-1999 为保留值 for debug
		/// @note 2000-2999 为保留值 for sparrow
		
	*/
NR_PLUGIN_ENUM32(NRAction) {
    NR_ACTION_UNKNOWN = 0,
    NR_ACTION_CLICK = 1,
    NR_ACTION_DOUBLE_CLICK = 2,
    NR_ACTION_LONG_PRESS = 3,
    NR_ACTION_OPEN_SCREEN = 4,
    NR_ACTION_CLOSE_SCREEN = 5,
    NR_ACTION_INCREASE_BRIGHTNESS = 6,
    NR_ACTION_DECREASE_BRIGHTNESS = 7,
    NR_ACTION_INCREASE_VOLUME = 8,
    NR_ACTION_DECREASE_VOLUME = 9,
    NR_ACTION_SWITCH_TO_MONO = 10,
    NR_ACTION_SWITCH_TO_STEREO = 11,
    NR_ACTION_NEXT_EC_LEVEL = 12,
    NR_ACTION_SWITCH_TO_DP_VOICE = 13,
    NR_ACTION_SWITCH_TO_UAC_VOICE = 14,
    NR_ACTION_RESERVED0 = 15,
    NR_ACTION_RESERVED1 = 16,
    NR_ACTION_RESERVED2 = 17,
    NR_ACTION_RESERVED3 = 18,
    NR_ACTION_RESERVED4 = 19,
    NR_ACTION_SWITCH_SLEEP_TIME_LEVEL = 30,
    NR_ACTION_SWITCH_DISPLAY_COLOR_CALIBRATION = 31,
    NR_ACTION_STARTUP_STATE = 32,
    NR_ACTION_TRIGGER_SWITCH_SPACE_MODE = 33,
    NR_ACTION_TRIGGER_RECENTER = 34,
    NR_ACTION_TRIGGER_OSD_MAIN_MENU = 35,
    NR_ACTION_TRIGGER_TAKE_PHOTO = 36,
    NR_ACTION_TRIGGER_TAKE_VIDEO = 37,
    NR_ACTION_RESERVED5 = 1000,
    NR_ACTION_DISCONNECT = 2000,
    NR_ACTION_FORCE_QUIT = 2001,
};

	/*
		
		/// @brief 眼镜屏幕颜色校准方式
		/// \n NR_DISPLAY_COLOR_CALIBRATION_TYPE_NONE 不校准
		/// \n NR_DISPLAY_COLOR_CALIBRATION_TYPE_CALIBRATION 索尼或光学校准方案（光学优先）
		
	*/
NR_PLUGIN_ENUM32(NRDisplayColorCalibrationType) {
    NR_DISPLAY_COLOR_CALIBRATION_TYPE_NONE = 0,
    NR_DISPLAY_COLOR_CALIBRATION_TYPE_CALIBRATION = 1,
};

	/*
		
		/// @brief 眼镜屏幕开关方式
		/// \n NR_DISPLAY_SCREEN_ENABLE_METHOD_DIRECT 直接打开或关闭
		/// \n NR_DISPLAY_SCREEN_ENABLE_METHOD_STEP_BY_STEP 逐步打开或关闭, 暂不支持
		
	*/
NR_PLUGIN_ENUM32(NRDisplayScreenEnableMethod) {
    NR_DISPLAY_SCREEN_ENABLE_METHOD_DIRECT = 0,
    NR_DISPLAY_SCREEN_ENABLE_METHOD_STEP_BY_STEP = 1,
};

	/*
		
		/// @brief dp 输入模式
		/// \n NR_DP_INPUT_MODE_MONO 单目
		/// \n NR_DP_INPUT_MODE_STEREO 双目
		/// \n NR_DP_INPUT_MODE_SYNTH_STEREO 单目转双目
		
	*/
NR_PLUGIN_ENUM32(NRDpInputMode) {
    NR_DP_INPUT_MODE_MONO = 0,
    NR_DP_INPUT_MODE_STEREO = 1,
    NR_DP_INPUT_MODE_SYNTH_STEREO = 2,
};

	/*
		
		/// @brief 场景模式： 0: 空间屏幕 1: 星空模式(host端跑了 SDK)
		/// \n air-like: 
		/// \n light:
		/// @return 结果
		
	*/
NR_PLUGIN_ENUM32(NRGlassesSceneMode) {
    NR_GLASSES_SCENE_MODE_SPACE_SCREEN = 0,
    NR_GLASSES_SCENE_MODE_SDK_RENDER = 1,
};

	/*
		
		/// @brief 获取 空间模式 0 hover 1 follow
		/// \n air-like: 
		/// \n light:
		/// @return 结果
		
	*/
NR_PLUGIN_ENUM32(NRGlassesSpaceMode) {
    NR_GLASSES_SPACE_MODE_HOVER = 0,
    NR_GLASSES_SPACE_MODE_FOLLOW = 1,
};

	/*
		
		/// @brief 获取眼镜的开机流程状态
		/// \n air-like: 
		/// \n light:
		/// @return 结果
		
	*/
NR_PLUGIN_ENUM32(NRGlassesStartupState) {
    NR_GLASSES_STARTUP_STATE_PROCESSING = 0,
    NR_GLASSES_STARTUP_STATE_END = 1,
};

	/*
		
		/// @brief 支持的设备
		
	*/
NR_PLUGIN_ENUM64(NRGlassesSupportedDevices) {
    NR_GLASSES_SUPPORTED_DEVICES_IMU_1 = 0x0001,
    NR_GLASSES_SUPPORTED_DEVICES_GRAYCAMERA_1 = 0x0002,
    NR_GLASSES_SUPPORTED_DEVICES_GRAYCAMERA_2 = 0x0004,
    NR_GLASSES_SUPPORTED_DEVICES_GRAYCAMERA_3 = 0x0008,
    NR_GLASSES_SUPPORTED_DEVICES_GRAYCAMERA_4 = 0x0010,
    NR_GLASSES_SUPPORTED_DEVICES_RGBCAMERA_1 = 0x0020,
    NR_GLASSES_SUPPORTED_DEVICES_RGBCAMERA_2 = 0x0040,
    NR_GLASSES_SUPPORTED_DEVICES_RGBCAMERA_3 = 0x0080,
    NR_GLASSES_SUPPORTED_DEVICES_RGBCAMERA_4 = 0x0100,
    NR_GLASSES_SUPPORTED_DEVICES_MAGNETIC = 0x0200,
    NR_GLASSES_SUPPORTED_DEVICES_AMBIENT_LIGHT = 0x0400,
    NR_GLASSES_SUPPORTED_DEVICES_IMU_2 = 0x0800,
    NR_GLASSES_SUPPORTED_DEVICES_IMU_3 = 0x1000,
    NR_GLASSES_SUPPORTED_DEVICES_IMU_4 = 0x2000,
};

	/*
		
		/// @brief PowerSave状态
		
	*/
NR_PLUGIN_ENUM32(NRPowerSaveState) {
    NR_POWER_SAVE_STATE_ENTER = 1,
};

	/*
		
		/// @brief 过温/高温状态
		
	*/
NR_PLUGIN_ENUM32(NRTemperatureState) {
    NR_TEMPERATURE_STATE_NORMAL = 0,
    NR_TEMPERATURE_STATE_OVER = 1,
    NR_TEMPERATURE_STATE_HIGH = 2,
};

#pragma pack(1)
	/*
		
		/// @brief Audio In 数据
		/// \n data: AudioIn 的数据(虚拟地址)
		
	*/
typedef struct NRAudioInData {
    union {
        struct {
            const char * data_data;
            uint32_t data_size;
            uint64_t timestamp;
            NRAudioSampleRate sample_rate;
            NRAudioFormat format;
            NRAudioChannelIn channel;
        };
        uint8_t padding[48];
    };

} NRAudioInData;

	/*
		
		/// @brief dp配置
		/// \n buffer_mode dp frame的模式
		/// \n submit_line_count 只有在submit_type为NR_DP_SUBMIT_TYPE_INTERVAL_LINE时才有效，配置NRDpGetFrame在当前dp frame写入line_count行数据时返回
		
	*/
typedef struct NRDpConfig {
    union {
        struct {
            NRDpBufferMode buffer_mode;
            NRDpSubmitType submit_type;
            uint32_t submit_line_count;
        };
        uint8_t padding[128];
    };

} NRDpConfig;

	/*
		
		/// @brief display的配置
		/// \n dp_display_sync_line_count dp写到dp_display_sync_line_count行时，display启动开始上屏
		/// \n lines64_enable 是否需要64行的linebuffer，默认是32行的linebuffer
		/// \n display_resolution display的分辨率和刷新率
		
	*/
typedef struct NRDpDisplayConfig {
    union {
        struct {
            uint32_t dp_display_sync_line_count;
            uint32_t lines64_enable;
            NRResolution display_resolution;
        };
        uint8_t padding[128];
    };

} NRDpDisplayConfig;

	/*
		
		/// @brief 按键数据
		
	*/
typedef struct NRKeyStateData {
    union {
        struct {
            NRKeyType key_type;
            NRKeyState key_state;
            uint64_t hmd_time_nanos_device;
        };
        uint8_t padding[64];
    };

} NRKeyStateData;

	/*
		
		/// @brief Proximity Sensor Data
		
	*/
typedef struct NRProximityData {
    union {
        struct {
            int32_t data[8];
        };
        uint8_t padding[64];
    };

} NRProximityData;

	/*
		
		/// @brief 温度数据
		
	*/
typedef struct NRTemperatureData {
    union {
        struct {
            NRTemperatureID id;
            float value;
        };
        uint8_t padding[32];
    };

} NRTemperatureData;

	/*
		
		/// @brief 功能数据
		
	*/
typedef struct NRActionData {
    union {
        struct {
            NRAction action;
            uint32_t action_param;
            uint32_t action_param2;
            uint64_t hmd_time_nanos_device;
        };
        uint8_t padding[64];
    };

} NRActionData;

	/*

	*/
typedef struct NRDisplayDpResolutionMap {
    NRSize2i dp_resolution[32];
    NRSize2i display_resolution[32];

} NRDisplayDpResolutionMap;

	/*

	*/
typedef struct NRDisplayRoi {
    NRSize2i display_size;
    NRRectf left_oled_roi;
    NRRectf right_oled_roi;

} NRDisplayRoi;

	/*

	*/
typedef struct NRDisplayRoiArray {
    NRDisplayRoi display_rois[8];

} NRDisplayRoiArray;

	/*
		
		/// @brief 事件数据
		
	*/
typedef struct NREventReportData {
    union {
        struct {
            uint32_t category_id;
            uint32_t event_id;
            uint32_t time_offset;
            uint32_t info;
            uint32_t info2;
            const char * description_data;
            uint32_t description_size;
        };
        uint8_t padding[96];
    };

} NREventReportData;

#pragma pack()

	/*

	*/
typedef void (*NRAudioPlayCallback)(
        void * user_data
);

#endif // NRAPP