#include <Arduino.h>
#include "config.h"
#include "buzzer.h"

static bool silentMode = false;

void buzzerInit() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("BUZZER_PIN init and set to low");
}

void buzzerOn() {
  if (!silentMode) 
  {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("SOUND ON");
  }
}

void buzzerOff() 
{
  digitalWrite(BUZZER_PIN, LOW);
  Serial.println("SOUND OFF");
}

void toggleSilentMode() 
{
  Serial.println("SOUND MODE CHANGED");
  silentMode = !silentMode;
  buzzerOff();
}

bool isSilentMode() {
  return silentMode;
}
