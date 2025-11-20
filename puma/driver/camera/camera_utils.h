#pragma once
#include <cstdint>
#include <cstddef>

/// height 241,
constexpr int32_t kFloraImageMinHeight = 241;
/// const variable declaration
/// height 480,
constexpr int32_t kImageHeight = 481;
/// width 640,
constexpr int32_t kImageWidth = 640;

/// height 480, width 640
constexpr int32_t kImageSize = kImageWidth * kImageHeight;
/// num of bytes an image has
constexpr int32_t kImageSizeInByte = kImageSize * sizeof(uint8_t);

/// minimum and maximum time interval of two adjacent camera frames(usec)
constexpr int32_t kMinCameraTimeInterval = 30000;
constexpr int32_t kMaxCameraTimeInterval = 36000;

/// maximum camera gain
constexpr int32_t kMaxGain = 15;

#pragma pack(1)

typedef struct UniversalFrameMetaData {
    uint32_t magic0;
    uint32_t frame_id;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint64_t timestamp;        // capture time
    uint64_t exposure_time_ns; // exposure duration
    uint64_t gain_value;       // gain
    uint32_t rolling_shutter;  // rolling shutter
    uint32_t version;          // struct version
    uint64_t timestamp_system; // timestamp + host_time_offset
    uint8_t sensor_index;      //left=0,right=1
    uint8_t reserved[63];
    uint32_t magic1;
} UNIVERSAL_META_DATA;

struct frame_t {
    union {
        struct {
            /// Image data for this frame
            union {
                uint8_t *data;
                uint64_t ptr_padding;
            };
            /// Size of image data buffer
            uint32_t data_bytes;
            /// Number of camera
            uint32_t width;
            /// Height of image in pixels
            uint32_t height;
            /// Number of bytes per horizontal line
            uint32_t step;
            /// sof
            uint32_t sof;
            /// Bytes per pixel
            uint32_t bytes_per_pixel;
            ///
            uint32_t stride;
            /// Timestamp us
            uint64_t timestamp;
            /// Exposure time
            uint32_t exposure_time;
            /// Gain
            uint32_t gain;
        };
        uint8_t padding[64];
    };

    union {
        struct {
            /// ov580 version
            uint32_t version;

            int64_t device_start_time;
        };
        uint8_t meta[64];
    };
};

struct GrayCameraUnit {
    union {
        struct {
            /// Image data offset
            uint32_t offset;
            /// Timestamp nanos
            uint64_t hmd_time_nanos;
            /// Width of image in pixels
            uint32_t width;
            /// Height of image in pixels
            uint32_t height;
            /// Number of bytes per horizontal line
            uint32_t step;
            /// Exposure time
            uint32_t exposure_time; /// us
            /// Gain
            uint32_t gain; /// 1/16
            ///camera_id
            uint8_t cam_id;
        };
        uint8_t padding[64];
    };
};

struct GrayCameraFrame {
    union {
        struct {
            GrayCameraUnit cameras[4];
            /// Image data for this frame
            union {
                uint8_t *data;
                uint64_t data_align;
            };
            /// Size of image data buffer
            uint32_t data_bytes;
            /// Number of camera
            uint8_t camera_count;
            /// Pixel format
            uint8_t pixel_format;
            /// Timestamp raw
            uint64_t timestamp_raw;
            /// Raw timestamp timer bits
            uint8_t timer_bits;
            /// Timestamp hw
            uint64_t hmd_hw_time_nanos;
        };
        uint8_t padding[384];
    };
};

struct CameraInfo{
    union {
        struct {
            uint32_t width;
            uint32_t height;
            uint32_t channels;
            uint32_t uvc_fmt;
            uint32_t frame_rate;
            int32_t usb_fd;
            bool    height_from_config;
        };
        uint8_t padding[48];
    };
};

struct GenericShmWrapper {
    union {
        struct {
            uint32_t index;
            uint32_t size;
            uint8_t  reserved[512]; /// use to save struct
        };
        uint8_t padding[1024];
    };

    union {
        struct {
            /// type = 0: index valid
            uint32_t type;
            uint32_t reserved_size;
        };
        uint8_t meta[128];
    };
};
#pragma pack()

struct CameraFrameInfo
{
    uint16_t width;
    uint16_t height;
    uint16_t fps;
    CameraFrameInfo(uint16_t w, uint16_t h,uint16_t f):width(w),height(h),fps(f) {}
};


enum VideoProperty {
    CAMERA_MAX_EXPOSURE_TIME = 0,
    CAMERA_LUMA_VALUE = 1,
    CAMERA_ISP0_GAMMA_ENABLE = 2,
    CAMERA_ISP1_GAMMA_ENABLE = 3,
    CAMERA_ISP0_GAMMA = 4,
    CAMERA_ISP1_GAMMA = 5,
    CAMERA_TARGET_BRIGHTNESS = 6,
};

typedef void (CopyFrameFunction)(uint8_t *src, uint8_t *dst, uint32_t data_bytes, uint32_t width, uint32_t height, uint32_t bytes_per_pixel);