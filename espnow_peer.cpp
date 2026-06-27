// espnow_peer.cpp
// Helmet-to-helmet accident alert via ESP-NOW (2.4 GHz, no router needed).
//
// HOW TO USE:
//   1. Upload GetMacAddress.ino to Helmet B → note its MAC address
//   2. Paste Helmet B's MAC into PEER_MACS[][] below
//   3. Upload this full project to BOTH helmets
//   4. When Helmet A detects an accident, it calls espnowSendAccidentAlert()
//   5. Helmet B receives the packet, sets alertReceived = true
//   6. main.ino on Helmet B detects this and fires its buzzer
//
// RANGE: ~230 m outdoors, ~80-100 m indoors through walls
// NO PAIRING, NO ROUTER, NO INTERNET NEEDED

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "config.h"
#include "espnow_peer.h"

#if ENABLE_ESPNOW

// ── PEER MAC ADDRESSES ────────────────────────────────────────────────────
// Add one row per peer helmet.
// Format: { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF }
// To find Helmet B's MAC: upload GetMacAddress.ino to it and check Serial.
static uint8_t PEER_MACS[][6] = {
 { 0x24, 0x6F, 0x28, 0xF4, 0x8E, 0x5C } // ← Helmet 2 MAC ADDRESS
 // { 0x24, 0x6F, 0x28, 0xF3, 0xB7, 0xB4 } // ←Helmet 1 MAC ADDRESS
};
static const int PEER_COUNT = sizeof(PEER_MACS) / sizeof(PEER_MACS[0]);

// ── Packet structure ──────────────────────────────────────────────────────
// Keep this struct identical on all helmets.
typedef struct __attribute__((packed))
{
  uint8_t  type;       // 0x01 = accident alert
  uint32_t helmetId;   // lower 3 bytes of sender MAC – identifies which helmet crashed
} AlertPacket;

// ── State ─────────────────────────────────────────────────────────────────
static volatile bool alertReceived = false;

// ── Receive callback (executes on Helmet B when packet arrives) ───────────
static void onDataReceived(const esp_now_recv_info_t *info,
                           const uint8_t *data,
                           int len)
{
  const uint8_t *mac = info->src_addr;

  if (len < (int)sizeof(AlertPacket)) return;

  AlertPacket pkt;
  memcpy(&pkt, data, sizeof(AlertPacket));

  if (pkt.type == 0x01)
  {
    alertReceived = true;
    Serial.printf("[ESP-NOW] ACCIDENT ALERT from %02X:%02X:%02X:%02X:%02X:%02X\n",
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  }
}

// ── Send callback (debug only) ────────────────────────────────────────────
// ✅ NEW ESP32 (IDF v5+) compatible callback
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {

  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");

  Serial.print("Receiver MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

Serial.printf("[ESP-NOW] Send to %02X:%02X:%02X:%02X:%02X:%02X → %s\n",
              mac_addr[0], mac_addr[1], mac_addr[2],
              mac_addr[3], mac_addr[4], mac_addr[5],
              status == ESP_NOW_SEND_SUCCESS ? "SUCCESS" : "FAILED");
}
// ─────────────────────────────────────────────────────────────────────────
void espnowInit()
{
  // ESP-NOW requires STA mode (not connected to any AP)
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("[ESP-NOW] INIT FAILED – peer alerts will not work");
    return;
  }

  esp_now_register_recv_cb(onDataReceived);
  esp_now_register_send_cb(onDataSent);

  // Register all peer helmets
  for (int i = 0; i < PEER_COUNT; i++)
  {
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, PEER_MACS[i], 6);
    peerInfo.channel = 0;       // 0 = same channel as current Wi-Fi channel
    peerInfo.encrypt = false;   // no encryption (add later if needed)

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
      Serial.println("[ESP-NOW] Failed to register peer " + String(i));
    else
      Serial.printf("[ESP-NOW] Peer %d registered: %02X:%02X:%02X:%02X:%02X:%02X\n",
                    i,
                    PEER_MACS[i][0], PEER_MACS[i][1], PEER_MACS[i][2],
                    PEER_MACS[i][3], PEER_MACS[i][4], PEER_MACS[i][5]);
  }

  Serial.print("[ESP-NOW] My MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.println("[ESP-NOW] Ready");
}

// ─────────────────────────────────────────────────────────────────────────
void espnowSendAccidentAlert()
{
  AlertPacket pkt;
  pkt.type     = 0x01;
  pkt.helmetId = (uint32_t)(ESP.getEfuseMac() & 0xFFFFFF);   // unique per board

  for (int i = 0; i < PEER_COUNT; i++)
  {
    esp_now_send(PEER_MACS[i], (uint8_t*)&pkt, sizeof(pkt));
  }
  Serial.println("[ESP-NOW] Accident alert sent to all peers");
}

// ─────────────────────────────────────────────────────────────────────────
bool espnowAccidentReceived()
{
  return alertReceived;
}

void espnowClearAlert()
{
  alertReceived = false;
}

#else
// ── Stubs when ENABLE_ESPNOW = 0 ─────────────────────────────────────────
void espnowInit()              {}
void espnowSendAccidentAlert() {}
bool espnowAccidentReceived()  { return false; }
void espnowClearAlert()        {}
#endif
