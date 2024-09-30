#include <wifi_manager.h>
#include <eeprom_manager.h>

#include <ESP8266WiFi.h>

#define WIFI_MAX_RETRIES 30  // Maximum number of retries before failover

uint8_t switchWiFi = 0;
uint8_t switchWiFi_status = 0;

void connectToWiFi() 
{
    settings_t user_wifi = {};

    /* Load Wi-Fi settings */
    user_wifi = getWiFiSettings();

    Serial.print("Credentials loaded from EEPROM - SSID: ");
    Serial.print(user_wifi.ssid);
    //Serial.print("\tPassword: ");
    //Serial.println(user_wifi.password);

    if (strlen(user_wifi.ssid) > 0 && strlen(user_wifi.password) > 0) 
    {
        Serial.print("Connecting to Wi-Fi ");
        WiFi.mode(WIFI_STA);
        WiFi.begin(user_wifi.ssid, user_wifi.password);
        
        uint8_t tries = 0;
        while (WiFi.status() != WL_CONNECTED && tries < WIFI_MAX_RETRIES)
        {
            Serial.print(".");
            delay(1000);
            tries++;
        }
    }
    else
    {
        Serial.println("Error. Empty credentials.");
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        checkWiFiStatus();
        Serial.println("Failed to connect, starting AP mode.");
        startAPMode();
    } 
    else
    {
        Serial.println(" Connected!");
        onWiFiConnected();
    }
}

String getIPAddress() 
{   // Return the IP address as a string
    return WiFi.localIP().toString();
}

void startAPMode() 
{
    Serial.println("Starting AP mode...");
    WiFi.mode(WIFI_AP); //Access Point mode
    WiFi.softAP("Lionel's Smart Light", "lionajul96");

    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void onWiFiConnected() 
{
    Serial.println("Wi-Fi connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

void checkWiFiStatus()
{
    switch (WiFi.status())
    {
        case WL_IDLE_STATUS:
            Serial.println("Wi-Fi is in idle status.");
            break;
        case WL_NO_SSID_AVAIL:
            Serial.println("No SSID available. Cannot reach configured SSID.");
            break;
        case WL_CONNECT_FAILED:
            Serial.println("Wi-Fi connection failed.");
            break;
        case WL_WRONG_PASSWORD:
            Serial.println("Wi-Fi connection failed due to wrong password.");
            break;
        case WL_DISCONNECTED:
            Serial.println("Wi-Fi is disconnected.");
            break;
        case WL_CONNECTED:
            Serial.println("Wi-Fi connected successfully.");
            break;
        default:
            Serial.println("Unknown Wi-Fi status.");
            break;
    }
}

bool showWiFiSetup()
{   // If it's in AccessPoint mode or is not connected, setup-index.html file
    // must be shown to allow user set network credentials (SSID and password)
    return WiFi.getMode() == WIFI_AP || WiFi.status() != WL_CONNECTED;
}

void switchWiFiMode()
{
    // Disconnect from the AP if connected
    WiFi.softAPdisconnect();
    Serial.println("AP connection ended. Starting STA mode...");

    settings_t user_wifi = getWiFiSettings();

    if (strlen(user_wifi.ssid) > 0 && strlen(user_wifi.password) > 0) 
    {
        Serial.print("Connecting to SSID: ");
        Serial.println(user_wifi.ssid);
        WiFi.mode(WIFI_STA);
        WiFi.begin(user_wifi.ssid, user_wifi.password);
        switchWiFi_status = 1;
    }
}

void setWiFiSwitch()
{
    switchWiFi = 1;
}

uint8_t getWiFiSwitch()
{
    uint8_t ret = 0;

    if(switchWiFi)
    { 
        ret = 1; 
        switchWiFi = 0;
    }

    return ret;
}

void setWiFiSwitch_status()
{
    switchWiFi_status = 1;
}

uint8_t getWiFiSwitch_status()
{
    uint8_t ret = 0;

    if(switchWiFi_status)
    { 
        ret = 1; 
        switchWiFi_status = 0;
    }

    return ret;
}

