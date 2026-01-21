void setup() {
  pinMode(22, OUTPUT);          //escolhe se será pino de entrada "INPUT" ou pino de saída "OUTPUT"
  pinMode(8, OUTPUT);           //escolha um pino para ser PWM
  pinMode(10, INPUT_PULLUP);    //INPUT_PULLUP adiciona um resistor de pullup na entrada escolhida para evitar porta flutuante

  Serial.begin(115200);         //inicializa a comunicação serial com 115200 de baurate (deve ser ajustado no monitor serial)
}

void loop() {
  digitalRead(10);              //lê o estado do pino
  digitalWrite(22, 1);          //escreve no pino o estado lógico 1 = HIGH (5V/3.3V ) e 0 = LOW (0V)
  Serial.println("Estou printando o valor do pino 10: " + String(digitalRead(10)));
  analogWrite(8, 255);          //escreve um valor pseudoanalógico no pino 8 - esse valor varia de 0 até 255 (8 bits)

  analogRead(A1);               //lê o valor analógico de um pino - não é necessário setar como INPUT, mas é uma boa prática
}
