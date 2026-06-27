#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "config.h"
#include "adxl345.h"

#if ENABLE_ADXL345

#define ADXL345_ADDR 0x53

static bool freeFallDetected = false;
static bool accidentDetected = false;
static unsigned long freeFallTime = 0;

static void writeRegister(uint8_t reg, uint8_t value)
{
  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

void adxlInit()
{
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  Wire.beginTransmission(ADXL345_ADDR);
  // if (Wire.endTransmission() != 0) return;

  if (Wire.endTransmission() != 0)
  {
    Serial.println("❌ ADXL345 not detected");
    while (1);
  }

  Serial.println("✅ ADXL345 detected");

  writeRegister(0x2D, 0x08); // Measurement mode
  writeRegister(0x31, 0x08); // Full resolution ±2g
  writeRegister(0x2C, 0x0A); // 100 Hz

  Serial.println("ADXL345 init.");
}

void adxlUpdate()
{
  if (accidentDetected) return;

  int16_t x, y, z;

  Wire.beginTransmission(ADXL345_ADDR);
  Wire.write(0x32);
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345_ADDR, 6);

  if (Wire.available() < 6) return;

  x = Wire.read() | (Wire.read() << 8);
  y = Wire.read() | (Wire.read() << 8);
  z = Wire.read() | (Wire.read() << 8);

  float ax = x * 0.004;
  float ay = y * 0.004;
  float az = z * 0.004;

  float accMag = sqrt(ax * ax + ay * ay + az * az);

  Serial.print("Acc Mag: ");
  Serial.println(accMag);

  if (accMag < ADXL_FREE_FALL_THRESHOLD && !freeFallDetected)
  {
    freeFallDetected = true;
    freeFallTime = millis();
    Serial.println("⚠️ Possible Free Fall");
  }

  if (freeFallDetected)
  {
    if (accMag > ADXL_IMPACT_THRESHOLD &&
        (millis() - freeFallTime) < ADXL_FALL_TIME_WINDOW_MS)
    {
      Serial.println("🚨 FALL DETECTED 🚨 : Acc Mag: ");
      Serial.println(accMag);
      
      accidentDetected = true;
      freeFallDetected = false;
    }
    else if ((millis() - freeFallTime) > ADXL_FALL_TIME_WINDOW_MS)
    {
      freeFallDetected = false;
    }
  }
}

bool isAccidentDetected()
{
  return accidentDetected;
}

#else
void adxlInit() {}
void adxlUpdate() {}
bool isAccidentDetected() { return false; }
#endif
