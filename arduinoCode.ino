int ledR = 9;
int ledG = 10;
int ledB = 11;

void setup() {
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
}

void loop() {
  analogWrite(ledR, 255); // vermelho 100%
  analogWrite(ledG, 128); // verde ~50%
  analogWrite(ledB, 0);   // azul apagado
  delay(1000);
}
