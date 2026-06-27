# Inter-Communicating-Multifunctional-Helmet
Smart Helmet for Accident Detection and Emergency Communication using ESP32.
# 🪖 Inter Communicating Multi-Functional Helmet for Bikers

## 📌 Overview

The **Inter Communicating Multi-Functional Helmet for Bikers** is a smart helmet designed to enhance rider safety by combining accident detection, emergency communication, GPS tracking, and helmet-to-helmet communication into a single embedded system.

The system automatically detects accidents using an ADXL345 accelerometer, obtains the rider's location through a GPS module, sends emergency SMS alerts via GSM, and simultaneously notifies nearby riders using ESP-NOW communication. The helmet also includes capacitive touch sensors for helmet detection and user override functionality.

---

## ✨ Features

- Automatic accident detection using ADXL345
- Real-time GPS location tracking
- Google Maps link generation
- Emergency SMS alerts using SIM800L GSM module
- Helmet-to-Helmet communication using ESP-NOW
- Capacitive touch sensor for helmet wear detection
- Emergency buzzer notification
- Low-cost and portable design
- Standalone operation without internet connectivity

---

## 🛠 Hardware Components

- ESP32 Development Board
- ADXL345 3-Axis Accelerometer
- NEO-6M GPS Module
- SIM800L GSM Module
- Capacitive Touch Sensors
- Active Buzzer
- Li-ion Battery
- TP4056 Battery Charging Module
- Connecting Wires and PCB

---

## 💻 Software Requirements

- Arduino IDE
- ESP32 Arduino Core
- TinyGPS++
- Wire Library
- WiFi Library
- ESP-NOW Library

---

## ⚙ System Workflow

1. Helmet is worn and the system becomes active.
2. ESP32 continuously monitors acceleration data from the ADXL345.
3. A dual-stage algorithm detects free-fall followed by high impact.
4. Upon accident confirmation:
   - GPS retrieves the rider's current location.
   - A Google Maps link is generated.
   - GSM sends an emergency SMS to predefined contacts.
   - ESP-NOW broadcasts an alert to nearby helmets.
   - The buzzer activates to indicate the emergency state.
5. The system remains in emergency mode until manually acknowledged.

---

## 📂 Project Structure

```
Project
│
├── main.ino
├── config.h
├── adxl345.cpp
├── adxl345.h
├── gps.cpp
├── gps.h
├── gsm.cpp
├── gsm.h
├── buzzer.cpp
├── buzzer.h
├── helmetTouch.cpp
├── helmetTouch.h
├── overrideTouch.cpp
├── overrideTouch.h
├── espnow_peer.cpp
├── espnow_peer.h
└── README.md
```

---

## 🚀 Technologies Used

- Embedded Systems
- ESP32
- C/C++
- Arduino IDE
- ESP-NOW
- GPS
- GSM
- I2C Communication
- UART Communication

---

## 📖 Applications

- Motorcycle Rider Safety
- Accident Detection
- Emergency Alert System
- Rider-to-Rider Communication
- Smart Transportation
- Road Safety

---

## 👨‍💻 Authors

- Avishkar Kale
- Naresh Gunale
- Anuj Kanade

**Department of Electronics and Telecommunication Engineering**

P.E.S. Modern College of Engineering, Pune

---

## 👩‍🏫 Project Guide

**Mrs. A. P. More**

Assistant Professor

Department of Electronics and Telecommunication Engineering

P.E.S. Modern College of Engineering, Pune

---

## 📚 References

- ESP32 Documentation (Espressif Systems)
- ADXL345 Datasheet (Analog Devices)
- SIM800L Hardware Design Guide
- u-blox NEO-6M GPS Datasheet
- ESP-NOW Programming Guide
- World Health Organization (WHO)
- Ministry of Road Transport and Highways (MoRTH)

---

## 📄 License

This project was developed as a Final Year Engineering Project for academic purposes.

© 2026 Avishkar Kale, Naresh Gunale, and Anuj Kanade. All rights reserved.
