#include "request.h"
#include "imu.h"


/**
 * setup function first called by hardware to set up any important drivers
 */
void setup() {
  Serial.begin(115200);

  int imuStatus = imu_create();
  if (imuStatus < 0) {
    Serial.println("IMU initialization unsuccessful");
    while(1) {}
  }
  
  request_connect("viggolicious", "84219114");
  IMU_t imuSensor;
  int count = 20;
  while (count--) {
    imu_readSensor();
    imuSensor = imu_get_data();
    String x = String(imuSensor.accel_x, 2);
    String y = String(imuSensor.accel_y, 2);
    String z = String(imuSensor.accel_z, 2);
    String temp = String(imuSensor.temperature, 2);
    String jsonData = "{\"sessionId\":\"test-run-5\",\"deviceId\":\"ESP-boi\",\"timestamp\":\"2020-01-01T10:10:10\",\"temperature\":" + temp + ",\"location\":{\"lat\":1234,\"long\":1234},\"rotation\":{\"x\":" + x + ",\"y\":" + y + ",\"z\":" + z + "}}";
    Serial.println(jsonData);
    Serial.print("RUN: ");
    Serial.println(20 - count);
    request_send(jsonData);
    delay(250);
  }
}

/**
 * main program loop which will run constantly
 */
void loop(){

}
