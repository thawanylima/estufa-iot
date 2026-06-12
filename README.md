# 🌱 Estufa Automatizada IoT 

# Configuração da IDE Arduino

### Instalação do suporte ao ESP8266

Adicionar a URL abaixo em:

**Arquivo → Preferências → URLs adicionais para Gerenciadores de Placas**

```txt
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

Instalar a placa:

```txt
ESP8266 by ESP8266 Community
```

---

## Configuração da Placa

| Parâmetro     | Valor                        |
| ------------- | ---------------------------- |
| Board         | NodeMCU 1.0 (ESP-12E Module) |
| CPU Frequency | 80 MHz                       |
| Upload Speed  | 115200                       |
| Flash Size    | 4MB                          |
| Flash Mode    | DIO                          |

---

# Bibliotecas Necessárias

Instalar pelo Gerenciador de Bibliotecas da IDE Arduino.

## Biblioteca DHT

```cpp
#include <DHT.h>
```

Biblioteca:

```txt
DHT sensor library by Adafruit
```

Dependência:

```txt
Adafruit Unified Sensor
```

---

## Biblioteca Blynk IoT

```cpp
#include <BlynkSimpleEsp8266.h>
```

Biblioteca:

```txt
Blynk by Volodymyr Shymanskyy
```

---

## Biblioteca Wi-Fi ESP8266

```cpp
#include <ESP8266WiFi.h>
```

Instalada automaticamente com o pacote ESP8266.

---

# Mapeamento dos Pinos

## Sensor DHT11

| DHT11 | NodeMCU |
| ----- | ------- |
| VCC   | 3V3     |
| GND   | GND     |
| DATA  | D4      |


---

## Sensor de Umidade do Solo

| Sensor | NodeMCU |
| ------ | ------- |
| VCC    | 3V3     |
| GND    | GND     |
| AO     | A0      |

---

## Relé da Bomba

### Controle

| Relé | NodeMCU |
| ---- | ------- |
| VCC  | VIN     |
| GND  | GND     |
| IN   | D2      |

### Potência

| Ligação                          |
| -------------------------------- |
| COM → +5V da fonte externa       |
| NO → fio positivo da bomba       |
| Negativo da bomba → GND da fonte |

---

## Relé do Cooler

### Controle

| Relé | NodeMCU |
| ---- | ------- |
| VCC  | VIN     |
| GND  | GND     |
| IN   | D1      |

### Potência

| Ligação                           |
| --------------------------------- |
| COM → +5V da fonte externa        |
| NO → fio positivo do cooler       |
| Negativo do cooler → GND da fonte |

---

# Configuração do Blynk IoT

Inserir no código:

```cpp
#define BLYNK_TEMPLATE_ID "SEU_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Estufa IoT"
#define BLYNK_AUTH_TOKEN "SEU_TOKEN"
```

Configurar a rede Wi-Fi:

```cpp
char ssid[] = "NOME_DA_REDE";
char pass[] = "SENHA_DA_REDE";
```

---

# Datastreams Utilizados

Criar os seguintes Virtual Pins no Template Blynk.

| Função                 | Virtual Pin | Tipo    | Faixa   |
| ---------------------- | ----------- | ------- | ------- |
| Temperatura            | V0          | Double  | 0 a 50  |
| Umidade do Ar          | V1          | Double  | 0 a 100 |
| Umidade do Solo        | V2          | Double  | 0 a 100 |
| Estado da Bomba        | V3          | Integer | 0 ou 1  |
| Estado do Cooler       | V4          | Integer | 0 ou 1  |
| Botão Manual da Bomba  | V5          | Integer | 0 ou 1  |
| Botão Manual do Cooler | V6          | Integer | 0 ou 1  |
| Modo Automático/Manual | V7          | Integer | 0 ou 1  |

---

# Widgets Recomendados

| Widget | Datastream |
| ------ | ---------- |
| Gauge  | V0         |
| Gauge  | V1         |
| Gauge  | V2         |
| LED    | V3         |
| LED    | V4         |
| Switch | V5         |
| Switch | V6         |
| Switch | V7         |

---

# Parâmetros Configuráveis

## Controle de Temperatura

```cpp
float temperaturaMax = 28.0;
float temperaturaMin = 27.0;
```

* temperaturaMax → Liga o cooler
* temperaturaMin → Desliga o cooler

---

## Controle de Umidade do Solo

```cpp
int soloSeco = 20;
int soloMolhado = 40;
```

* soloSeco → Liga a bomba
* soloMolhado → Desliga a bomba

---

# Frequência de Atualização

Os sensores são lidos a cada:

```cpp
timer.setInterval(2000L, lerSensores);
```

Equivalente a:

```txt
2 segundos
```

---



