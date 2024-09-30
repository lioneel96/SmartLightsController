#ifndef EEPROM_MANAGER_H
#define EEPROM_MANAGER_H

typedef struct 
{
    char ssid[30];
    char password[30];
} settings_t;

void initializeEEPROM(void);
void saveWiFiSettings(const settings_t& wifi_settings);
settings_t getWiFiSettings(void);

#endif