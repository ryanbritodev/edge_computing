//https://wokwi.com/projects/408477414953060353

/* 
------------------ FIAP --------------------
CHECKPOINT 4
EDGE COMPUTING & COMPUTER SYSTEMS
Participantes:
Prof. Paulo Marcotti PF2150
Ryan Brito Pereira Ramos RM554497
--------------------------------------------
*/

/*
---------------- EXPLICAÇÃO ----------------

O circuito controla a iluminação usando um 
sensor de movimento e um sensor de luminosidade 
LDR. Ele aciona um LED de forma automática, 
com base na detecção de movimento e na quantidade
de luz ambiente. O sensor LDR mede a luminosidade,
e os valores são remapeados de 0 a 1000 lux.
A cada ciclo (1 segundo), o sistema calcula a média das 
três últimas leituras da luz. O sensor de movimento 
detecta movimento no ambiente. Se se houver movimento 
e a luminosidade for baixa (menos de 300 lux),
o LED acende (modo noturno). Se houver movimento
e a luminosidade for alta, o LED permanece apagado 
(modo diurno). Sem movimento, o LED sempre fica apagado.

--------------------------------------------
*/

int pinoMovimento = 13;  // Pino do Sensor de Movimento (PIR)
int pinoLDR = 34;  // Pino do LDR
int pinoLED = 2;   // Pino do LED

// Variáveis para armazenar as últimas leituras de luminosidade
int valorLDR1 = 0;
int valorLDR2 = 0;
int valorLDR3 = 0;

void setup() {
  Serial.begin(115200);  // Inicializa o monitor serial com baud rate do ESP32 de 115200 como orientado em sala de aula
  pinMode(pinoMovimento, INPUT);
  pinMode(pinoLED, OUTPUT);
}

void loop() {
  // Leitura do LDR (valores de 0 a 4095)
  int valorLDR = analogRead(pinoLDR); // Leitura da luminosidade

  // Remapeando a leitura para um valor de luminosidade mais compreensível (0 a 1000 lux), tendo em vista que o máximo capturado pelo ESP32 em pinos analógicos é 4095
  int luminosidadeNormalizada = map(valorLDR, 0, 4095, 1000, 0);

  int estadoMovimento = digitalRead(pinoMovimento); // Leitura do Sensor de Movimento (PIR)
  
  // Realizando o reajuste das últimas 3 leituras do LDR
  valorLDR3 = valorLDR2;
  valorLDR2 = valorLDR1;
  valorLDR1 = luminosidadeNormalizada;

  // Cálculo da média das últimas 3 leituras do LDR + valor de leitura do Sensor de Movimento (cálculo realizado apenas para ilustração)
  int mediaLuminosidade = (valorLDR1 + valorLDR2 + valorLDR3 + estadoMovimento) / 3;

  // Exibe a média no Monitor Serial
  Serial.print("Média da Luminosidade (lux): ");
  Serial.println(mediaLuminosidade);

  // Definição de um Ponto Crítico para diferenciar "dia" ou "noite"
  int pontoCritico = 300;

  // Lógica para o modo dia/noite com base na leitura do Sensor de Movimento e do LDR
  if (estadoMovimento == HIGH) {  // Detectou movimento
    if (mediaLuminosidade < pontoCritico) {  // Baixa luminosidade (modo noturno)
      Serial.println("Movimento detectado: Modo Noturno - LED ACESO");
      digitalWrite(pinoLED, HIGH);  // Acende o LED
    } else {  // Alta luminosidade (modo diurno)
      Serial.println("Movimento detectado: Modo Dia - LED APAGADO");
      digitalWrite(pinoLED, LOW);  // Mantém o LED apagado
    }
  } else {  // Não há movimento
    Serial.println("Sem movimento - LED APAGADO");
    digitalWrite(pinoLED, LOW);  // Mantém o LED apagado
  }

  delay(1000);  // Pequeno delay de 1 segundo
}
