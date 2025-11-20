#pragma once

#include "nr_plugin_types.h"

#ifdef NRAPP

#include "nr_plugin_imu_types.inc"

#else


NR_PLUGIN_ENUM(NRImuDataMask) {
    NR_IMU_DATA_MASK_NULL = 0x0000,
    NR_IMU_DATA_MASK_GYROSCOPE = 0x0001,
    NR_IMU_DATA_MASK_ACCELEROMETER = 0x0002,
    NR_IMU_DATA_MASK_MAGNETOMETER = 0x0004,
    NR_IMU_DATA_MASK_TEMPERATURE = 0x0008,
    NR_IMU_DATA_MASK_ALL = 0xFFFF,
};

#pragma pack(1)

typedef struct NRImuData {
    union {
        struct {
            uint64_t hmd_time_nanos_system;
            uint64_t hmd_time_nanos_device;
            uint64_t hmd_time_nanos_sensor;
            int32_t data_mask;
            NRVector3f gyroscope;
            NRVector3f accelerometer;
            NRVector3f magnetometer;
            float temperature;
            int8_t imu_id;
            uint32_t frame_id;
            int32_t gyroscope_numerator;
            int32_t accelerometer_numerator;
            int32_t magnetometer_numerator;
            uint32_t out_numerator_mask;
            uint32_t group_delay;
        };
        uint8_t padding[128];
    };

} NRImuData;

#pragma pack()

#endif // NRAPP