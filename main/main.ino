#include "request.h"
#include "imu.h"
#include "configuration.h"

#define REQUEST_FEQUENCY 100UL // 100ms
#define POSITION_MEASSURE_FEQUENCY 10UL // 10ms
#define ROUTER_SSID "viggolicious"
#define ROUTER_PASWD "84219114"
#define SESSION_PREFIX "test-run-"

/* unused Arduino loop function */
void loop() {}

/**
 * setup function first called by hardware to set up any important drivers
 */
void setup() {
  Serial.begin(115200);

  /* create IMU instance */
  int imuStatus = imu_create();
  if (imuStatus < 0) {
    Serial.println("IMU initialization unsuccessful");
    while(1) {}
  }
  
  /* Get session length */
  Serial.print("Session length: ");
  delay(50);
  while (!Serial.available()) {} // await serial connection
  String inputRequestCount = Serial.readString();
  Serial.println(inputRequestCount);
  conf_set_session_count(inputRequestCount.toInt());

  /* get session ID */
  delay(50);
  Serial.print("Session ID: ");
  while (!Serial.available()) {} // await serial connection
  String sessionId = Serial.readString();
  sessionId.remove(sessionId.length() - 1); // remove newline char
  Serial.println(sessionId);
  conf_set_session_id(SESSION_PREFIX + sessionId);

  /* connect to network */
  request_connect(ROUTER_SSID, ROUTER_PASWD);

  /* create tasks */
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
 * WiFi task to send data every REQUEST_REQUENCY ms
 */
void taskSendData(void* parameter) {
  const TickType_t xFrequency = pdMS_TO_TICKS(REQUEST_FEQUENCY);
  TickType_t xLastWakeTime = xTaskGetTickCount();
  IMU_t imuSensor;
  int requestCount = conf_get_session_count();
  String sessionId = conf_get_session_id();
  String deviceName = conf_get_device_name();

  while(requestCount--) {
    xTaskDelayUntil(&xLastWakeTime, xFrequency);
    imuSensor = imu_get_data();
    String jsonData = generateJSON(sessionId, deviceName, imuSensor);
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
