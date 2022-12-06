#pragma once

struct GPS_t {
  float longtitude;
  float latitude;
};

int gps_create();
void gps_read_sensor();
GPS_t gps_get_data();