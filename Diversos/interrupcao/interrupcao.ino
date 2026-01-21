hw_timer_t *timer = NULL;                                 //ponteiro para o timer
volatile bool estadoLed = false;                          //volatile fala pro compilador "nem tente otimizar a variável q ela pode mudar"

void IRAM_ATTR onTimer() {                                //faz rodar a função direto na memória ram
  estadoLed = !estadoLed;
  digitalWrite(22, estadoLed);
}

void setup() {
  Serial.begin(115200);
  pinMode(22, OUTPUT);

  // timerBegin(frequencia);
  // Colocamos 1.000.000 Hz para ter resolução de 1 microssegundo.
  timer = timerBegin(1000000);

  timerAttachInterrupt(timer, &onTimer); //vincula a interrupção ao timer criado com o nome "timer"
  
  /*
  limite: 1000000 ticks (1 segundo)  frequencia = ticks/segundo -> isolando o segundo: segundo = ticks/frequencia
  auto_reload: true (recomeça sozinho)
  repeticoes: 0 (0 = repete para sempre -> infinito)
  */
  
  timerAlarm(timer, 1000000, true, 0); 
}

void loop() {
  //nada pq vai rodar na interrupção
}