// https://wokwi.com/projects/408557550221920257

#include <DHT.h>
/* 

------------------ FIAP --------------------
CHECKPOINT 4
EDGE COMPUTING & COMPUTER SYSTEMS
Participantes:
Prof. Paulo Marcotti PF2150
Arthur Cotrick Pagani RM554510
--------------------------------------------
-------------- EXPLICAÇÃO ------------------

O protótipo realiza a medição tanto da 
umidade quanto da temperatura do ambiente e
ilustra-os em uma fita de led com um sentido
percentual. 

Com as informações captadas, realiza uma média,
ponderada, com os valores, sendo que,
a temperatura tem um peso de 3 na conta, enquanto
que a umidade, um peso de 2.

Feito o cálculo por peso, obtém-se um valor
que é remapeado para um intervalo de 0 até o
número de leds contidos, neste caso, 10.

Assim, com o remapeamento e a lógica necessária,
este "progresso" dos valores de umidade e temperatura
são mostrados na fita.
--------------------------------------------

*/
#define DHTPIN 27
#define DHTTYPE DHT22

const int ledCount = 10;
int ledPins[] = {
  12, 13, 14, 15, 16, 17, 18, 19, 21, 22
};

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  for (int ledAtual = 0; ledAtual < ledCount; ledAtual++)
  {
    pinMode(ledPins[ledAtual], OUTPUT);
  }
  dht.begin();
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (isnan(temperatura) || isnan(umidade))
  {
    Serial.println("DHT22 apresentou um erro de leitura, valores não serão computados");
  }
  else
  {
    int temperaturaPeso = temperatura * 3;
    int umidadePeso = umidade * 2;
    int mediaPonderadaValores = floor((temperaturaPeso + umidadePeso) / 5);
    
    int ledsLevel = map(mediaPonderadaValores, -24, 88, 0, ledCount);

    for (int ledAtual = 0; ledAtual < ledCount; ledAtual++)
    {
      if (ledAtual < ledsLevel)
      {
        digitalWrite(ledPins[ledAtual], HIGH);
      }
      else
      {
        digitalWrite(ledPins[ledAtual], LOW);
      }
    }
  }
  delay(2000);
}
