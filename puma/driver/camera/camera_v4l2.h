#pragma once
#include <puma/env/system_config.h>

#if defined(PUMA_SYSTEM_LINUX) || defined(PUMA_SYSTEM_ANDROID)
#include <puma/driver/camera/camera_driver.h>
#include <string>
#include <queue>
#include <linux/videodev2.h>

struct buffer_t {
    void* start;
    size_t length;
};

struct v4l2_contrl {
    __u32 id;
    __s32 value;
};


class CameraV4l2 : public CameraDriver {
public:
    CameraV4l2(std::function<void (const GenericShmWrapper&)> _callback);

    ~CameraV4l2();

    virtual void setCameraProperties(const CameraInfo& info) override;

    virtual bool start() override;

    virtual void stop() override;

    virtual void setAutoExposure(int _auto_exposure) override;

    virtual void setExposureTime(double _exposure_time) override;

    virtual void setGain(int _gain) override;

    virtual bool setOutsideMem(const void *ptr, size_t size) override;
    virtual void setIOMethod(IO_Method _io_method) override;

    virtual bool valid() override;

protected:
    virtual void run() override;
    void frame_process(void* _frame, uint32_t _size, int _index = -1);

    /// find device by vid and pid
    bool find_device(int16_t _vid, int16_t _pid);

    /// find device by product name, i.e. capabilities card info
    bool find_device(const std::string& _device_name);

    bool find_device();

    int is_readable(timeval* tv);
    int read_frame();
    bool open_device();
    void close_device();
    bool start_device();
    bool stop_device();
    int restart_device();
    bool init_device();
    bool check_capabilities();
    bool check_format();
    bool config_format();
    bool config_fps();
    bool start_capturing();
    bool stop_capturing();
    bool uninit_device();
    bool init_read();
    bool init_mmap();
    bool init_userptr();
    bool init_gain_range();

    bool ConvertImageData(void* _frame, uint32_t _size);

protected:
    /// video file handle
    int camera_fd_{-1};
    int restart_flag_{0};
    bool isStreamOn_{false};

    /// device type, default video capture
    v4l2_buf_type device_type_{V4L2_BUF_TYPE_VIDEO_CAPTURE};
    v4l2_pix_format pix_format_;
    uint32_t image_bytes_;

    ///
    const uint32_t V4L2_BUF_NUM_{30};
    buffer_t *buffers_{nullptr};
    uint32_t buffers_num_{V4L2_BUF_NUM_};
    uint16_t image_fps_{30};

    uint8_t *outside_buff_ptr{nullptr};
    uint32_t outside_buff_size_{0};
    std::queue<v4l2_buffer> memory_user_queue_;

    std::vector<uint8_t> converted_image_data_;
};

#endif // 
