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

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("MSG recebida no tópico: ");
  Serial.println(topic);

  String msg = "";
  for (unsigned int i = 0; i < length; i++) {                     //transforma o payload recebido do mqtt para string
    msg += (char)payload[i];
  }

  Serial.println("Conteudo: " + msg);

  //logica de controle para ligar o led usando o servidor
  if (msg == "LIGA") {                                                            
    digitalWrite(LED_PIN, 0); 
    client.publish(topico_status, "LIGADO", true);
  } 
  else if (msg == "DESLIGA") {                                                    
    digitalWrite(LED_PIN, 1);
    client.publish(topico_status, "DESLIGADO", true);
  }
}


boolean attemptMqttConnection() {                                     //função de Tentativa de Conexão (Retorna true/false, não trava o código)
  
  
  String clientId = String(mqtt_client_id) + "_" + String(WiFi.macAddress()); //gera ID Único (Evita colisão entre alunos)
  
  Serial.print("Tentando MQTT como: " + clientId + "... ");

  if (client.connect(
        clientId.c_str(),                                               //ID DO CLIENTE: Quem sou eu? (Ex: "ESP32_Patrick_MAC")
        NULL,                                                           //USUÁRIO: Tem login? (NULL pois o HiveMQ é público/anônimo)
        NULL,                                                           //SENHA: Tem senha? (NULL pois o HiveMQ é público/anônimo)
        topico_status,                                                  //TÓPICO DO TESTAMENTO (LWT): Onde publicar se a conexão cair?
        0,                                                              //QoS DO TESTAMENTO: Qual a prioridade? (0 = mais rápido/leve)
        true,                                                           //RETAIN (FIXAR): true = A mensagem fica "colada" no tópico
        "OFFLINE"                                                       //MENSAGEM DO TESTAMENTO: O que o Broker deve gritar por mim?
     )) {
    
    Serial.println("CONECTADO!");
    client.publish(topico_status, "ONLINE", true);                      //avisa que está vivo (Sobrescreve o Offline anterior)
    client.subscribe(topico_comando);                                   //assina o tópico de comando
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