#include <EEPROM.h>

int botao = 2;
int ldr = A0;
int luminosidade;
int endereco = 0;
int tamanho = 3;
int estadoAtualBotao = 0;
int ultimoEstadoBotao = 0;

void setup() 
{
  pinMode(botao, INPUT);
  Serial.begin(9600);
  Serial.print("Tamanho da EEPROM = ");
  Serial.println(EEPROM.length());
}

void loop()
{
  leDaEEPROM();
  luminosidade = analogRead(ldr);
  Serial.print("Luminosidade = ");
  Serial.println(luminosidade);
  estadoAtualBotao = digitalRead(botao);
  if (estadoAtualBotao != ultimoEstadoBotao) {
    if (estadoAtualBotao == HIGH){
      salvaNaEEPROM(luminosidade);
    }
    delay(2000);
  }
  ultimoEstadoBotao = estadoAtualBotao;
}

void salvaNaEEPROM(int dado)
{
  if (endereco == tamanho){
      endereco = 0;
  }
  EEPROM.update(endereco, dado);
  delay(2000);
  Serial.print("Valor ");
  Serial.print(dado);
  Serial.print(" salvo com sucesso no endereco ");
  Serial.println(endereco);
  endereco++;
}

void leDaEEPROM()
{
  for (int i = 0; i < tamanho; i++){
    Serial.print("Valor ");
    Serial.print(EEPROM.read(i));
    Serial.print(" lido do endereco ");
    Serial.println(i);
  }
}
