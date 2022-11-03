#pragma once
#include <Arduino.h>

void request_connect(char ssid[], char passwd[]);
int request_send(String jsonData);
