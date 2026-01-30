volatile int contador = 0;
int on = 0;
volatile int somar = 1;
int pinos[] = {23,21,19,5,18,15};
volatile int botao = 4;
volatile int flagBotao = 0;
volatile int tempo = 0;
int click = 0;
hw_timer_t* timer = NULL;

void IRAM_ATTR FuncaoBoa(){
  if(flagBotao != 1 ){
    flagBotao = 1,
    tempo = 50;
  }
}

void IRAM_ATTR onTimer(){
  contador++;
  if(flagBotao == 1){
    if(tempo > 0){
      tempo--;
    }else{
      flagBotao = 0;
      if(!digitalRead(botao)){
        click = click + 1;
      }
    }
  }
}

void setup() {
  for(int i = 0; i < 6; i++){
    pinMode(pinos[i], OUTPUT);
  }

  pinMode(botao , INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(botao),
    FuncaoBoa,
    FALLING);
  timer = timerBegin(1000000); 
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);
  Serial.begin(115200);
}

void Horario(){
  digitalWrite(pinos[on], HIGH);
  if (contador >= 2000){
    contador = 0;
    digitalWrite(pinos[on],LOW );
    on = on - somar;
    if (on < 0) on = 6;
    if (on > 6) on = 0;
  }
}

void AntiHorario(){
  digitalWrite(pinos[on], HIGH);
  if (contador >= 2000){
    contador = 0;
    digitalWrite(pinos[on],LOW );
    on = on + somar;
    if (on < 0) on = 6;
    if (on > 6) on = 0;
  }
}

void loop() {
  if(click == 1){
   AntiHorario();
  }
  if(click == 2){
  Horario();
  click = 0;
  } 
  else{
  Horario();
  } 
    
  Serial.println(click);

}
