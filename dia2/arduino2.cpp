// Código em C++ feito para piscar um LED azul e vermelho de um Arduino Uno R3
//

void setup()
{
  pinMode(10, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop()
{
  digitalWrite(8, LOW);
  digitalWrite(10, HIGH);
  delay(1000); // Esperar por 1000 milisegundos
  digitalWrite(10, LOW);
  digitalWrite(8, HIGH);
  delay(1000); // Esperar por 1000 milisegundos
}
