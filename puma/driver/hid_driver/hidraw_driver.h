#pragma once

#include <puma/env/system_config.h>
#if defined(PUMA_SYSTEM_LINUX) || defined(PUMA_SYSTEM_ANDROID)
#include <puma/driver/hid_driver/hid_driver.h>
#include <puma/driver/hid_driver/hid_utils.h>

class HidrawDriver : public HidDriver {
public:
    HidrawDriver(std::function<void (const hid_data_t&)> _callback);
    virtual ~HidrawDriver();

    virtual bool init() override;
    virtual bool start() override;

    bool initDevice();

    virtual bool setProperties(const uint8_t* cmd, uint32_t cmd_size) override;

    virtual int32_t setFeatureReport(const uint8_t* data, uint32_t length) override;

    virtual int32_t getFeatureReport(uint8_t* data, uint32_t length) override;

    virtual bool getPidVid(uint16_t& pid, uint16_t& vid) override;

protected:
    virtual void run() override;

protected:
    int32_t dev_fd_ = -1;
    uint16_t vid_{0}, pid_{0};
};

#endif 