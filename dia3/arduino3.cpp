int ledVermelhoCarro = 12;
int ledAmareloCarro = 11;
int ledVerdeCarro = 10;
int ledVermelhoPedestre = 9;
int ledVerdePedestre = 8;
int botao = 2;
int tempoTravessia = 5000;
long tempoFinalizado = 0;
int tempoCarros = 8000;

void setup()
{
  	Serial.begin(9600);
    pinMode(ledVermelhoCarro, OUTPUT);
    pinMode(ledAmareloCarro, OUTPUT);
    pinMode(ledVerdeCarro, OUTPUT);
    pinMode(ledVermelhoPedestre, OUTPUT);
    pinMode(ledVerdePedestre, OUTPUT);
    pinMode(botao, INPUT);
  
    digitalWrite(ledVerdeCarro, HIGH);
  	digitalWrite(ledAmareloCarro, LOW);
  	digitalWrite(ledVermelhoCarro, LOW);
  	digitalWrite(ledVermelhoPedestre, HIGH);
    digitalWrite(ledVerdePedestre, LOW);
}

void loop()
{
  	passarCarro();
  
  	Serial.print("Tempo do millis(): ");
    Serial.println(millis());
  	Serial.print("Tempo desde o ultimo ciclo de passagem de pedestres: ");
    Serial.println(millis() - tempoFinalizado);
  
  	if(digitalRead(botao) && (millis() - tempoFinalizado) >= tempoCarros)
    {
  		passarPedestre();
        tempoFinalizado = millis();
      
      	Serial.print("Momento em que terminou o ciclo de passagem de pedestres: ");
      	Serial.println(tempoFinalizado);
    }
}

void passarCarro()
{
	digitalWrite(ledVermelhoPedestre, HIGH);
    digitalWrite(ledVerdePedestre, LOW);
  	delay(1000);
    digitalWrite(ledVerdeCarro, HIGH);
  	digitalWrite(ledAmareloCarro, LOW);
  	digitalWrite(ledVermelhoCarro, LOW);
  	digitalWrite(ledVermelhoPedestre, HIGH);
    digitalWrite(ledVerdePedestre, LOW);
}


  
void passarPedestre()
{
	delay(1000);
  	digitalWrite(ledVerdeCarro, LOW);
  	digitalWrite(ledAmareloCarro, HIGH);
  	digitalWrite(ledVermelhoCarro, LOW);
  	delay(1000);
  	digitalWrite(ledVerdeCarro, LOW);
  	digitalWrite(ledAmareloCarro, LOW);
  	digitalWrite(ledVermelhoCarro, HIGH);
  	delay(1000);
  	digitalWrite(ledVermelhoPedestre, LOW);
    digitalWrite(ledVerdePedestre, HIGH);
 	delay(tempoTravessia);
}
