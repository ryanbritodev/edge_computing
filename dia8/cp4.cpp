// https://wokwi.com/projects/408488648836828161

/* 
------------------------- FIAP -------------------------
CHECKPOINT 4
EDGE COMPUTING & COMPUTER SYSTEMS
Participantes:
Prof. Paulo Marcotti PF2150
Arthur Cotrick Pagani RM554510
Diogo Leles Franciulli RM558487
Felipe Sousa de Oliveira RM559085
Ryan Brito Pereira Ramos RM554497
--------------------------------------------------------

---------------------- EXPLICAÇÃO ----------------------

Este código realiza o controle de um Servomotor com base
nas leituras de três sensores: LDR (luminosidade), 
Potenciômetro e um DHT22 (temperatura e umidade). A cada ciclo,
o sistema lê os valores de luminosidade (LDR), temperatura, 
umidade e do potenciômetro, e os remapeia para valores entre 0 e 45 graus, 
contabilizando as três últimas leituras feitas e (assim como o tempo).
O ângulo total do Servomotor é obtido somando os ângulos 
mapeados das leituras de luminosidade, temperatura, umidade 
e potenciômetro. O Servomotor se movimenta para o ângulo 
correspondente, com um limite máximo de 180°. O código
calcula e imprime a variação dos valores lidos (derivadas) para 
cada sensor, utilizando a fórmula (V3 – V1) / (T3 – T1), que
mede a variação em relação ao tempo. Todos os valores das
leituras atuais, as médias, e as derivadas são impressos no 
monitor serial a cada 2 segundos para acompanhamento em tempo
real. Além de monitorar valores instantâneos, também se monitora
a variação ao longo do tempo, calculando as derivadas para 
avaliar a velocidade de mudança de cada grandeza.

--------------------------------------------------------
*/

#include <DHT.h> // Bilblioteca do DHT22
#include <ESP32Servo.h> // Biblioteca para controlar o Servomotor no ESP32, não utilizar a Servo como a do Arduino

// Pinos dos sensores (Leitura de 4 valores de entrada)
int pinoLDR = 34; // Pino do LDR
int pinoPot = 35; // Pino do Potenciômetro
#define DHTPIN 33 // Pino do DHT22
#define DHTTYPE DHT22 // Tipo de DHT (DHT22)
int pinoServo = 4; // Pino do Servomotor

// Inicialização do DHT22
DHT dht(DHTPIN, DHTTYPE);

// Inicialização do Servomotor
Servo meuServo;

// Variáveis para armazenar as últimas 3 leituras de cada grandeza (LDR, Potenciômetro, Temperatura, Umidade)
int valorLDR1 = 0, valorLDR2 = 0, valorLDR3 = 0;
int valorPot1 = 0, valorPot2 = 0, valorPot3 = 0;
float temp1 = 0, temp2 = 0, temp3 = 0;
float umid1 = 0, umid2 = 0, umid3 = 0;

// Armazenamento dos tempos de leitura (em milissegundos)
unsigned long tempoLDR1 = 0, tempoLDR2 = 0, tempoLDR3 = 0;
unsigned long tempoPot1 = 0, tempoPot2 = 0, tempoPot3 = 0;
unsigned long tempoTemp1 = 0, tempoTemp2 = 0, tempoTemp3 = 0;
unsigned long tempoUmid1 = 0, tempoUmid2 = 0, tempoUmid3 = 0;

void setup() {
  Serial.begin(115200); // Inicializa o monitor serial com baud rate do ESP32 de 115200 como orientado em sala de aula
  dht.begin(); // Inicializa o DHT22
  
  meuServo.attach(pinoServo, 500, 2400); // Conecta o Servomotor ao pino e define o range dos ângulos (0° a 180° graus)
}

