#include <WiFi.h>
#include "env.h"

void WiFiEvent(WiFiEvent_t event) {                                              
    switch(event) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP: 
            Serial.println("WiFi Conectado! IP: " + WiFi.localIP().toString());
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("WiFi perdido. O driver tentará reconectar sozinho...");
            break;
    }
}

void configWifi(){
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS); 
}
  