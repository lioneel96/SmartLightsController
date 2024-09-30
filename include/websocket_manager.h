#ifndef WEBSOCKET_MANAGER_H
#define WEBSOCKET_MANAGER_H

#include <ESPAsyncWebServer.h>

void initWebSocket(AsyncWebServer *server);
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);

#endif // WEBSOCKET_MANAGER_H