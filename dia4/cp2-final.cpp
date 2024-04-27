// ---- FIAP ----
// CHECKPOINT 2 - EDGE COMPUTING E COMPUTER SYSTEMS
// Prof. Flávio Grassi
// Participantes:
// Arthur Cotrick Pagani RM 554510
// Diogo Leles Franciulli RM 558487
// Felipe Sousa de Oliveira RM 559085
// Ryan Brito Pereira Ramos RM 554497

#include <LiquidCrystal.h>
#include <DHT.h>

// Inicializa o LCD com os pinos de controle
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

// Definindo as entradas para os sensores
int ldr = A2; // Fotoresistor para medir a luz ambiente
#define DHT_PIN 13 // Pino para o DHT22
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE); // Criação do objeto para o sensor DHT22

// Variáveis para armazenar as leituras
int num_readings = 5;
float light_avg = 0;

void setup() {
  lcd.begin(16, 2); // Configura o LCD com 16 colunas e 2 linhas
  pinMode(ldr, INPUT); // Configura o fotoresistor como entrada
  Serial.begin(9600); // Inicia o monitor serial para depuração
  dht.begin(); // Inicializa o sensor DHT22
}

void loop() {
  light_avg = 0;

  // Soma as leituras para calcular a média do sensor de luz
  for (int i = 0; i < num_readings; i++) {
    light_avg += analogRead(ldr); // Lê o valor do fotoresistor
    delay(100); // Aguarda um pouco entre as leituras
  }

  light_avg /= num_readings;

  // Ajusta a escala para refletir a proporção inversa entre leitura do fotoresistor e luz
  float light_level = map(light_avg, 1023, 0, 0, 100); // Invertendo a escala

  // Verifica as leituras do sensor DHT22
  float temp = dht.readTemperature(); // Lê a temperatura
  float hum = dht.readHumidity();     // Lê a umidade

  // Verifica se as leituras são NaN e corrige conforme necessário
  if (isnan(temp) || isnan(hum)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Erro no DHT22");
    delay(5000); // Pausa para visualizar a mensagem
    return; // Sai do loop para evitar leituras erradas
  }

  // Mostrar a temperatura no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  if (temp >= 10 && temp <= 15) {
    lcd.print("Temp. OK");
  } else if (temp > 15) {
    lcd.print("Temp. ALTA");
  } else {
    lcd.print("Temp. BAIXA");
  }

  lcd.setCursor(0, 1);
  lcd.print("Temp. = ");
  lcd.print(temp);
  lcd.print("C");

  delay(5000); // Aguarda 5 segundos

  // Mostrar a umidade no LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  if (hum >= 50 && 70) {
    lcd.print("Umidade. OK");
  } else if (hum > 70) {
    lcd.print("Umidade. ALTA");
  } else {
    lcd.print("Umidade. BAIXA");
  }

  lcd.setCursor(0, 1);
  lcd.print("Umid. = ");
  lcd.print(hum);
  lcd.print("%");

  delay(5000); // Aguarda 5 segundos

  // Mostrar o nível de luz ambiente no LCD
  lcd.clear(); 
  lcd.setCursor(0, 0);
  
  if (light_level < 30) {
    lcd.print("Luz Ambiente OK");
  } else if (light_level < 70) {
    lcd.print("Meia Luz");
  } else {
    lcd.print("Muito CLARO");
  }

  lcd.setCursor(0, 1);
  lcd.print("Luz = ");
  lcd.print(light_level);
  lcd.print("%");

  delay(5000); // Aguarda 5 segundos
}
