#pragma once

#include <Arduino.h>

String conf_get_session_id();
int conf_get_session_count();
String conf_get_api_domain();
String conf_get_device_id();
String conf_get_device_name();

void conf_set_session_id(String id);
void conf_set_session_count(int count);
void conf_set_api_domain(String url);
void conf_set_device_id(String id);
void conf_set_device_name(String deviceName);
