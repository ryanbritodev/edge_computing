#include <Servo.h>

int pinoPotenciometro = A0;
int pinoServoMotor = 5;
int valorPotenciometro = 0;
int anguloServomotor = 0;

Servo servo;

void setup(){
  servo.attach(pinoServoMotor);
  Serial.begin(9600);
}

void loop(){
  controlaServoViaPotenciometro();
  //controlaServoViaSerial();
}

void controlaServoViaPotenciometro(){
  valorPotenciometro = analogRead(pinoPotenciometro);
  anguloServomotor = map(valorPotenciometro, 0, 675, 0, 180);
  servo.write(anguloServomotor);
  Serial.print("Valor do potenciometro: ");
  Serial.print(valorPotenciometro);
  Serial.print(" | Angulo do servo motor: ");
  Serial.println(anguloServomotor);
  delay(100);
}

void controlaServoViaSerial(){
  String anguloDesejado;
  while(true){
    if(Serial.available() > 0){
      anguloDesejado = Serial.readString();
      Serial.print("Angulo desejado: ");
      Serial.println(anguloDesejado);
      if(anguloDesejado.toInt() >= 0 && anguloDesejado.toInt() <= 180){
        servo.write(anguloDesejado.toInt());
      }
    }
  }
}
