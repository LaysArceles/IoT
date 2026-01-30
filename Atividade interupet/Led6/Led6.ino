int leds[] = {23,21,19,5,18,15};
int botao = 4;

hw_timer_t *timer = NULL;
hw_timer_t *timerH = NULL;
hw_timer_t *timerA = NULL;
int flag = 0;
int Tempo = 0;
volatile int click = 0;

// tempo dos piscas piscas
int posicaoH = 0;
int aux1 = 0;
int posicaoA = 6;
int aux2 = 0;

void IRAM_ATTR blickHorario(){
  digitalWrite(leds[posicaoH],HIGH);
  digitalWrite(leds[posicaoH - 1], LOW);
  aux1 = 1;
}
void IRAM_ATTR blickAntihorario(){
  digitalWrite(leds[posicaoA ], LOW);
  digitalWrite(leds[posicaoA -1 ], HIGH);
  aux2 = 1;
}

void IRAM_ATTR onTime(){
  if (flag == 1){
    if(Tempo > 0){
      Tempo --;
    }
    else{
      flag = 0;
      if(digitalRead(botao)){
        click ++;
        Serial.println(click);
      }
    }
  }
}

void IRAM_ATTR funcaoCount(){
  if(flag != 1){
    flag = 1;
    Tempo = 200;
  }
}
void setup() {
  Serial.begin(9600);
  pinMode(botao, INPUT_PULLUP);
  for (int i = 0; i < 6 ; i++){
    pinMode(leds[i], OUTPUT);
  }

  attachInterrupt( digitalPinToInterrupt(botao),funcaoCount,FALLING);
  timer = timerBegin(1000000);
  timerA = timerBegin(1000000);
  timerH = timerBegin(1000000);

  timerAttachInterrupt(timer,&onTime);
  timerAlarm(timer,1000,true,0);


}
void funHorario(){
 
  timerAttachInterrupt(timerH,&blickHorario);
  timerAlarm(timerH,1000000,true,0);
  if(click == 2 ){
   posicaoA = posicaoH;
   digitalWrite(leds[posicaoH], LOW);
    return;
  }
  if (aux1 == 1){
    posicaoH ++;
      if (posicaoH > 6 ){
        posicaoH = 0;
      }
    aux1 = 0;
  }
}
void funAntihorario(){
  timerAttachInterrupt(timerA,&blickAntihorario);
  timerAlarm(timerA,1000000,true,0);
 
  if(click == 2){
    posicaoH = posicaoA;
    digitalWrite(leds[posicaoH-1], LOW);
    return;
  }
  if (aux2 == 1){
    posicaoA --;
      if (posicaoA <  0 ){
        posicaoA = 6;
      }
    aux2 = 0;
  }
}

void loop() {

  switch (click){
    case 0:
      funHorario();
    case 1 :
      funAntihorario();
      break;
    case 2 :
      funHorario();
      click = 0;
      break;
    default:
      break;
  }

}

