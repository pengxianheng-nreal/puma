#pragma once

#include <puma/driver/hid_driver/hid_driver.h>
#include <puma/driver/hid_driver/hid_utils.h>
#include <puma/driver/hidapi/include/hidapi.h>

class HidapiDriver : public HidDriver {
public:
    HidapiDriver(std::function<void (const hid_data_t&)> _callback);
    virtual ~HidapiDriver();

    virtual bool init() override;
    virtual bool start() override;
    virtual void stop() override;

    virtual bool setProperties(const uint8_t* cmd, uint32_t cmd_size) override;

    virtual int32_t setFeatureReport(const uint8_t* data, uint32_t length) override;

    virtual int32_t getFeatureReport(uint8_t* data, uint32_t length) override;

    virtual bool getPidVid(uint16_t& pid, uint16_t& vid) override;

	static void Log(HidLogLevel log_level,const char *log);

protected:
    virtual void run() override;
    static void callback(void* data, int32_t data_len, void* user_ptr);
    bool initDevice();

protected:
#if defined(PUMA_SYSTEM_WINDOWS)
    hid_context context_;
#else
    hid_context context_{nullptr};
#endif
    hid_device * dev_fd_{nullptr};
    bool use_callback_{true};
    bool set_property_error_no_device_output_{false};

};