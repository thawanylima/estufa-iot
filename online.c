/********************************************************
  Modo Manual e Automatico
 ********************************************************/
#define BLYNK_TEMPLATE_ID "TMPL2gjxaIllu"
#define BLYNK_TEMPLATE_NAME "Estufa Manual"
#define BLYNK_AUTH_TOKEN "AvqF9W4zSMIjVTpRaunFZo-hhwvXXwWe"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// ================= WIFI =================

char ssid[] = "S23 FE de Vitoria";
char pass[] = "vitoria2003";

// ================= PINOS =================

#define DHTPIN D4
#define DHTTYPE DHT11

#define SOIL_PIN A0

#define RELE_BOMBA D2
#define RELE_COOLER D1

// ================= OBJETOS =================

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// ================= CONFIGURAÇÕES =================

float temperaturaMax = 28.0;
float temperaturaMin = 27.0;

int soloSeco = 20;
int soloMolhado = 40;

// ================= RELÉS =================

bool releAtivoLow = true;

// ================= ESTADOS =================

bool bombaLigada = false;
bool coolerLigado = false;

bool modoManual = false;

float umidadeRelativa = 0;

// =====================================================
// RELÉS
// =====================================================

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

// =====================================================
// BLYNK
// =====================================================

// V7 = MODO
// 0 = AUTOMÁTICO
// 1 = MANUAL

BLYNK_WRITE(V7)
{
  modoManual = param.asInt();

  Serial.print("Modo: ");

  if (modoManual)
    Serial.println("MANUAL");
  else
    Serial.println("AUTOMATICO");
}

// Sincroniza os estados salvos no servidor ao conectar/reconectar
BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V5, V6, V7);
}

// =====================================================
// BOTÃO MANUAL DA BOMBA
// V5
// =====================================================

BLYNK_WRITE(V5)
{
  if (!modoManual)
    return;

  if (param.asInt())
    ligarBomba();
  else
    desligarBomba();

  Blynk.virtualWrite(V3, bombaLigada);
}

// =====================================================
// BOTÃO MANUAL DO COOLER
// V6
// =====================================================

BLYNK_WRITE(V6)
{
  if (!modoManual)
    return;

  if (param.asInt())
    ligarCooler();
  else
    desligarCooler();

  Blynk.virtualWrite(V4, coolerLigado);
}

// =====================================================
// LEITURA DOS SENSORES
// =====================================================

void lerSensores()
{
  float umidadeAr = dht.readHumidity();
  float temperatura = dht.readTemperature();

  int leituraSolo = analogRead(SOIL_PIN);

  umidadeRelativa =
      100.0 - ((leituraSolo / 1024.0) * 100.0);

  if (isnan(umidadeAr) || isnan(temperatura))
  {
    Serial.println("Erro ao ler DHT11");
    return;
  }

  // ==========================================
  // MODO AUTOMÁTICO
  // ==========================================

  if (!modoManual)
  {
    // COOLER

    if (!coolerLigado &&
        temperatura >= temperaturaMax)
    {
      ligarCooler();
    }
    else if (coolerLigado &&
             temperatura <= temperaturaMin)
    {
      desligarCooler();
    }

    // BOMBA

    if (!bombaLigada &&
        umidadeRelativa <= soloSeco)
    {
      Serial.println("Solo seco -> bomba LIGADA");
      ligarBomba();
    }
    else if (bombaLigada &&
             umidadeRelativa >= soloMolhado)
    {
      Serial.println("Solo úmido -> bomba DESLIGADA");
      desligarBomba();
    }
  }

  // ==========================================
  // ENVIO AO BLYNK
  // ==========================================

  Blynk.virtualWrite(V0, temperatura);
  Blynk.virtualWrite(V1, umidadeAr);
  Blynk.virtualWrite(V2, umidadeRelativa);

  Blynk.virtualWrite(V3, bombaLigada);
  Blynk.virtualWrite(V4, coolerLigado);

  // Mantém dashboard sincronizado
  Blynk.virtualWrite(V5, bombaLigada ? 1 : 0);
  Blynk.virtualWrite(V6, coolerLigado ? 1 : 0);
  Blynk.virtualWrite(V7, modoManual);

  // ==========================================
  // MONITOR SERIAL
  // ==========================================

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" °C");

  Serial.print(" | Umid. Ar: ");
  Serial.print(umidadeAr);
  Serial.print("%");

  Serial.print(" | Umid. Solo: ");
  Serial.print(umidadeRelativa);
  Serial.print("%");

  Serial.print(" | Bomba: ");
  Serial.print(bombaLigada ? "ON" : "OFF");

  Serial.print(" | Cooler: ");
  Serial.print(coolerLigado ? "ON" : "OFF");

  Serial.print(" | Modo: ");
  Serial.println(modoManual ? "MANUAL" : "AUTO");
}

// =====================================================

void setup()
{
  Serial.begin(115200);

  dht.begin();

  pinMode(RELE_BOMBA, OUTPUT_OPEN_DRAIN);
  pinMode(RELE_COOLER, OUTPUT_OPEN_DRAIN);

  desligarBomba();
  desligarCooler();

  Blynk.begin(
      BLYNK_AUTH_TOKEN,
      ssid,
      pass);

  timer.setInterval(2000L, lerSensores);

  Serial.println();
  Serial.println("================================");
  Serial.println(" ESTUFA IOT INICIADA ");
  Serial.println("================================");
}

// =====================================================

void loop()
{
  Blynk.run();
  timer.run();
}
