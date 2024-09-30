#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>   // Include Arduino core

void connectToWiFi(void);
String getIPAddress(void);
void startAPMode(void);
void checkWiFiStatus(void);
void onWiFiConnected(void);
bool showWiFiSetup(void);
void switchWiFiMode(void);

void setWiFiSwitch(void);
uint8_t getWiFiSwitch(void);


void setWiFiSwitch_status(void);
uint8_t getWiFiSwitch_status(void);

#endif