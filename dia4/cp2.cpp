#include <LiquidCrystal.h> // Biblioteca para controlar o LCD

// Inicializa o LCD com os pinos de controle
LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

// Definindo as entradas para os sensores
int ldr = A2;          // Fotoresistor para medir a luz ambiente
int pot_umidade = A0;  // Potenciômetro para medir umidade
int tmp = A1;          // Sensor TMP para medir temperatura

// Número de vezes que lemos os sensores para calcular uma média
int num_readings = 5;

// Variáveis para armazenar a média das leituras
float light_avg = 0;
float temp_avg = 0;
float humidity_avg = 0;

void setup() {
    lcd.begin(16, 2); // Configura o LCD com 16 colunas e 2 linhas
    pinMode(ldr, INPUT); // Configura o fotoresistor como entrada
    pinMode(pot_umidade, INPUT); // Configura o sensor de umidade como entrada
    pinMode(tmp, INPUT); // Configura o sensor de temperatura como entrada
    Serial.begin(9600); // Inicia a comunicação serial para debugar
}

void loop() {
    // Zera as variáveis para as médias
    light_avg = 0;
    temp_avg = 0;
    humidity_avg = 0;

    // Soma as leituras para calcular a média
    for (int i = 0; i < num_readings; i++) {
        light_avg += analogRead(ldr); // Lê o valor do fotoresistor
        temp_avg += analogRead(tmp); // Lê o valor do sensor de temperatura
        humidity_avg += analogRead(pot_umidade); // Lê o valor do sensor de umidade
        delay(100); // Aguarda um pouco entre as leituras para evitar ruído
    }

    // Calcula a média final dividindo pelo número de leituras
    light_avg /= num_readings;
    temp_avg /= num_readings;
    humidity_avg /= num_readings;
    
    // Converte a leitura de temperatura para graus Celsius
    float voltage = temp_avg * (5.0 / 1023.0); // Converte para tensão (assumindo 5V como referência)
    float temperature = (voltage - 0.5) * 100; // Converte a tensão para temperatura em graus Celsius

    // Converte as leituras de luz e umidade para uma escala mais compreensível
    float light_level = map(light_avg, 0, 1023, 0, 100); // Converte para uma escala de 0 a 100%
    float humidity = map(humidity_avg, 0, 1023, 0, 100); // Converte para uma escala de 0 a 100%

    // Mostra informações sobre a temperatura no LCD
    lcd.clear(); // Limpa o display antes de mostrar uma nova mensagem
    lcd.setCursor(0, 0); // Define o início da mensagem na primeira linha
    if (temperature >= 10 && temperature <= 15) { // Verifica se a temperatura está em uma faixa confortável
        lcd.print("Temp. OK"); // Se a temperatura estiver entre 10 e 15 graus, é considerada adequada
    } else if (temperature > 15) { // Se for maior que 15 graus
        lcd.print("Temp. ALTA"); // Indica que a temperatura está alta
    } else { // Se for menor que 10 graus
        lcd.print("Temp. BAIXA"); // Indica que está baixa
    }

    lcd.setCursor(0, 1); // Move para a segunda linha
    lcd.print("Temp. = "); // Escreve a temperatura atual
    lcd.print(temperature); // Mostra a temperatura em graus Celsius
    lcd.print("C"); // Adiciona o símbolo de Celsius

    delay(3000); // Aguarda 3 segundos antes de mostrar a próxima informação

    // Mostra informações sobre a umidade no LCD
    lcd.clear(); // Limpa o display
    lcd.setCursor(0, 0); // Define o início da mensagem
    if (humidity >= 50 && humidity <= 70) { // Se a umidade estiver em uma faixa confortável
        lcd.print("Umid. OK"); // Indica que a umidade está boa
    } else if (humidity > 70) { // Se a umidade for maior que 70%
        lcd.print("Umid. ALTA"); // Indica que está alta
    } else { // Se for menor que 50%
        lcd.print("Umid. BAIXA"); // Indica que está baixa
    }

    lcd.setCursor(0, 1); // Move para a segunda linha
    lcd.print("Umid. = "); // Mostra a porcentagem de umidade
    lcd.print(humidity); 
    lcd.print("%"); // Adiciona o símbolo de porcentagem

    delay(3000); // Aguarda 3 segundos antes de mudar para a próxima informação

    // Mostra informações sobre a luz ambiente no LCD
    lcd.clear(); // Limpa o display
    lcd.setCursor(0, 0); // Define o início da mensagem
    if (light_level < 30) { // Se a luz ambiente for menor que 30%
        lcd.print("Luz Ambiente OK"); // Luz em nível adequado
    } else if (light_level < 70) { // Se for entre 30% e 70%
        lcd.print("Meia Luz"); // Um nível moderado de luz
    } else { // Se for maior que 70%
        lcd.print("Muito Claro"); // Indica que está muito claro
    }

    lcd.setCursor(0, 1); // Move para a segunda linha
    lcd.print("Luz = "); // Mostra a porcentagem de luz ambiente
    lcd.print(light_level); 
    lcd.print("%"); // Adiciona o símbolo de porcentagem
    
    delay(3000); // Aguarda 3 segundos antes de repetir o ciclo
}
