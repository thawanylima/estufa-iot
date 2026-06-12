#include <DHT.h>

// ===== PINOS =====
#define DHTPIN D4
#define DHTTYPE DHT11
#define SOIL_PIN A0
#define RELE_BOMBA D2
#define RELE_COOLER D1

float umidadeRelativa;

DHT dht(DHTPIN, DHTTYPE);

// ===== CONFIGURAÇÕES =====
float temperaturaMax = 28.0;   // Liga cooler acima disso
float temperaturaMin = 27.0;   // Desliga cooler abaixo disso

int soloSeco = 20;
int soloMolhado = 40;

// ===== IMPORTANTE =====
// Se o relé ligar com LOW, deixe true
bool releAtivoLow = true;

// ===== CONTROLE =====
bool coolerLigado = false;
bool bombaLigada = false;

// ===== FUNÇÕES =====
void ligarBomba() {
  digitalWrite(RELE_BOMBA, releAtivoLow ? LOW : HIGH);
  bombaLigada = true;
}

void desligarBomba() {
  digitalWrite(RELE_BOMBA, releAtivoLow ? HIGH : LOW);
  bombaLigada = false;
}

void ligarCooler() {
  digitalWrite(RELE_COOLER, releAtivoLow ? LOW : HIGH);
  coolerLigado = true;
}

void desligarCooler() {
  digitalWrite(RELE_COOLER, releAtivoLow ? HIGH : LOW);
  coolerLigado = false;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  dht.begin();

  pinMode(RELE_BOMBA, OUTPUT_OPEN_DRAIN);
  pinMode(RELE_COOLER, OUTPUT_OPEN_DRAIN);

  desligarBomba();
  desligarCooler();

  Serial.println();
  Serial.println("=== Sistema de Monitoramento de Plantas ===");
}

void loop() {
  float umidadeAr = dht.readHumidity();
  float temperatura = dht.readTemperature();
  int leituraSolo = analogRead(SOIL_PIN);
  umidadeRelativa = (100.00 - ((analogRead(SOIL_PIN)/1024.00) * 100.00));

  if (isnan(umidadeAr) || isnan(temperatura)) {
  Serial.println("Erro ao ler o DHT11!");
  delay(2000);
  return;
  }

  // ===== CONTROLE DO COOLER =====
  if (!coolerLigado && temperatura >= temperaturaMax) {
  ligarCooler();
  } else if (coolerLigado && temperatura <= temperaturaMin) {
  desligarCooler();
  }

  // ===== CONTROLE DA BOMBA =====
  // ===== CONTROLE DA BOMBA COM HISTERESE =====

  if (!bombaLigada && umidadeRelativa <= soloSeco) {
  Serial.println("Solo seco -> bomba LIGADA");
  ligarBomba();
  }
  else if (bombaLigada && umidadeRelativa >= soloMolhado) {
  Serial.println("Solo umido -> bomba DESLIGADA");
  desligarBomba();
  }

  // ===== MONITOR SERIAL =====

  Serial.print("Umid. Ar: "); Serial.print(umidadeAr, 1);
  Serial.print("% | Temp: "); Serial.print(temperatura, 1); Serial.print(" C");
  Serial.print(" | Solo: "); Serial.print(umidadeRelativa);
  Serial.print("% | Bomba: "); Serial.print(bombaLigada ? "ON" : "OFF");
  Serial.print(" | Cooler: "); Serial.println(coolerLigado ? "ON" : "OFF");
  Serial.println("------------------------------------------------");

  delay(2000);
}
