#include "env.h"

extern unsigned long tempo;
extern bool isConnected;
hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {
  if (!isConnected) tempo++;
}

void configuraTimer() {
  timer = timerBegin(1000000); 
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000, true, 0);
}