#include <DHT.h>
#define DHTPIN 2  // Pino digital 2 conectado ao DHT11
#define DHTTYPE DHT11  // Tipo do sensor DHT

DHT dht(DHTPIN, DHTTYPE);

double maior_Valor = 0, valor_Corrente = 0, valor_Atual = 0, energia_gasta = 0;
float tensao = 127, potencia = 0;
unsigned long tempo_inicio = 0, tempo_total = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(7, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  float umidade = dht.readHumidity();
  
  if (umidade <= 40) {
    digitalWrite(7, HIGH);  // Liga o umidificador

    if (tempo_inicio == 0) {
        tempo_inicio = millis();  
    }

    valor_Atual = analogRead(A0);
    delay(17);

    maior_Valor = valor_Atual * 0.004882812;
    valor_Corrente = (maior_Valor - 2.5) * 1000 / 66 / 1.41421356;  // Sensibilidade: 66mV/A para ACS712 30A | Conversão para RMS
    potencia = valor_Corrente * tensao;

    // Calcular o tempo de operação em horas
    float tempo_horas = (millis() - tempo_inicio) / 3600000.0;
    energia_gasta += potencia * tempo_horas;

    tempo_total += tempo_horas; 
    tempo_inicio = millis(); 

    // Enquanto estiver operando, imprime a umidade na tela
    Serial.print("Umidade do ar no momento (%): ");
    Serial.println(umidade);

  } 
  else if (umidade >= 60) {
    digitalWrite(7, LOW);  // Desliga o umidificador
    tempo_inicio = 0;  
  
    Serial.println("----- Relatório do Umidificador -----");
    Serial.print("Umidade do ar no momento (%): ");
    Serial.println(umidade);
    Serial.print("Energia gasta (kWh): ");
    Serial.println(energia_gasta);
    Serial.print("Tempo de operação (horas): ");
    Serial.println(tempo_total);  
    Serial.println("-------------------------------------");
  } 
  else {
    digitalWrite(7, LOW);  
    Serial.print("Umidade do ar no momento (%): "); //trocar para printar no lcd o tanto que ta no momento e quando te potencia que ta gastando
    Serial.println(umidade);
  }
}
