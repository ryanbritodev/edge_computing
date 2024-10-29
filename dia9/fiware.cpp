#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 21      // Pino do sensor DHT
#define DHTTYPE DHT22  // Tipo de sensor DHT **OBS: TROCAR PELO DHT11 NO DIA DO CHECKPOINT!!!**
#define LED_PIN 2      // Pino do LED
#define LDR_PIN 34     // Pino do LDR (sensor de luminosidade)

// Informações da rede Wi-Fi
const char* SSID = "";    // Nome da rede Wi-Fi
const char* PASSWORD = "";           // Senha da rede Wi-Fi

// Informações do Broker MQTT
const char* BROKER_MQTT = ""; // IP do Broker MQTT **OBS: PODE MUDAR AO INICIAR A INSTÂNCIA NOVAMENTE!!!**
const int BROKER_PORT = 1883;             // Porta do Broker MQTT
const char* TOPICO_SUBSCRIBE_LED = "/iot/led"; // Tópico para controle do LED
const char* TOPICO_PUBLISH_TEMP = "/iot/temperature"; // Tópico para enviar temperatura
const char* TOPICO_PUBLISH_HUMI = "/iot/humidity";    // Tópico para enviar umidade
const char* TOPICO_PUBLISH_LUX = "/iot/luminosity";   // Tópico para enviar luminosidade
const char* ID_MQTT = "fiware_001"; // ID MQTT

WiFiClient espClient;
PubSubClient MQTT(espClient);
DHT dht(DHTPIN, DHTTYPE);

// Função de verificação da conexão Wi-Fi
void initWiFi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Conectado!");
}

// Função de verificação da conexão ao Broker MQTT
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

// Função chamada quando uma mensagem é recebida no MyMQTT
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  
  Serial.print("Mensagem recebida no tópico ");
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(msg);

  // Controle do LED via MyMQTT
  if (msg == "on") {
    digitalWrite(LED_PIN, HIGH);
  } else if (msg == "off") {
    digitalWrite(LED_PIN, LOW);
  }
}

// Função para verificar a conexão ao Wi-Fi e MQTT
void VerificaConexoesWiFIEMQTT() {
  if (WiFi.status() != WL_CONNECTED) {
    initWiFi();
  }

  if (!MQTT.connected()) {
    while (!MQTT.connected()) {
      Serial.print("Conectando ao broker MQTT...");
      if (MQTT.connect(ID_MQTT)) {
        Serial.println(" Conectado ao broker MQTT!");
        MQTT.subscribe(TOPICO_SUBSCRIBE_LED);
      } else {
        Serial.print("Falha ao conectar. Erro: ");
        Serial.println(MQTT.state());
        delay(2000);
      }
    }
  }
}

// Publicar dados dos sensores no MyMQTT
void publishSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int ldrValue = analogRead(LDR_PIN);
  int luminosity = map(ldrValue, 0, 4095, 0, 100);  // Mapeia o valor do LDR para 0-100%

  // Publicar temperatura, umidade e luminosidade
  MQTT.publish(TOPICO_PUBLISH_TEMP, String(temperature).c_str());
  MQTT.publish(TOPICO_PUBLISH_HUMI, String(humidity).c_str());
  MQTT.publish(TOPICO_PUBLISH_LUX, String(luminosity).c_str());

  // Log no Monitor Serial
  Serial.print("Temperatura: ");
  Serial.println(temperature);
  Serial.print("Umidade: ");
  Serial.println(humidity);
  Serial.print("Luminosidade: ");
  Serial.println(luminosity);
}

void setup() {
  Serial.begin(115200); // **OBS: CONFIGURAR VELOCIDADE DE BAUDRATE PARA 115200!!!**
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  dht.begin();

  initWiFi();
  initMQTT();
}

void loop() {
  VerificaConexoesWiFIEMQTT();
  publishSensorData();
  MQTT.loop();
  delay(1000);  // Aguarda 1 segundos entre cada envio de dados
}
