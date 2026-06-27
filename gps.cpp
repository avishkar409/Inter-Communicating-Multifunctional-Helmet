#include <Arduino.h>
#include "config.h"

#if ENABLE_GPS
#include <TinyGPS++.h>

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

static double lastLat = 0.0;
static double lastLng = 0.0;
static bool hasFix = false;

void gpsInit()
{
  gpsSerial.begin(GPS_BAUDRATE, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);
  Serial.println("GPS Initialized");
}

void gpsUpdate()
{
  while (gpsSerial.available() > 0)
  {
    char c = gpsSerial.read();
    gps.encode(c);
  }

  // 🔥 move this OUTSIDE while
  if (gps.location.isValid())
  {
    lastLat = gps.location.lat();
    lastLng = gps.location.lng();
    hasFix = true;

    Serial.print("Lat: ");
    Serial.print(lastLat, 6);
    Serial.print("  Lon: ");
    Serial.println(lastLng, 6);
  }
}
String getLocationText()
{
  Serial.print("FINAL Lat: ");
  Serial.println(lastLat);

  Serial.print("FINAL Lon: ");
  Serial.println(lastLng);

  String link = "https://maps.google.com/?q=";

  if (lastLat != 0 && lastLng != 0)
  {
    link += String(lastLat, 6);
    link += ",";
    link += String(lastLng, 6);
  }
  else
  {
    link +="18.5261982,73.8464997"; // fallback
  }

  return link;
}

#else
void gpsInit() {}
void gpsUpdate() {}
String getLocationText() { return "GPS disabled"; }
#endif
