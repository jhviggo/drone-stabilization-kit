#pragma once
#include "MPU9250.h"

struct IMU_t {
  float accel_x;
  float accel_y;
  float accel_z;
  float temperature;
};

int imu_create();
void imu_read_sensor();
IMU_t imu_get_data();
float imu_get_roll();
float imu_get_pitch();
float imu_get_temperature();
