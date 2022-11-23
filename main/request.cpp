#include "request.h"
#include <WiFi.h>

/**
 * Connects to the given WiFi signal
 * 
 * @param ssid router name
 * @param passwd password 
 */
void request_connect(char ssid[], char passwd[]) {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, passwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/**
 * Sends a post request with the given json data
 * 
 * @param jsonData a string representing JSON data
 * @return int code showing if the request was successful
 */
int request_send(String jsonData) {
  WiFiClient client;
  const int httpPort = 80;
  const char* host = "drone-stabilization-367016.ey.r.appspot.com";
  if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      return 1;
  }
  client.print(String("POST ") + "/run " + "HTTP/1.1\r\n"
    + "Host: " + host + "\r\n"
    + "Content-Type: application/json\r\n"
    + "Content-Length: " + jsonData.length() + "\r\n\r\n"
    + jsonData + "\r\n\r\n");

  unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return 1;
      }
    }

    if (client.available()) {
      String line = client.readStringUntil('\r');
      Serial.println(line);
      client.flush();
    }
    return 0;
}
