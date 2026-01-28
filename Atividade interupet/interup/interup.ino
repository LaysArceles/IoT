volatile int botao = 0;
hw_timer_t *timer = NULL;

int flagBotao = 0;
int tempo = 0;

void IRAM_ATTR onTimer() {
	if(flagBotao == 1) {
		if (tempo > 0) {
			tempo--;
		} else {
			flagBotao = 0;
			if (digitalRead(22)) {
				botao++;
        Serial.print("Valor do botão: ");
		    Serial.println(botao);
			}
		}
	}
}

void IRAM_ATTR Soma() {
	if (flagBotao != 1) {
		flagBotao = 1;
		tempo = 200;
	}
}

void setup() {
	pinMode(22, INPUT_PULLUP);      

	attachInterrupt(
		digitalPinToInterrupt(22), 
		Soma, 
		FALLING); 

  Serial.begin(115200);

	  timer = timerBegin(1000000);
  	timerAttachInterrupt(timer, &onTimer);                          
  	timerAlarm(timer, 1000, true, 0);
}

void loop() {
    
}
