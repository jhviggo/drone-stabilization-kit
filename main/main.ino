#include "request.h"

/**
 * setup function first called by hardware to set up any important drivers
 */
void setup() {
  Serial.begin(115200);
  request_connect("viggolicious", "84219114");
  int count = 10;
  while (count--) {
    String jsonData = "{\"sessionId\":\"test-run-3\",\"deviceId\":\"ESP-boi\",\"timestamp\":\"2020-01-01T10:10:10\",\"temperature\":" + String(count) + ",\"location\":{\"lat\":1234,\"long\":1234},\"rotation\":{\"x\":10,\"y\":10,\"z\":10}}";
    request_send(jsonData);
    delay(250);
  }
}

/**
 * main program loop which will run constantly
 */
void loop(){

}
