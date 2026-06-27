#ifndef CONFIG_H
#define CONFIG_H

//connecting second helmet
#define ENABLE_ESPNOW 1

// ================== PIN DEFINITIONS (SENSORS & ACTUATORS) ==================
// Using ESP32 safe GPIOs (no GSM conflict)

#define HELMET_TOUCH_PIN 32 // Touch Sensor 1 (Helmet detection)
#define OVERRIDE_TOUCH_PIN 33 // Touch Sensor 2 (Override / mute)
#define IR_EYE_PIN 34 // IR Eye sensor (input-only GPIO)
#define BUZZER_PIN 25 // Buzzer output

// ================== I2C CONFIG (ADXL345) ==================
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// ================== PHASE-1 TIMING CONFIG ==================
#define EYE_CLOSE_TIME_MS 3000
#define SHORT_TOUCH_MAX_MS 3000
#define LONG_TOUCH_MIN_MS 3000

// ================== ADXL345 CONFIG (PHASE-2) ==================
#define ENABLE_ADXL345 1

#define ADXL_FREE_FALL_THRESHOLD 0.4 // g
#define ADXL_IMPACT_THRESHOLD 2.5 // g
#define ADXL_FALL_TIME_WINDOW_MS 800 // ms

// ================== GSM CONFIG (SIM800 – LILYGO T-CALL V1.4) 
#define ENABLE_GSM 1

#define EMERGENCY_CONTACT_COUNT 3

extern const char* EMERGENCY_CONTACTS[EMERGENCY_CONTACT_COUNT];


// GSM pins (DO NOT CHANGE – board specific)
#define MODEM_RST 5
#define MODEM_PWKEY 4
#define MODEM_POWER_ON 23
#define MODEM_TX 27
#define MODEM_RX 26

#define GSM_BAUDRATE 9600
#define GSM_PIN_CODE "" // Leave empty if SIM has no PIN

// ================== GPS CONFIG (NEO-6M) ==================
#define ENABLE_GPS 1

#define GPS_RX_PIN 17 // ESP32 UART2 RX
#define GPS_TX_PIN 16 // ESP32 UART2 TX
#define GPS_BAUDRATE 9600

#define GPS_FIX_TIMEOUT_MS 15000 // Optional GPS fix timeout (ms)

#endif


