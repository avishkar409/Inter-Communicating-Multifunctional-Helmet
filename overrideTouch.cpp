#include <Arduino.h>
#include "config.h"
#include "overrideTouch.h"

static unsigned long touchStartTime = 0;
static bool touchActive = false;

void overrideTouchInit() {
  pinMode(OVERRIDE_TOUCH_PIN, INPUT);
  Serial.println("OVERRIDE_TOUCH_PIN init");
}

TouchEvent readOverrideTouch() {
  if (digitalRead(OVERRIDE_TOUCH_PIN) == HIGH) {
    if (!touchActive) {
      touchActive = true;
      touchStartTime = millis();
    }
  } else {
    if (touchActive) {
      touchActive = false;
      unsigned long duration = millis() - touchStartTime;

      if (duration < SHORT_TOUCH_MAX_MS) {
        return SHORT_PRESS;
      } else {
        return LONG_PRESS;
      }
    }
  }
  return NO_PRESS;
}
