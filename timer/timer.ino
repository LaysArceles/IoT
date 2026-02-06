hw_timer_t * timer = NULL;
int B = 0;
int A = 0;

void IRAM_ATTR ontimer(){
  B++;
}

void setup() {
  Serial.begin(115200);
  timer = timerBegin(1000000);
  timerAttachInterrupt(timer, &ontimer);
  timerAlarm(timer,3250000,true,0);
}

void loop() {
  A++;
  if ( ){
    Serial.println("A: " + String(A));
    Serial.println("B: " + String(B));
  }
}
