#include "Time.h"
#include <WiFi.h>

hw_timer_t *timer = NULL;
int tempo = 0 ;
int led_Verde = 23;
int led_V = 19;

void IRAM_ATTR offTimer(){
  if(tempo == 5){
    digitalWrite(led_V,!digitalRead(led_V));
    digitalWrite(led_Verde, 0);
    tempo = 0;
  }
  tempo ++; 
}
void IRAM_ATTR onTimer(){
  digitalWrite(led_Verde,!digitalRead(led_Verde));
  digitalWrite(led_V, 0);
}


const char*SSID = "Miauu";
const char*password = "Soulinda";

const char* Servidor = "pool.ntp.org";
const long fuso = -10800; // -3h 3600
const int verao = 0;

void WiFiEvent(WiFiEvent_t event) {
 switch(event){
   case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("conectadinho");
      timerAttachInterrupt(timer,&onTimer);
      timerAlarm(timer,100000,true,0);
      

      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.print("desconectadinho");
      timerAttachInterrupt(timer,&offTimer);
      timerAlarm(timer,100000,true,0);   
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

  pinMode(led_V, OUTPUT);
  pinMode(led_Verde,OUTPUT);
}


void loop() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Sem Hora");
    return;
  }
}
