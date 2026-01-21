/**
 * @author    patrosque
 * @brief     Cliente MQTT Industrial para IoT (ESP32).
 * Implementa protocolo Pub/Sub com LWT (Last Will),
 * reconexão não-bloqueante e identidade única via MAC.
 * @version   2.0.0 (Migração HTTP -> MQTT)
 * @date      2026-01-15
 */


#include <WiFi.h>
#include <PubSubClient.h>
#include "env.h"

WiFiClient espClient;
PubSubClient client(espClient);

hw_timer_t *timer = NULL;
unsigned long tempo = 0;
bool isConnected = false;

//callback do wifi
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

int estaLigado = 0;
int nomeEnviado = 0;
int souEu = 0;

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Lendo Tópico: ");
  Serial.println(topic);
  String guardar = String(topic);

  // LE O QUE TA ESCRITO NO TOPICO
  String msg = "";
  for (unsigned int i = 0; i < length; i++) {                     //transforma o payload recebido do mqtt para string
    msg += (char)payload[i];
  }
  //

  if (String(topic) == topico_comando){
    //Serial.print("Lendo Tópico DENTRO DO IF: ");
    Serial.println(topic);
    if (msg == "1") {           
        //Serial.print("Lendo Tópico DENTRO DO IF 2: ");
        Serial.println(topic);                                                 
        digitalWrite(LED_PIN, 1); 
        estaLigado = 1;
        Serial.print("LIGOU: ");
        Serial.println(estaLigado);
        client.publish(topico_id, "0", true);
      } 
      else if (msg == "0") {                      
        // AGUARDE SER 1                              
        digitalWrite(LED_PIN, 0);
        estaLigado = 0;
        Serial.println("DESLIGOU");
      }
  }

  // VERIFICA SE É TOPICO DE ID E SE TA LIGADO
  Serial.print("Topico Atual: ");
  Serial.println(guardar);

  if ((String(guardar) == String(topico_id)) && (estaLigado == 1)){
    //Serial.println("Entrando em Tópico ID");
    if(msg == String(meu_id - 1)){    

      souEu = 1;
      Serial.println("SOU EU!");
      client.publish(topico_nomes, "", true);
    }
  }

  if((String(topic) == String(topico_nomes)) && (souEu == 1)){

    msg += meu_nome;

    client.publish(topico_nomes, msg.c_str(), true);
    client.publish(topico_id, String(meu_id).c_str(), true);
    souEu = 0;

    // Serial.println("Meu ID: ");
    // Serial.print(meu_id);
    Serial.println("Nome Registrado: ");
    Serial.print(msg);
  }
}

boolean attemptMqttConnection() {                                     //função de Tentativa de Conexão (Retorna true/false, não trava o código)
 
  String clientId = String(mqtt_client_id) + "_" + String(WiFi.macAddress()); //gera ID Único (Evita colisão entre alunos)
  
  Serial.print("Tentando MQTT como: " + clientId + "... ");

  if (client.connect(
        clientId.c_str(),                                               //ID DO CLIENTE: Quem sou eu? (Ex: "ESP32_Patrick_MAC")
        NULL,                                                           //USUÁRIO: Tem login? (NULL pois o HiveMQ é público/anônimo)
        NULL,                                                           //SENHA: Tem senha? (NULL pois o HiveMQ é público/anônimo)
        topico_comando,                                                  //TÓPICO DO TESTAMENTO (LWT): Onde publicar se a conexão cair?
        0,                                                              //QoS DO TESTAMENTO: Qual a prioridade? (0 = mais rápido/leve)
        true,                                                           //RETAIN (FIXAR): true = A mensagem fica "colada" no tópico
        "OFFLINE"                                                       //MENSAGEM DO TESTAMENTO: O que o Broker deve gritar por mim?
     )) {
    
    Serial.println("CONECTADO!");
    client.subscribe(topico_comando);                                   //assina o tópico de comando
    client.subscribe(topico_id);    
    client.subscribe(topico_nomes);    
    return true;
  } 
  else {
    Serial.print("Falha. rc=");
    Serial.print(client.state());
    return false;
  }
}

void IRAM_ATTR onTimer() {
  if (!isConnected)                                                      //interrupção a cada 1ms, só incrementa se não estiver conectado
    tempo++;
}

void setup() {

  Serial.begin(115200);

  //configuração dos pinos que serão controlados pelo servidor
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  //configuração do wifi
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);                                               
  
  //configuração do mqtt
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);         

  //configuração do timer
  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);                          
  timerAlarm(timer, 1000, true, 0);                                 
}

void loop() {
  taConectado();
}

void taConectado(){
if (!client.connected()) {
    isConnected = false;
    if (tempo > 5000) {                                     // Só tenta conectar se já passaram 5 segundos desde a última vez
      tempo = 0;
      if (attemptMqttConnection())                          // Tenta conectar. Se falhar, o loop continua rodando (não trava o Wi-Fi)
        tempo = 0;
    }
  } 
  else {
    isConnected = true;
    client.loop();                                          // Se está conectado, mantém o Keep Alive
  }
}