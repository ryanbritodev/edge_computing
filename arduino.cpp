// Código em C++ feito para piscar um LED de um Arduino Uno R3 feito no site Tinkercad
//
void setup()
{
  pinMode(10, OUTPUT);
}

void loop()
{
  digitalWrite(10, HIGH);
  delay(500); // Esperar por 500 milisegundos
  digitalWrite(10, LOW);
  delay(500); // Esperar por 500 milisegundos
}
