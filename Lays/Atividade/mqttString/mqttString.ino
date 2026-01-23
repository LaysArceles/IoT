/**
 * @author    patrosque
 * @brief     Cliente MQTT Industrial para IoT (ESP32).
 * Implementa protocolo Pub/Sub com LWT (Last Will),
 * reconexão não-bloqueante e identidade única via MAC.
 * @version   2.1.1 
 * @date      2026-01-19
 * @history
 * v1.0.0 - 2026-01-10 - Implementação básica do conexão MQTT
 * v1.1.0 - 2026-01-12 - Adição comunicação string unidirecional
 * v1.2.1 - 2026-01-19 - Organização em pastas
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"
int Green = 26;
int Red = 23;
int Blue = 32;

unsigned long tempo = 0;
bool isConnected = false;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);      
  pinMode(Blue, OUTPUT);

  digitalWrite(LED_PIN, HIGH);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  configWifi();      
  configuraTimer(); 
}

void loop() {



  taConectado();
  

}