#include "imu.h"

MPU9250 IMU(Wire,0x68);
struct IMU_t imuData;

int imu_create() {
  return IMU.begin();
}

void imu_readSensor() {
  IMU.readSensor();
  imuData.accel_x = IMU.getAccelX_mss();
  imuData.accel_y = IMU.getAccelY_mss();
  imuData.accel_z = IMU.getAccelZ_mss();
  imuData.temperature = IMU.getTemperature_C();

}

IMU_t imu_get_data() {
  return imuData;
}

float imu_get_roll() {
  float ax = imuData.accel_x;
  float ay = imuData.accel_y;
  float az = imuData.accel_z;
  return atan(ay / sqrt(ax * ax + az * az)) * 180 / 3.14;
}

float imu_get_pitch() {
  float ax = imuData.accel_x;
  float ay = imuData.accel_y;
  float az = imuData.accel_z;
  return atan(ax / sqrt(ay * ay + az * az)) * 180 / 3.14;
}

float imu_get_temperature() {
  return imuData.temperature;
}
