// https://wokwi.com/projects/408637504409466881

/*
------------------ FIAP --------------------
CHECKPOINT 4
EDGE COMPUTING & COMPUTER SYSTEMS
Participantes:
Prof. Paulo Marcotti PF2150
Diogo Leles Franciulli RM558487
--------------------------------------------

---------------- EXPLICAÇÃO ----------------
O circuito ao lado controla a leitura de temperatura 
usando um sensor NTC e mede a distância com 
um sensor ultrassônico. Ele exibe as informações
em um display LCD e calcula a relação 
entre temperatura e distância. O valor 
lido pelo pino analógico é convertido para 
graus Celsius utilizando a equação do coeficiente
Beta do termistor (projeto usado como base https://wokwi.com/projects/357160539584107521),
garantindo precisão na leitura.
Se o valor lido for inválido, o sistema retorna 
o zero absoluto em graus celsius (-273,15 °C) como erro. O sensor
ultrassônico mede a distância até um objeto 
enviando pulsos e medindo o tempo de retorno 
do eco. O valor é convertido para centímetros.
As leituras de temperatura e distância são exibidas
no display LCD de forma rotativa a cada ciclo (2 segundos).
Caso a temperatura e a distância sejam válidas,
o sistema também exibe a relação entre os dois
valores. Caso haja erro, o sistema indica uma
leitura inválida. Os dados captados são enviados
para o monitor serial para acompanhamento.
--------------------------------------------
*/

#include <LiquidCrystal_I2C.h> // Biblioteca pro LCD I2C (melhor por conta das suas poucas conexões)

LiquidCrystal_I2C lcd(0x27, 16, 2); // Endereço do LCD I2C, com suas dimensões (16 colunas, 2 linhas)

const int sensorNTC = 34; // Pino para o Sensor NTC (Temperatura)
const int trigPin = 12; // Pino "Trigger" do Sensor Ultrassônico
const int echoPin = 13; // Pino "Echo" do Sensor Ultrassônico
const float BETA = 3950; // Coeficiente Beta do termistor NTC (Referência: https://wokwi.com/projects/357160539584107521)
const int resistorFixed = 10000; // Resistor ilusório fixo de 10kΩ (10 mil ohms)

void setup() {
  lcd.init(); // Inicializa o LCD
  lcd.backlight(); // Liga a luz de fundo do LCD
  
  // Exibe mensagem de boas-vindas ao projeto
  lcd.setCursor(0, 0);
  lcd.print("CHECKPOINT 4");
  lcd.setCursor(0, 1);
  lcd.print("DIOGO LELES");
  delay(3000); // Delay de 3 segundos pra exibir as mensagens

  pinMode(trigPin, OUTPUT); // Definindo o pino "Trigger" como saída
  pinMode(echoPin, INPUT); // Definindo o pino "Echo" como entrada

  Serial.begin(115200); // Inicializa o monitor serial com baud rate do ESP32 de 115200 como orientado em sala de aula
  Serial.println("---------------------"); // Primeira linha para divisão das leituras
  analogReadResolution(10); // Configura a resolução para leitura analógica
}

void loop() {
  float temperatura = lerTemperatura(); // Lê a temperatura do Sensor NTC
  float distancia = lerDistanciaUltrassonico(); // Lê a distância do Sensor ultrassônico
  
  // Exibe os valores no LCD de forma rotativa
  exibirNoLCD("Temperatura:", temperatura, "C");
  delay(2000); // Delay de 2 segundos
  exibirNoLCD("Distancia:", distancia, "cm");
  delay(2000); // Delay de 2 segundos
  exibirRelacaoNoLCD(temperatura, distancia);
  delay(2000); // Delay de 2 segundos

  // Temperatura, Distância e Relação
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" ℃");
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  Serial.print("Relacao: ");
  Serial.println(temperatura / distancia);
  Serial.println("---------------------");
}

float lerTemperatura() {
  int valorADC = analogRead(sensorNTC); // Lê o valor do Sensor NTC

  // Evita possíveis divisões por zero e valores errados
  if (valorADC == 0 || valorADC == 1023) { // Protege de valores extremos
    return -273.15; // Retorna o valor mais baixo possível (zero absoluto em graus celsius), indicando falha de leitura
  }

  // Cálculo da temperatura em Celsius usando a equação fornecida no projeto https://wokwi.com/projects/357160539584107521
  float tempCelsius = 1 / (log(1 / (1023.0 / valorADC - 1)) / BETA + 1.0 / 298.15) - 273.15;
  
  return tempCelsius;
}

float lerDistanciaUltrassonico() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Envia pulso de 10 milisegundos no pino "Trigger"
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  float duracao = pulseIn(echoPin, HIGH);  // Lê a duração do pulso no pino "Echo"
  float distancia = duracao * 0.034 / 2;   // Converte a distância em centímetros
  return distancia;
}

void exibirNoLCD(String linha1, float valor, String unidade) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linha1);
  lcd.setCursor(0, 1);
  lcd.print(valor);
  lcd.print(" ");
  lcd.print(unidade);
}

void exibirRelacaoNoLCD(float temperatura, float distancia) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Relacao:");
  lcd.setCursor(0, 1);
  if (distancia == 0 || temperatura == -273.15) { // Segunda confirmção, para evitar divisão por zero
    lcd.print("Invalida");
  } else {
    float relacao = temperatura / distancia;  // Cálculo matemático simples
    lcd.print(relacao);
  }
}
