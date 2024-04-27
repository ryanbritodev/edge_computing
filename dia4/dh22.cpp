#include <DHT.h>
#include <LiquidCrystal.h>

// Pinos do LCD
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

// Define o pino do sensor DHT
#define DHT_PIN 13

// Define o tipo do sensor DHT22
#define DHT_TYPE DHT22
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  lcd.begin(16, 2);
  dht.begin(); // Inicializa o sensor DHT
}

void loop() {
  float temp = dht.readTemperature(); // Lê a temperatura
  float hum = dht.readHumidity();     // Lê a umidade
  
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print("C");
  
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(hum);
  lcd.print("%");
  
  delay(1000); // Aguarda 1 segundo antes de repetir a leitura
}
