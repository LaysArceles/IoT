/**
 * @author    patrosque
 * @brief     API RESTful Assíncrona para Dashboard Industrial.
 * Implementa endpoints JSON para leitura de sensores e
 * controle de atuadores via protocolo HTTP.
 * * @version   1.0.0
 * * @date    2026-01-15
 * * @history
 * v1.0.0 - 2026-01-15 - Implementação básica de leitura e escrita em tópicos usando MQTT.
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"

WiFiClient espClient;
PubSubClient client(espClient);


void WiFiEvent(WiFiEvent_t event) {                                              //callback para tentar reconectar no wifi                                
    switch(event) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP: 
            Serial.println("conectado no wifi! IP: " + WiFi.localIP().toString());               
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("desconectado do wifi, tentando novamente...");
            WiFi.reconnect();                                                    // Tenta voltar sozinho
            break;
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem chegou no tópico: ");
  Serial.println(topic);

  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  Serial.print("Conteudo: " + String(msg));

  if (msg == "LIGA") {                                                            //se msg for "LIGA"
    digitalWrite(LED_PIN, 0);                                                   
    client.publish(topico_status, "O LED foi ligado!"); 
  } 
  else if (msg == "DESLIGA") {                                                    //se msg for "DESLIGADO"
    digitalWrite(LED_PIN, 1);
    client.publish(topico_status, "O LED foi desligado!");
  }
}

void reconnect() {
  while (!client.connected()) {                                                   // Loop até conectar no MQTT
    Serial.println("Tentando conectar ao MQTT Broker...");
    
    if (client.connect(mqtt_client_id)) {                                         // Tenta conectar com ID único
      Serial.println("Conectado!");
      client.subscribe(topico_comando);  
    } else {
      Serial.print("Falhou, rc=" + client.state());
      Serial.println(" tentando em 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);                                               //Inicia o wifi com os argumentos do arquivo "env.h"
  Serial.print("Conectando ao Wi-Fi...");
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);                                                   // Define quem escuta
}

void loop() {
  if (!client.connected())
    reconnect();
  client.loop();                                                                  // Mantém a conexão viva (Keep Alive)
}
