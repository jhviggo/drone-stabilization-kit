#include <TinyGPS.h>
#include <SoftwareSerial.h>

TinyGPS gps;
SoftwareSerial ss(4, 3);

struct GPS_t gpsData;

int gps_create() {
  ss.begin(9600, SWSERIAL_8N1, 13, 12, false);
}

void gps_read_sensor() { 
  float lat, lon;
  unsigned long age;
  gps.f_get_position(&flat, &flon, &age);
  gpsData.latitude = lat;
  gpsData.longtidude = lon;
}

GPS_t gps_get_data() {
  return gpsData;
}