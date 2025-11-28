#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// === Sensor and Relay Pins ===
const int voltageSensor1 = A0; // V1 -> MAINS (Transformer)
const int voltageSensor2 = A1; // V2 -> BATTERY
const int currentSensor  = A2; // (Not used for random mode, but kept for compatibility)

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Power Source Relays
const int relayMains = 12;   // Relay for mains power
const int relayBattery = 4;  // Relay for battery backup

// Device Relays
const int fanRelay   = 5;
const int ledRelay   = 6;
const int usbRelay   = 7;
const int extraRelay = 8;

// LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// === Configurable parameters ===
const float voltageMultiplier = 5.0;  // Adjust to your resistor divider
const float MAX_ALLOWED_VOLTAGE = 12.0;
const float MAINS_MIN_VOLTAGE = 5.5;   // treat <=0.5V as power-off threshold

// Timing intervals
const unsigned long SAMPLE_INTERVAL_MS = 500;
const unsigned long LCD_INTERVAL_MS    = 700;
const unsigned long DHT_INTERVAL_MS    = 2000;

// Variables
float mainsVoltage = 0.0;
float batteryVoltage = 0.0;
float liveCurrent = 0.0;
float temp = NAN;
float hum  = NAN;
String powerSource = "NoPwr";

unsigned long lastSample = 0;
unsigned long lastLCD = 0;
unsigned long lastDHT = 0;

// === Setup ===
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0)); // randomize seed

  dht.begin();
  lcd.init();
  lcd.backlight();

  pinMode(relayMains, OUTPUT);
  pinMode(relayBattery, OUTPUT);
  pinMode(fanRelay, OUTPUT);
  pinMode(ledRelay, OUTPUT);
  pinMode(usbRelay, OUTPUT);
  pinMode(extraRelay, OUTPUT);

  // Start all relays OFF
  digitalWrite(relayMains, LOW);
  digitalWrite(relayBattery, LOW);
  digitalWrite(fanRelay, LOW);
  digitalWrite(ledRelay, LOW);
  digitalWrite(usbRelay, LOW);
  digitalWrite(extraRelay, LOW);
}

// === Read voltage from divider ===
float readVoltage(int pin) {
  int raw = analogRead(pin);
  float v = raw * (5.0 / 1023.0) * voltageMultiplier;
  if (v > MAX_ALLOWED_VOLTAGE) v = MAX_ALLOWED_VOLTAGE;
  return v;
}

// === Switching logic ===
void updatePowerSource(float vMains, float vBatt) {
  // If mains voltage is near zero, switch to battery (if available)
  if (vMains <= MAINS_MIN_VOLTAGE && vBatt > 0.5) {
    digitalWrite(relayMains, LOW);
    digitalWrite(relayBattery, HIGH);
    powerSource = "Batt";
  }
  // If mains is back, switch to mains
  else if (vMains > MAINS_MIN_VOLTAGE) {
    digitalWrite(relayMains, HIGH);
    digitalWrite(relayBattery, LOW);
    powerSource = "Mains";
  }
  // If both are off
  else {
    digitalWrite(relayMains, LOW);
    digitalWrite(relayBattery, LOW);
    powerSource = "NoPwr";
  }
}

// === Generate Random Current (1A–2A) or 0A if No Power ===
float getDisplayCurrent(String src) {
  if (src == "NoPwr") return 0.0;
  // random current between 1.0A to 2.0A
  float c = random(100, 200) / 100.0;
  return c;
}

// === Main Loop ===
void loop() {
  unsigned long now = millis();

  // --- Sampling Block ---
  if (now - lastSample >= SAMPLE_INTERVAL_MS) {
    lastSample = now;
    mainsVoltage = readVoltage(voltageSensor1);
    batteryVoltage = readVoltage(voltageSensor2);

    // update which source is active
    updatePowerSource(mainsVoltage, batteryVoltage);

    // Simulate random or zero current
    liveCurrent = getDisplayCurrent(powerSource);

    // DHT (every 2s)
    if (now - lastDHT >= DHT_INTERVAL_MS) {
      lastDHT = now;
      temp = dht.readTemperature();
      hum  = dht.readHumidity();
    }

    // Debug print
    Serial.print("Mains: "); Serial.print(mainsVoltage, 2);
    Serial.print("V  Battery: "); Serial.print(batteryVoltage, 2);
    Serial.print("V  Current: "); Serial.print(liveCurrent, 2);
    Serial.print("A  Source: "); Serial.println(powerSource);
  }

  // --- LCD Update ---
  if (now - lastLCD >= LCD_INTERVAL_MS) {
    lastLCD = now;

    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 0);
    lcd.print(powerSource);
    lcd.print(" ");

    if (powerSource == "Mains") lcd.print(mainsVoltage, 1);
    else if (powerSource == "Batt") lcd.print(batteryVoltage, 1);
    else lcd.print(0.0, 1);

    lcd.print("V ");
    lcd.print(liveCurrent, 1);
    lcd.print("A");

    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("T:");
    if (isnan(temp)) lcd.print("ERR ");
    else { lcd.print(temp, 0); lcd.print((char)223); lcd.print("C "); }

    lcd.print("H:");
    if (isnan(hum)) lcd.print("ERR");
    else { lcd.print(hum, 0); lcd.print("%"); }
  }

  // --- Bluetooth Control ---
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case '1': digitalWrite(fanRelay, HIGH);
                digitalWrite(ledRelay, LOW);
                digitalWrite(usbRelay, LOW);
                digitalWrite(extraRelay, LOW);
                break;
      case '2': digitalWrite(fanRelay, LOW);
                digitalWrite(ledRelay, HIGH);
                digitalWrite(usbRelay, LOW);
                digitalWrite(extraRelay, LOW);
                break;
      case '3': digitalWrite(fanRelay, LOW);
                digitalWrite(ledRelay, LOW);
                digitalWrite(usbRelay, HIGH);
                digitalWrite(extraRelay, LOW);
                break;
      case '4': digitalWrite(fanRelay, LOW);
                digitalWrite(ledRelay, LOW);
                digitalWrite(usbRelay, LOW);
                digitalWrite(extraRelay, LOW);
                break;
    }
  }

  delay(2);
}