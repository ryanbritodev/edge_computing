// https://wokwi.com/projects/408605955656804353

// ------------------------- FIAP -------------------------
// CHECKPOINT 4
// EDGE COMPUTING & COMPUTER SYSTEMS
// Participantes:
// Prof. Paulo Marcotti PF2150
// Felipe Sousa de Oliveira RM559085
// --------------------------------------------------------

// ---------------------- EXPLICAÇÃO ----------------------
// O circuito ao lado controla a frequência e a ativação de um buzzer, 
// utilizando um botão e um potenciômetro. O tom do buzzer é ajustado 
// com base na leitura do potenciômetro, sendo somente ativado com o clique 
// do botão. A cada ciclo, o valor do potenciômetro é lido e mapeado. Quando 
// o botão é pressionado, o buzzer toca a frequência correspondente. Caso 
// contrário, o buzzer permanece desligado. O sistema também imprime no 
// monitor serial o valor do potenciômetro, a frequência calculada, e o 
// estado do botão ("pressionado" ou "não pressionado"). Assim, a interação 
// entre potenciômetro e buzzer é monitorada em tempo real.
// --------------------------------------------------------

const int buzzerPin = 32;  
const int potPin = 13;  
int buttonState = 0;       
boolean soundEnabled = true; 
const int buttonPin = 4;   

void setup() {
  pinMode(buttonPin, INPUT);  
  pinMode(buzzerPin, OUTPUT); 
  Serial.begin(115200); // Inicializa o monitor serial com baud rate do ESP32 de 115200 como orientado em sala de aula
}

void loop() {
  buttonState = digitalRead(buttonPin);  // Leitura do estado do botão

  // Verifica se o botão foi pressionado
  if (buttonState == HIGH) {
    soundEnabled = !soundEnabled;  // ON | OFF
    delay(250);
  }

  // Se o som estiver ativado
  if (soundEnabled) {
    int sensorValue = analogRead(potPin);  // Leitura do valor do potenciômetro
    int frequency = map(sensorValue, 0, 1023, 100, 2000);  // Mapeamento do valor da frequência

    // Imprime a frequência mapeada no monitor serial
    Serial.print("Frequência do buzzer: ");
    Serial.println(frequency);

    tone(buzzerPin, frequency);  // Toca a frequência mapeada no buzzer
    delay(10);
    
    // Média da frequência e Estado do botão (cálculo simples realizado apenas para ilustração, como orientado no CP)
    float media = (float)(frequency + buttonState) / 2;  
    Serial.print("Média (frequência + estado do botão): ");
    Serial.println(media);
  } else {
    noTone(buzzerPin);  // Para o som quando o buzzer estiver desligado

    // Exibe no monitor serial que o som está desligado
    Serial.println("Som desligado");
  }

  // Imprime se o botão está ativado ou não no monitor serial
  if (buttonState == HIGH) {
    Serial.println("Botão pressionado");
  } else {
    Serial.println("Botão não pressionado");
  }
  delay(300); // Pequeno delay de 300 milisegundos
}
