#include "request.h"
#include "imu.h"

#define REQUEST_FEQUENCY 100UL // 100ms
#define POSITION_MEASSURE_FEQUENCY 10UL // 10ms

int requestCount = 0;
String sessionId;

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
  String inputRequestCount;
  
  Serial.print("Session length: ");
  delay(50);
  while (!Serial.available()) {} // await serial connection
  inputRequestCount = Serial.readString();
  Serial.println(inputRequestCount);
  requestCount = inputRequestCount.toInt();

  delay(50);
  Serial.print("Session ID: ");
  while (!Serial.available()) {} // await serial connection
  sessionId = Serial.readString();
  sessionId.remove(sessionId.length() - 1); // remove newline char
  Serial.println(sessionId);
  
  request_connect("viggolicious", "84219114");

  xTaskCreate(
    taskSendData,     /* Task function. */
    "WiFi task",      /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    5,                /* Priority of the task. */
    NULL);            /* Task handle. */

  xTaskCreate(
    taskMeassurePosition,
    "IMU task",
    10000,
    NULL,
    4,
    NULL);
}

/**
 * @param session name
 * @param device ID
 * @param IMU sensor struct containing data
 * @return JSON string containing data
 */
String generateJSON(String session, String device, IMU_t imuSensor) {
  String x = String(imuSensor.accel_x, 2);
  String y = String(imuSensor.accel_y, 2);
  String z = String(imuSensor.accel_z, 2);
  String temp = String(imuSensor.temperature, 2);
  return "{\"sessionId\":\"" + session + "\",\"deviceId\":\"" + device + "\",\"temperature\":" + temp + ",\"location\":{\"lat\":1234,\"long\":1234},\"rotation\":{\"x\":" + x + ",\"y\":" + y + ",\"z\":" + z + "}}";
}

/**
 * main program loop which will run constantly
 */
void loop(){}

/**
 * WiFi task to send data every REQUEST_REQUENCY ms
 */
void taskSendData(void* parameter) {
  const TickType_t xFrequency = pdMS_TO_TICKS(REQUEST_FEQUENCY);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  IMU_t imuSensor;

  while(requestCount--) {
    xTaskDelayUntil(&xLastWakeTime, xFrequency);
    imuSensor = imu_get_data();
    String jsonData = generateJSON("test-run-" + sessionId, "ESP-boi", imuSensor);
    Serial.println(jsonData);
    request_send(jsonData);
  }
  vTaskDelete( NULL );
}

/**
 * IMU task to meassure acceleration position every POSITION_MEASSURE_FEQUENCY ms
 */
void taskMeassurePosition(void* parameter) {
  const TickType_t xFrequency = pdMS_TO_TICKS(POSITION_MEASSURE_FEQUENCY);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true) {
    imu_readSensor();
    xTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
  vTaskDelete( NULL ); // should not be reached
}
