#pragma once

#include <cstdint>

#pragma pack(1)
struct hid_data_t {
    union {
        struct {
            /// Gyroscope data xyz (rad/sec)
            float gyro[3];
            /// Acceleration data xyz (m/s^2)
            float acc[3];
            /// Magnetometer data xyz
            float mag[3];
            /// Temperature of imu
            float temperature;
            /// Timestamp of sof (us)
            uint64_t timestamp;
        };
        uint8_t padding[512];
    };
    union {
        struct {
            uint32_t type; /// 
            uint32_t size; /// raw data size
            uint32_t version;
            uint32_t error_code; /// error code
            int64_t device_start_time;
            uint64_t sys_time_after_read_usb;
        };
        uint8_t meta[128];
    };
};

struct HidInfo{
    union {
        struct {
            int32_t usb_fd;
            int32_t use_callback;
            int16_t interface_number;
            bool is_imu;
        };
        uint8_t padding[48];
    };
};

struct ImuFrame {
    union {
        struct {
            uint64_t hmd_time_nanos;
            float gyro[3];
            float acc[3];
            float mag[3];
            float temperature;
			uint64_t timestamp_raw;
			uint8_t timer_bits;
			uint64_t hmd_hw_time_nanos;
            uint8_t mag_update_flag;
        };
        uint8_t padding[128];
    };
};

struct VsyncFrame {
    union {
        struct {
            uint64_t sequence;
			uint64_t hmd_hw_time_nanos;
            uint64_t hmd_time_nanos;
        };
        uint8_t padding[128];
    };
};

struct HidStatusData {
    enum HidStatusType{
        HID_INVALID = -1,
        HID_UNPLUG = 0,
        HID_PLUGIN = 1,
    };
    union {
        struct {
            int32_t status;
        };
        uint8_t padding[64];
    };
    uint32_t padding_size;    
};
#pragma pack()
