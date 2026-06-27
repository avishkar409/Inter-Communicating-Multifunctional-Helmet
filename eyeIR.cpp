#include <Arduino.h>
#include "config.h"
#include "eyeIR.h"

static unsigned long eyeCloseStart = 0;
static bool eyeClosed = false;

void eyeIRInit() {
  pinMode(IR_EYE_PIN, INPUT);
  //Serial.println("IR_EYE_PIN init");
}

bool isEyeClosedTooLong() {
  if (digitalRead(IR_EYE_PIN) == LOW) {  // LOW = eyes closed
    if (!eyeClosed) {
      eyeClosed = true;
      eyeCloseStart = millis();
    }
    if (millis() - eyeCloseStart >= EYE_CLOSE_TIME_MS) {
      return true;
    }
  } else {
    eyeClosed = false;
    eyeCloseStart = 0;
  }
  return false;
}
