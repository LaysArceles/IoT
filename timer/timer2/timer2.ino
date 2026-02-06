#define LED_BUILTIN 22
hw_timer_t * timer = NULL;
hw_timer_t * timer2 = NULL;
int s = 0;

void IRAM_ATTR Tempo(){
  s++;
}
void IRAM_ATTR Piscar(){
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
}

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200); 

  timer = timerBegin(1000000);
  timer2 = timerBegin(1000000);
  timerAttachInterrupt(timer, &Tempo);
  timerAlarm(timer,1000,true,0);
  
}

void loop() {
Serial.println(s);

  if ( s == 5000){
    timerAttachInterrupt(timer2, &Piscar);
    timerAlarm(timer,1000000,true,0);
  }
  if ( s == 1000){
    timerAttachInterrupt(timer2, &Piscar);
    timerAlarm(timer,5000000,true,0);
    s = 0;
  }

}