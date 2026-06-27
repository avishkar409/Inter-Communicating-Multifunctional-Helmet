#ifndef GSM_H
#define GSM_H

void gsmInit();
void gsmUpdate();
bool sendEmergencySMS(const String &message);

#endif
