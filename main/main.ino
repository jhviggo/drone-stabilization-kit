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
    task_send_data,   /* Task function. */
    "WiFi task",      /* String with name of task. */
    20000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    5,                /* Priority of the task. */
    NULL);            /* Task handle. */

  xTaskCreate(
    task_meassure_position,
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
String generate_json(String session, String device, IMU_t imuSensor) {
  String x = String(imuSensor.accel_x, 2);
  String y = String(imuSensor.accel_y, 2);
  String z = String(imuSensor.accel_z, 2);
  String temp = String(imuSensor.temperature, 2);
  return "{\"sessionId\":\"" + session + "\",\"deviceId\":\"" + device + "\",\"temperature\":" + temp + ",\"location\":{\"lat\":56.1206,\"long\":10.1464},\"rotation\":{\"x\":" + x + ",\"y\":" + y + ",\"z\":" + z + "}}";
}

/**
 * WiFi task to send data every REQUEST_REQUENCY ms
 */
void task_send_data(void* parameter) {
  const TickType_t xDelay = REQUEST_FEQUENCY / portTICK_PERIOD_MS;
  IMU_t imuSensor;
  int requestCount = conf_get_session_count();
  String sessionId = conf_get_session_id();
  String deviceName = conf_get_device_name();

  while(requestCount--) {
    vTaskDelay( xDelay );
    imuSensor = imu_get_data();
    String jsonData = generate_json(sessionId, deviceName, imuSensor);
    //Serial.println(jsonData);
    request_send(jsonData);
  }
  vTaskDelete( NULL );
}

/**
 * IMU task to meassure acceleration position every POSITION_MEASSURE_FEQUENCY ms
 */
void task_meassure_position(void* parameter) {
  const TickType_t xFrequency = pdMS_TO_TICKS(POSITION_MEASSURE_FEQUENCY);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true) {
    imu_read_sensor();
    xTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
  vTaskDelete( NULL ); // should not be reached
}