void loop() {
  // Leitura dos sensores
  int valorLDR = analogRead(pinoLDR); // Luminosidade
  float temperatura = dht.readTemperature(); // Temperatura
  float umidade = dht.readHumidity(); // Umidade
  int valorPotenciometro = analogRead(pinoPot); // Potenciômetro

  // Mapeia o valor do LDR de 0 a 1000
  int luminosidadeMapeada = map(valorLDR, 0, 4095, 1000, 0);

  // Armazena o tempo atual para todas as grandezas
  unsigned long tempoAtual = millis();

  // Realizando o reajuste das últimas 3 leituras de cada valor e de seu respectivo tempo
  // LDR
  valorLDR3 = valorLDR2;
  valorLDR2 = valorLDR1;
  valorLDR1 = luminosidadeMapeada;
  tempoLDR1 = tempoLDR2;
  tempoLDR2 = tempoLDR3;
  tempoLDR3 = tempoAtual;

  // Potenciômetro
  valorPot3 = valorPot2;
  valorPot2 = valorPot1;
  valorPot1 = valorPotenciometro;
  tempoPot1 = tempoPot2;
  tempoPot2 = tempoPot3;
  tempoPot3 = tempoAtual;

  // Temperatura
  temp3 = temp2;
  temp2 = temp1;
  temp1 = temperatura;
  tempoTemp1 = tempoTemp2;
  tempoTemp2 = tempoTemp3;
  tempoTemp3 = tempoAtual;

  // Umidade
  umid3 = umid2;
  umid2 = umid1;
  umid1 = umidade;
  tempoUmid1 = tempoUmid2;
  tempoUmid2 = tempoUmid3;
  tempoUmid3 = tempoAtual;

  // Verificando se o sensor DHT22 falhou, para fins de depuração na hora da montagem
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha na leitura do sensor DHT!");
    return;
  }

  // Média das últimas 3 leituras de luminosidade
  int mediaLuminosidade = (valorLDR1 + valorLDR2 + valorLDR3) / 3;

  // Mapeia as grandezas para o intervalo de 0 a 45 graus (25% de 180°)
  int anguloLuminosidade = map(mediaLuminosidade, 0, 1000, 0, 45);
  int anguloTemperatura = map(temperatura, -40, 80, 0, 45);
  int anguloUmidade = map(umidade, 0, 100, 0, 45);
  int anguloPotenciometro = map(valorPotenciometro, 0, 4095, 0, 45);

  // Soma dos ângulos mapeados das 4 entradas
  int anguloServo = anguloLuminosidade + anguloTemperatura + anguloUmidade + anguloPotenciometro;

  // Move o Servomotor para o ângulo calculado (resultado da soma acima)
  meuServo.write(anguloServo);

  // Cálculo das derivadas (V3 – V1) / (T3 – T1) com conversão do tempo do millis em segundos
  float derivadaLDR = (valorLDR3 - valorLDR1) / ((tempoLDR3 - tempoLDR1) / 1000.0);
  float derivadaPot = (valorPot3 - valorPot1) / ((tempoPot3 - tempoPot1) / 1000.0);
  float derivadaTemp = (temp3 - temp1) / ((tempoTemp3 - tempoTemp1) / 1000.0);
  float derivadaUmid = (umid3 - umid1) / ((tempoUmid3 - tempoUmid1) / 1000.0);

  // Valores atuais captados por cada sensor
  Serial.println("------------------------------------------");
  Serial.print("Média da Luminosidade (0-1000): ");
  Serial.println(mediaLuminosidade);
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  Serial.print("Umidade: ");
  Serial.println(umidade);
  Serial.print("Potenciômetro: ");
  Serial.println(valorPotenciometro);
  Serial.print("Ângulo Total do Servo: ");
  Serial.println(anguloServo);
  Serial.println("------------------------------------------");

  // Derivadas
  Serial.println("Derivadas:");
  Serial.print("Derivada LDR: ");
  Serial.println(derivadaLDR);
  Serial.print("Derivada Potenciômetro: ");
  Serial.println(derivadaPot);
  Serial.print("Derivada Temperatura: ");
  Serial.println(derivadaTemp);
  Serial.print("Derivada Umidade: ");
  Serial.println(derivadaUmid);
  Serial.println("------------------------------------------");

  // Matriz 6x4 dos valores e tempos (V1, V2, V3 e T1, T2, T3)
  Serial.println("Matriz (4X6) de Valores e Tempos:");
  Serial.println("V1\tV2\tV3\tT1\tT2\tT3");
  Serial.print(valorLDR1); Serial.print("\t");
  Serial.print(valorLDR2); Serial.print("\t");
  Serial.print(valorLDR3); Serial.print("\t");
  Serial.print(tempoLDR1 / 1000); Serial.print("\t");
  Serial.print(tempoLDR2 / 1000); Serial.print("\t");
  Serial.println(tempoLDR3 / 1000);

  Serial.print(valorPot1); Serial.print("\t");
  Serial.print(valorPot2); Serial.print("\t");
  Serial.print(valorPot3); Serial.print("\t");
  Serial.print(tempoPot1 / 1000); Serial.print("\t");
  Serial.print(tempoPot2 / 1000); Serial.print("\t");
  Serial.println(tempoPot3 / 1000);

  Serial.print(temp1); Serial.print("\t");
  Serial.print(temp2); Serial.print("\t");
  Serial.print(temp3); Serial.print("\t");
  Serial.print(tempoTemp1 / 1000); Serial.print("\t");
  Serial.print(tempoTemp2 / 1000); Serial.print("\t");
  Serial.println(tempoTemp3 / 1000);

  Serial.print(umid1); Serial.print("\t");
  Serial.print(umid2); Serial.print("\t");
  Serial.print(umid3); Serial.print("\t");
  Serial.print(tempoUmid1 / 1000); Serial.print("\t");
  Serial.print(tempoUmid2 / 1000); Serial.print("\t");
  Serial.println(tempoUmid3 / 1000);

  // Atraso de 2 segundos
  delay(2000);
}
