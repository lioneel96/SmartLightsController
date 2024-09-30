#include <websocket_manager.h>
#include <lights_manager.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

AsyncWebSocket ws("/ws");

void initWebSocket(AsyncWebServer *server) 
{
    ws.onEvent(onEventHandler);  // Set WebSocket event handler
    server->addHandler(&ws);  // Attach WebSocket to the server
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) 
{
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        String message = String((char*)data);
        //Serial.println("Received: " + message);

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, message);

        if (!error) 
        {   // Update the LED color using the RGB values
            int r = doc["r"];
            int g = doc["g"];
            int b = doc["b"];
            Serial.printf("RGB values - R: %d, G: %d, B: %d\n", r, g, b);
            setLightsColor(r, g, b);
        }
        else
        {
            Serial.println("Failed to parse JSON");
        }
    }
}

void onEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    switch (type) 
    {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}