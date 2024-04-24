#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);
int ldr = A2;          // Pin para o fotoresistor
int pot_umidade = A0;  // Pin para o potenciômetro de umidade
int pot_temp = A1;     // Pin para o potenciômetro de temperatura

// Número de leituras para calcular a média
int num_readings = 5;

// Variáveis para armazenar as leituras
float light_avg = 0;
float temp_avg = 0;
float humidity_avg = 0;

void setup() {
    lcd.begin(16, 2);
    pinMode(ldr, INPUT);
    pinMode(pot_umidade, INPUT);
    pinMode(pot_temp, INPUT);
    Serial.begin(9600);
}

void loop() {
    // Resetando as médias antes de ler novamente
    light_avg = 0;
    temp_avg = 0;
    humidity_avg = 0;

    // Calculando a média das leituras
    for (int i = 0; i < num_readings; i++) {
        light_avg += analogRead(ldr);
        temp_avg += analogRead(pot_temp);
        humidity_avg += analogRead(pot_umidade);
        delay(100); // Pequena pausa entre leituras
    }

    // Calculando a média final
    light_avg /= num_readings;
    temp_avg /= num_readings;
    humidity_avg /= num_readings;

    // Convertendo leituras para valores compreensíveis
    float light_level = map(light_avg, 0, 1023, 0, 100);  // Converte para porcentagem de 0 a 100
    float temperature = map(temp_avg, 0, 1023, 0, 100);   // Converte para 0 a 100 (aproximado)
    float humidity = map(humidity_avg, 0, 1023, 0, 100);  // Converte para porcentagem de 0 a 100

    // Exibir temperatura
    lcd.clear(); // Limpa o LCD
    lcd.setCursor(0, 0); 
    if (temperature >= 10 && temperature <= 15) {
        lcd.print("Temp. OK");
    } else if (temperature > 15) {
        lcd.print("Temp. ALTA");
    } else {
        lcd.print("Temp. BAIXA");
    }
    lcd.setCursor(0, 1);
    lcd.print("Temp. = ");
    lcd.print(temperature);
    lcd.print("C");
    delay(3000); // Esperar 3 segundos antes de mudar para a próxima mensagem

    // Exibir umidade
    lcd.clear();
    lcd.setCursor(0, 0);
    if (humidity >= 50 && humidity <= 70) {
        lcd.print("Umid. OK");
    } else if (humidity > 70) {
        lcd.print("Umid. ALTA");
    } else {
        lcd.print("Umid. BAIXA");
    }
    lcd.setCursor(0, 1);
    lcd.print("Umid. = ");
    lcd.print(humidity);
    lcd.print("%");
    delay(3000);

    // Exibir luminosidade
    lcd.clear();
    lcd.setCursor(0, 0);
    if (light_level < 30) {
        lcd.print("Luz Ambiente OK");
    } else if (light_level < 70) {
        lcd.print("Meia Luz");
    } else {
        lcd.print("Muito Claro");
    }
    lcd.setCursor(0, 1);
    lcd.print("Luz = ");
    lcd.print(light_level);
    lcd.print("%");
    delay(3000);
}
