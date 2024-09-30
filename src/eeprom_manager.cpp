#include <eeprom_manager.h>
#include <EEPROM.h>

void initializeEEPROM() 
{
    EEPROM.begin( sizeof(settings_t) );
}

void saveWiFiSettings(const settings_t& wifi_settings) 
{
    EEPROM.put(0, wifi_settings);
    EEPROM.commit();
}

settings_t getWiFiSettings()
{
    settings_t wifi_setings = {};
    EEPROM.get( 0, wifi_setings );
    return wifi_setings;
}
