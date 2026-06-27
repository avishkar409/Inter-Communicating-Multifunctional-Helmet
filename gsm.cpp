#include <Arduino.h>
#include "config.h"

#if ENABLE_GSM

#define TINY_GSM_MODEM_SIM800
#define SerialMon Serial
#define SerialAT  Serial1
#define TINY_GSM_DEBUG SerialMon

#include <TinyGsmClient.h>

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

static bool emergencySmsSent = false;

const char* EMERGENCY_CONTACTS[EMERGENCY_CONTACT_COUNT] = {
"+917385110589",
//"+918805110105",
"+918855047503"
};


void gsmInit()
{
  Serial.print("GSM Init in process...");

  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  Serial.print("GSM output set");


  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);
  Serial.print("GSM digital write done");

  SerialAT.begin(GSM_BAUDRATE, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  modem.restart();
  modem.waitForNetwork();

  modem.sendAT("+CMGF=1");
  modem.sendAT("+CNMI=2,2,0,0,0");
}

void gsmUpdate()
{
  while (SerialAT.available())
    SerialAT.read();
}

bool sendEmergencySMS(const String &message)
{
  if (emergencySmsSent) return false;

  bool allSent = true;

  for (int i = 0; i < EMERGENCY_CONTACT_COUNT; i++)
  {
    bool success = modem.sendSMS(EMERGENCY_CONTACTS[i], message);

    if (!success)
      allSent = false;

    delay(2000);  // small delay between messages (important for SIM800)
  }

  if (allSent)
  {
    emergencySmsSent = true;
    Serial.println("Emergency SMS sent to all contacts");
  }
  else
  {
    Serial.println("Some SMS failed");
  }

  return allSent;
}


#else
void gsmInit() {}
void gsmUpdate() {}
bool sendEmergencySMS(const String &) { return false; }
#endif