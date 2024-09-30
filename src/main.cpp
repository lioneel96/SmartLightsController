#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include <wifi_manager.h>
#include <server_routes.h>
#include <filesystem_manager.h>
#include <eeprom_manager.h>
#include <websocket_manager.h>
#include <lights_manager.h>

#include <user_interface.h>  // Required for os_timer functions
void myTask(void *arg); // Function that will be called periodically
os_timer_t myTimer;

uint8_t retryCounter = 0;

AsyncWebServer server(80);

void setup() 
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("Starting app...");

  initLEDs();

  os_timer_setfn(&myTimer, (os_timer_func_t *)myTask, NULL);
  os_timer_arm(&myTimer, 1000, true);

  initializeEEPROM();

  // Initialize filesystem (LittleFS)
  if ( !mountFilesystem() ) return;

  // List files in LittleFS
  //listDir("/", 0);  // List all files at the root directory
  
  connectToWiFi();

  setupRoutes(&server); // Set up routes for HTTP server
    
  server.begin(); // Start the server

  initWebSocket(&server); // Initialize WebSocket
}

void loop()
{

}


void myTask(void *arg) 
{
  if( getWiFiSwitch() )
  {
    switchWiFiMode();
  }

  if( getWiFiSwitch_status() )
  {
    if( showWiFiSetup() ) // aka: is not conected
    {
      if( retryCounter < 30)
      {
        Serial.print(".");
        setWiFiSwitch_status();
        retryCounter++;
      }
      else
      {
        checkWiFiStatus();
        Serial.println("Max. retry number exceded. Redeploying AP.");
        startAPMode();
        retryCounter = 0;
      }
    }
    else
    {
      Serial.println(" connected!");
      checkWiFiStatus();
      retryCounter = 0;
      Serial.print("STA IP Address: ");
      Serial.println( getIPAddress() );
    }
  }
}