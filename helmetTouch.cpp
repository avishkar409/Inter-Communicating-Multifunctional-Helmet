#include <Arduino.h>
#include "config.h"
#include "helmetTouch.h"

static bool helmetConfirmed = false;

void helmetTouchInit() {
  pinMode(HELMET_TOUCH_PIN, INPUT);
  Serial.println("HELMET_TOUCH_PIN init");

}

bool isHelmetWornOnce() {
  if (!helmetConfirmed && digitalRead(HELMET_TOUCH_PIN) == HIGH) {
    helmetConfirmed = true;
    return true;
  }
  return helmetConfirmed;
}
