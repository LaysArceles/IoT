#ifndef ENV_H
#define ENV_H
#define WIFI_SSID "BTIA_SON5CT"
#define WIFI_PASS "IUVr4qSh8kQe"
#define LED_PIN 22                                                             //led que já vem soldado na esp32 

const char* mqtt_server    = "broker.hivemq.com";
const char* mqtt_client_id = "esp32_alunx";                                    // <---- Coloca o seu nomezinho aqui
const char* topico_comando = "bosch/ets/dta/lays/luz";
const char* topico_status  = "bosch/ets/dta/lays/status";
const char* topico_texto   = "bosch/ets/dta/lays/texto";
const int   mqtt_port      =  1883;
float frequencia;


#endif