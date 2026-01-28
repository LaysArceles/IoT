int leds[] = {13,14,15,18,17,16};
int botao = 32;

hw_timer_t *timer = NULL;
volatile int click = 0;

int flag = 0;
int Tempo = 0;
int count = 0;
int countempo = 0; 
int aux = 0;
int posisao = 0;
int Anitposisao = 0;
int aux2 = 0; 

void IRAM_ATTR timerPiscar(){
  digitalWrite(leds[posisao] ,HIGH);
  digitalWrite(leds[posisao - 1 ] ,!digitalRead(leds[posisao]));  
  aux = 1;
}
void IRAM_ATTR antiorarioPiscar(){
  digitalWrite(leds[Anitposisao] ,HIGH);
  digitalWrite(leds[Anitposisao - 1 ] ,!digitalRead(leds[Anitposisao]));  
  aux2 = 1;
}
void IRAM_ATTR onTimer(){
  if(flag == 1){
    if(Tempo > 0 ){
      Tempo --;
    }else{
      flag = 0;
      if (digitalRead(botao)){
         click ++;
         Serial.println(click);
        
      }
    }
  }
}
void IRAM_ATTR funcaoTempo(){
  if (flag != 1){
    flag = 1;
    Tempo = 200;
  }
}

void setup() {
  for (int i = 0; i < 6; i++){
    pinMode(leds[i] , OUTPUT);
  }

  pinMode(botao,INPUT_PULLUP);
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(botao),funcaoTempo,FALLING);

  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer,1000,true,0);
  
}
void horario(){

  timerAttachInterrupt(timer, &timerPiscar);
  timerAlarm(timer,500000,true,0);
  timer = timerBegin(1000000);
  if (aux == 1){
    posisao ++;
    if(posisao > 6){
      posisao = 0;
      digitalWrite(leds[5] ,digitalRead(leds[posisao]));
    }
    aux = 0;
  }
  
}
void Antihorario(){
  timerAttachInterrupt(timer, &timerPiscar);
  timerAlarm(timer,500000,true,0);
  timer = timerBegin(1000000);
  if (aux == 1){
    Anitposisao ++;
    if(Anitposisao > 6){
      Anitposisao = 0;
      digitalWrite(leds[5] ,digitalRead(leds[Anitposisao]));
    }
    aux = 0;
  }
  // for (int n = 6; n > 0; n--){

  //   digitalWrite(leds[count] ,HIGH);
  //   digitalWrite(leds[count] ,LOW);
  //   count --;
  //   if (count <= 0){
  //     count = 6; 
  //   }
  // }

}

void loop() {

  horario();
  
}
