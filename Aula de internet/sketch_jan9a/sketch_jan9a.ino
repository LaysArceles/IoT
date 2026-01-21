#include "Time.h"
hw_timer_t *timer = NULL;
int tempo = 0 ; 
void IRAM_ATTR onTimer(){
  tempo ++;
}

int led_Verde = 32;
int led_V = 23; 

#include <WiFi.h> 
const char*SSID = "Miauu";
const char*password = "Soulinda";

const char* Servidor = "pool.ntp.org";
const long fuso = -10800; // -3h 3600
const int verao = 0;

void WiFiEvent(WiFiEvent_t event) {
 switch(event){
   case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("conectadinho");
      if (!time){
        digitalWrite(led_Verde,LOW);
      };
      digitalWrite(led_Verde,HIGH);
      digitalWrite(led_V,LOW);
      
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.print("desconectadinho");
      digitalWrite(led_V,HIGH);
      digitalWrite(led_Verde,LOW);
      WiFi.reconnect(); 
      break;
 }

}

void setup() {
  Serial.begin(9600);
  WiFi.onEvent(WiFiEvent);
  WiFi.begin(SSID,password);
  Serial.println("Inicicar Conexão wifi");
  configTime(fuso,verao,Servidor);

  timer = timerBegin(1000000);
  timerAttachInterrupt(timer,&onTimer);
  timerAlarm(timer,5000000,true,0);

  pinMode(led_V, OUTPUT);
  pinMode(led_Verde,OUTPUT);
  digitalWrite(led_Verde,LOW);
  digitalWrite(led_V,LOW);

}

void loop() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Sem Hora");
    return;
  }
  Serial.println(&timeinfo);
}
