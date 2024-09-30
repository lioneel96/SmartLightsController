#include <server_routes.h>
#include <eeprom_manager.h>
#include <wifi_manager.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

void setupRoutes(AsyncWebServer *server) 
{
    // Serve static files
    server->serveStatic("/colorjoe/", LittleFS, "/colorjoe/");
    server->serveStatic("/icons/", LittleFS, "/icons/");
    server->serveStatic("/styles.css", LittleFS, "/styles.css");
    server->serveStatic("/rgbSelector.js", LittleFS, "/rgbSelector.js");

    server->on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/favicon.ico", "image/x-icon");
        request->send(response);
    });

    // Handle GET requests
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {

        // Check if the ESP is in AP mode or connected (STA mode)
        if (showWiFiSetup())
        {
            // In AP mode (or not connected), serve setup-index.html
            AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/setup-index.html", "text/html");
            request->send(response);
        }
        else 
        {
            // In STA mode (connected), serve the normal index.html
            AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/index.html", "text/html");
            request->send(response);
        }

    });

    // Handle POST requests
    server->on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
        settings_t user_wifi;

        if (request->hasArg("ssid") && request->hasArg("password")) 
        {
            strncpy(user_wifi.ssid, request->arg("ssid").c_str(), sizeof(user_wifi.ssid));
            strncpy(user_wifi.password, request->arg("password").c_str(), sizeof(user_wifi.password));
            saveWiFiSettings(user_wifi);
            AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/setup-complete.html", "text/html");
            request->send(response);
        }
        else
        {
            Serial.println("No SSID or Password found.");
        }
    });

    // Handle the Wi-Fi mode switch request
    server->on("/switchWiFiMode", HTTP_GET, [](AsyncWebServerRequest *request) {
        setWiFiSwitch(); // Set falg to switch from AP to STA mode
        request->send(200, "text/plain", "Wi-Fi mode switched to STA.");
    });

    // Handle the IP address request
    server->on("/getIP", HTTP_GET, [](AsyncWebServerRequest *request){
        String ip = getIPAddress();
        request->send(200, "text/plain", ip);  // Send the IP address as a plain text response
    });

    server->on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
        ESP.restart();
        request->send(200, "text/plain", "Restarting...");
    });
    
    // Serve static CSS file
    server->on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/styles.css", "text/css");
        request->send(response);
    });

    // Serve icons from LittleFS
    server->on("/icons/icon-wifi.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/icons/icon-wifi.png", "image/png");
        request->send(response);
    });

    server->on("/icons/icon-password.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/icons/icon-password.png", "image/png");
        request->send(response);
    });

    server->on("/icons/icon-save.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/icons/icon-save.png", "image/png");
        request->send(response);
    });
}