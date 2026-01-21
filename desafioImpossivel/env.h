#ifndef ENV_H
#define ENV_H

#define WIFI_SSID "BTIA_SON5CT"
#define WIFI_PASS "IUVr4qSh8kQe"
#define LED_PIN 22                                                             //led que já vem soldado na esp32 

const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_client_id = "ESP32_patrick"; 

const char* topico_comando = "desafio/impossivel"; // Comando 1 ou 0
const char* topico_id  = "desafio/id"; // Ler e passar pro prox ID
const char* topico_nomes  = "desafio/nomes"; // Lista de Nomes

const int   mqtt_port = 1883;

const int   meu_id = 9; // TROCAR SEU ID AQUI
const char* meu_nome = "Lays";

#endif