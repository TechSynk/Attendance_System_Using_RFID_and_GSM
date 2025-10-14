#define BLYNK_TEMPLATE_ID "TMPL3zrfXOj0-"
#define BLYNK_TEMPLATE_NAME "Plant Watering"
#define BLYNK_AUTH_TOKEN "_yvbwkLQRwSQIHKST4wi0FCGw6eB2Z8h"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "abhi";
char pass[] = "abhi2004";

// Pins
#define MOISTURE_PIN 34
#define RELAY1 25
#define RELAY2 26
#define RELAY3 27

// Moisture thresholds
int threshold1 = 20;
int threshold2 = 35;
int threshold3 = 50;

// Variables
int moistureValue = 0;
int moisturePercent = 0;
bool manualMode = false;   // Auto = false, Manual = true
bool manual1 = false;
bool manual2 = false;
bool manual3 = false;

BlynkTimer timer;

// Virtual pins
#define VPIN_MODE V0      // Auto/Manual switch
#define VPIN_PUMP1 V1     // Manual button Plant 1
#define VPIN_PUMP2 V2     // Manual button Plant 2
#define VPIN_PUMP3 V3     // Manual button Plant 3
#define VPIN_MOISTURE V5  // Moisture gauge

// Auto/Manual switch handler
BLYNK_WRITE(VPIN_MODE) {
  manualMode = param.asInt();
  if (!manualMode) {
    // Reset manual buttons and turn off pumps
    manual1 = manual2 = manual3 = false;
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY3, HIGH);
    Blynk.virtualWrite(VPIN_PUMP1, 0);
    Blynk.virtualWrite(VPIN_PUMP2, 0);
    Blynk.virtualWrite(VPIN_PUMP3, 0);
  }
}

// Manual pump buttons
BLYNK_WRITE(VPIN_PUMP1) { if (manualMode) manual1 = param.asInt(); }
BLYNK_WRITE(VPIN_PUMP2) { if (manualMode) manual2 = param.asInt(); }
BLYNK_WRITE(VPIN_PUMP3) { if (manualMode) manual3 = param.asInt(); }

void checkSoil() {
  moistureValue = analogRead(MOISTURE_PIN);
  moisturePercent = map(moistureValue, 4095, 0, 0, 100);

  Blynk.virtualWrite(VPIN_MOISTURE, moisturePercent);

  // Plant 1
  if (manualMode) digitalWrite(RELAY1, manual1 ? LOW : HIGH);
  else digitalWrite(RELAY1, moisturePercent < threshold1 ? LOW : HIGH);

  // Plant 2
  if (manualMode) digitalWrite(RELAY2, manual2 ? LOW : HIGH);
  else digitalWrite(RELAY2, moisturePercent < threshold2 ? LOW : HIGH);

  // Plant 3
  if (manualMode) digitalWrite(RELAY3, manual3 ? LOW : HIGH);
  else digitalWrite(RELAY3, moisturePercent < threshold3 ? LOW : HIGH);

  // Debug
  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.print("% | Mode: ");
  Serial.print(manualMode ? "Manual" : "Auto");
  Serial.print(" | Pump1: ");
  Serial.print(digitalRead(RELAY1) ? "OFF" : "ON");
  Serial.print(" | Pump2: ");
  Serial.print(digitalRead(RELAY2) ? "OFF" : "ON");
  Serial.print(" | Pump3: ");
  Serial.println(digitalRead(RELAY3) ? "OFF" : "ON");
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, checkSoil);
}

void loop() {
  Blynk.run();
  timer.run();
}
