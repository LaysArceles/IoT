/**
 * @author    patrosque
 * @brief     API RESTful Assíncrona para Dashboard Industrial.
 * Implementa endpoints JSON para leitura de sensores e
 * controle de atuadores via protocolo HTTP.
 * * @version   1.2.0
 * @date      2026-01-15
 * * @history
 * v1.0.0 - 2026-01-10 - Implementação básica do WebServer.
 * v1.1.0 - 2026-01-12 - Adição da biblioteca ArduinoJson e endpoint /status.
 * v1.2.0 - 2026-01-15 - Implementação de CORS, WiFiEvent e Hardware Loop não-bloqueante.
 */

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h> 
#include "env.h"    

#define LED_PIN 22                                           

AsyncWebServer server(80);                                      //Cria uma nova instância do objeto e inicia na porta 80 (padrão mundia para http)

float temperaturaAtual = 0.0;                                   //variável para guardar os valores de temperatura
bool motorLigado = false;                                       //variável para guardar o valor de ligado/desligado

void lerSensores() {                                            //Função para simular os valores de temperatura (vocês deverão substituir por uma leitura real)
  temperaturaAtual = random(200, 450) / 10.0;
}

void WiFiEvent(WiFiEvent_t event) {                                           //callback para tentar reconectar no wifi                                
    switch(event) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP: 
            Serial.println("conectado no wifi! IP: " + WiFi.localIP().toString());               
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("desconectado do wifi, tentando novamente...");
            WiFi.reconnect();                                                 // Tenta voltar sozinho
            break;
    }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); 
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);                            //Inicia o wifi com os argumentos do arquivo "env.h"
  
  Serial.print("Conectando ao Wi-Fi...");
  criaServidor();
}

void loop() {
  //n precisa de nada pois tá tudo com "callback"
}

void criaServidor(){
  /*
  DefaultHeaders -> classe definida em "ESPAsyncWebServer"
  Instance()     -> retorna a instancia do objeto (design pattern ->> singleton)
  .addHeader     -> chama o método dele 
  Access-Control-Allow-Origin     ->> é uma string do protocolo global http que significa "permitir a origem de..."
  como usamos "*" dentro do método, signfica que não temos restrição de origem, então qualquer um pode ler a resposta (acessar os dados)

  do mesmo modo, "Access-Control-Allow-Methods" permitir que a origem utilize esses métodos, ai restringimos para GET, POST e PUT
  */
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");

  server.on("/api/status", HTTP_GET, [](AsyncWebServerRequest *request){  //define Rota + Verbo (GET)
 
    lerSensores();                                                        //atualiza as variáveis globais AGORA 
    
    JsonDocument doc;                                                     // instancia o objeto manipulador de JSON (Gerencia alocação de memória).
    
    doc["temp"] = temperaturaAtual;                                       // a biblioteca detecta o tipo da variável automaticamente.
    doc["rele"] = motorLigado;                                            // booleanos são convertidos automaticamente para "true"/"false".
    
    doc["ip"] = WiFi.localIP().toString();                                //IP da placa para debug.
    doc["uptime"] = millis() / 1000;                                      //Tempo de atividade em segundos.
    doc["memoria_livre"] = ESP.getFreeHeap();                             //Monitoramento de RAM livre

    /* visualização do objeto "doc"
       {
         "temp": 24.5,
         "rele": true,
         "ip": "192.168.0.105",
         "uptime": 150,
         "memoria_livre": 200000
       }
    */

    String response;                                                    // String vazia que servirá de buffer.
    
    serializeJson(doc, response);                                       //Transforma o Objeto C/C++ em Texto
    
    /*
    Envia o pacote HTTP de volta:
    - Status 200: OK (Sucesso).
    - "application/json": Avisa o cliente para tratar como Objeto, não como HTML.
    - Payload: O texto JSON gerado acima.
    */
    request->send(200, "application/json", response);
  });

  server.on("/api/control", HTTP_GET, [](AsyncWebServerRequest *request){
    
    //verifica se o parâmetro "state" existe na URL para evitar erros de ponteiro nulo
    if (request->hasParam("state")) {
      
      
      String valor = request->getParam("state")->value();            // Busca o valor associado à chave "state". O operador "->" acessa o método do objeto apontado
      
      if (valor == "on") {
        motorLigado = true;                                          // Atualiza estado lógico (Memória)
        digitalWrite(LED_PIN, 0);                                    // Atualiza estado físico (Hardware)
        Serial.println("CMD: Motor LIGADO");                         // "motor" liga com 0, pois usei o LED buildin pra representar e ele tem lógica invertida
      } 
      else if (valor == "off") {
        motorLigado = false;
        digitalWrite(LED_PIN, 1);
        Serial.println("CMD: Motor DESLIGADO");
      }
      
      
      request->send(200, "text/plain", "OK");                       // HTTP 200 (OK): Confirma para o Frontend que o comando foi processado
    } 
    else {
      request->send(400, "text/plain", "Erro: Parametro 'state' obrigatorio");  // HTTP 400 (Bad Request): O cliente fez uma requisição malformada (esqueceu o parâmetro).
    }
  });

  /*isso aqui só serve pra avisar quem acessar "192.168.1.0/" que na verdade o correto é "192.168.1.0/api/status"
  */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){                  
    request->send(200, "text/plain", "API bigode online. Acesse /api/status no seu Dashboard.");
  });
  server.begin();                                                   //logicamente inicia o servidor
}