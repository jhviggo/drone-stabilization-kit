#include "configuration.h"

struct Configuration {
  String sessionId;
  int sessionCount;
  String apiDomain = "drone-stabilization-367016.ey.r.appspot.com";
  String deviceId = "ESP-boi-1594";
  String deviceName = "ESP-boi";
} conf;

String conf_get_session_id() {
  return conf.sessionId;
}

int conf_get_session_count() {
  return conf.sessionCount;
}

String conf_get_api_domain() {
  return conf.apiDomain;
}

String conf_get_device_id() {
  return conf.deviceId;
}

String conf_get_device_name() {
  return conf.deviceName;
}

void conf_set_session_id(String id) {
  conf.sessionId = id;
}

void conf_set_session_count(int count) {
  conf.sessionCount = count;
}

void conf_set_api_domain(String url) {
  conf.apiDomain = url;
}

void conf_set_device_id(String id) {
  conf.deviceId = id;
}

void conf_set_device_name(String deviceName) {
  conf.deviceName = deviceName;
}
