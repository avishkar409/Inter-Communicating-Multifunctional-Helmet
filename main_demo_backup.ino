#include <Arduino.h>
#include "config.h"

#include "helmetTouch.h"
#include "eyeIR.h"
#include "overrideTouch.h"
#include "buzzer.h"
#include "adxl345.h"
#include "gps.h"
#include "gsm.h"
#include "espnow_peer.h"   // 🔥 ADDED

bool smsSent = false;

unsigned long accidentStartTime = 0;
bool accidentPending = false;

enum SystemState {
  SYSTEM_IDLE,
  SYSTEM_ACTIVE,
  SYSTEM_EMERGENCY
};

SystemState systemState = SYSTEM_IDLE;

void setup() 
{
  Serial.begin(9600);

  buzzerInit();
  buzzerOn();
  
  helmetTouchInit();
  //eyeIRInit();
  overrideTouchInit();

  adxlInit();
  gpsInit();
  gsmInit();

  espnowInit();   // 🔥 ESP-NOW INIT

  buzzerOff();
  Serial.println("Smart Helmet System Started but Current mode: SYSTEM_IDLE");
}

void loop() 
{
  gpsUpdate(); 

  // ───────────── SYSTEM IDLE ─────────────
  if (systemState == SYSTEM_IDLE) 
  {
    if (isHelmetWornOnce()) 
    {
      systemState = SYSTEM_ACTIVE;
      Serial.println("Helmet detected. System ACTIVE.");
    }
    return;
  }

  // ───────────── SYSTEM ACTIVE ─────────────
  if (systemState == SYSTEM_ACTIVE) 
  {
    // // 👁 Eye detection
    // if (isEyeClosedTooLong()) 
    // {
    //   Serial.println("EYE CLOSED");
    //   buzzerOn();
    // } 
    // else 
    // {
    //   Serial.println("EYE OPEN");
    //   buzzerOff();
    // }

    // ✋ Touch override
 TouchEvent event = readOverrideTouch();

    if (event == SHORT_PRESS) 
{
  buzzerOff();
}

if (event == LONG_PRESS) 
{
  toggleSilentMode();

  // ✅ cancel accident if user presses long
  if (accidentPending) {
    accidentPending = false;
    buzzerOff();
    Serial.println("Accident cancelled by user.");
  }
}

    // 📡 ESP-NOW RECEIVE (🔥 NEW)
    if (espnowAccidentReceived()) 
    {
      Serial.println("⚠️ Nearby rider accident detected!");
      buzzerOn();
      espnowClearAlert();
    }

    // 📊 Sensors update
    adxlUpdate();
    gpsUpdate();
    gsmUpdate();

    // 🚨 Accident detection
    if (isAccidentDetected()) 
 {
   if (!accidentPending) {
      accidentPending = true;
      accidentStartTime = millis();
      Serial.println("⚠️ Possible accident detected...");
  }

  // wait 3 seconds before confirming
  if (millis() - accidentStartTime > 3000) {
    systemState = SYSTEM_EMERGENCY;
    Serial.println("🚨 Accident CONFIRMED!");

    espnowSendAccidentAlert();   // 📡 send to other helmets
  }
}
else 
{
  accidentPending = false;
}

if (accidentPending) {
  buzzerOn();   // warning sound
}
}

  // ───────────── EMERGENCY STATE ─────────────
  if (systemState == SYSTEM_EMERGENCY) 
{
  Serial.println("SYSTEM MODE: Emergency state.");

  // Give GPS small time to update before sending SMS
unsigned long start = millis();
while (millis() - start < 2000)
{
  gpsUpdate();
}

  if (!smsSent)   // ✅ send only once
  {
String message = "🚨 Accident Detected!\n";
message += getLocationText();   // ✅ use existing function

    sendEmergencySMS(message);
    smsSent = true;
  }

  gsmUpdate();

  // keep sending ESP-NOW alert
  espnowSendAccidentAlert();
}
}