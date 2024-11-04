#include <DHT.h>  // Inclui a biblioteca DHT Sensor Library
#define DHTPIN 2  // Pino digital 2 conectado ao DHT11
#define DHTTYPE DHT11  // DHT 11

DHT dht(DHTPIN, DHTTYPE);  // Inicializando o objeto dht do tipo DHT passando como parâmetro o pino (DHTPIN) e o tipo do sensor (DHTTYPE)

float vetCorrente[300];

void setup() {
  Serial.begin(9600);  // Inicializa a comunicação serial
  dht.begin();          // Inicializa o sensor DHT11
  pinMode(7, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  // Liga o relé
  digitalWrite(7, HIGH);

  // Medições de umidade e temperatura
  float h = dht.readHumidity();      // Lê o valor da umidade
  float t = dht.readTemperature();   // Lê o valor da temperatura

  if (!isnan(h) && !isnan(t)) {      // Verifica se os valores de umidade e temperatura são válidos
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.println("%");

    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println("°C");
  } else {
    Serial.println("Erro ao ler o sensor de umidade/temperatura.");
  }

  // Coleta de corrente durante os 20 segundos de relé ligado
  double maior_Valor = 0;
  double valor_Corrente = 0;
  float tensao = 127;
  float potencia = 0;

  for (int i = 0; i < 300; i++) {
    vetCorrente[i] = analogRead(A0);
    delay(17);  // Aproximadamente 300 leituras em 20 segundos
  }

  // Calcula o maior valor de corrente medido
  for (int i = 0; i < 300; i++) {
    if (maior_Valor < vetCorrente[i]) {
      maior_Valor = vetCorrente[i];
    }
  }

  // Processa a corrente e calcula a potência
  maior_Valor = maior_Valor * 0.004882812;  // Conversão para volts
  valor_Corrente = maior_Valor - 2.5;       // Ajuste para ponto zero
  valor_Corrente = valor_Corrente * 1000;
  valor_Corrente = valor_Corrente / 66;     // Sensibilidade: 66mV/A para ACS712 30A
  valor_Corrente = valor_Corrente / 1.41421356; // Conversão para RMS
  
  // Exibe a corrente e potência calculadas
  Serial.print("Corrente = ");
  Serial.print(valor_Corrente);
  Serial.println(" A");

  potencia = valor_Corrente * tensao;
  Serial.print("Potencia = ");
  Serial.print(potencia);
  Serial.println(" W");
  delay(5000);
  // Desliga o relé e aguarda 20 segundos antes de repetir o ciclo
  digitalWrite(7, LOW);
  delay(5000);
}
