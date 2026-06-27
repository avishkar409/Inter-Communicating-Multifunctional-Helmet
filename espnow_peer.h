#ifndef ESPNOW_PEER_H
#define ESPNOW_PEER_H

// ── Call in setup() on BOTH helmets ───────────────────────────────────────
void espnowInit();

// ── Call on the SENDER helmet when accident is confirmed ──────────────────
void espnowSendAccidentAlert();

// ── Poll on RECEIVER helmet in loop() ────────────────────────────────────
bool espnowAccidentReceived();   // true when alert packet arrived
void espnowClearAlert();         // reset after buzzer is silenced

#endif
