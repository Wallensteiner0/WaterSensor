#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <FS.h>

// Pin-Konfiguration
const int sensor10Pin = D1;          // 10 % Füllstand
const int sensor50Pin = D2;          // 50 % Füllstand
const int sensor80Pin = D3;          // 80 % Füllstand
const int ledPin = D4;               // Status-LED
const int sensorCommonPin = D5;      // Der gemeinsame Empfangspin
const int pumpPin = D7;              // Schaltet Pumpe (Relais oder MOSFET)

// Zeitsteuerung
unsigned long sensorCheckInterval = 3000; // Zeit zwischen Checks (ms)
const unsigned long sensorCheckIntervalDefault = 3000;
const unsigned long sensorCheckIntervalFast = 1000;

// Zustandsvariablen
bool flag10 = false;
bool flag50 = false;  // Zu Beginn auf 0 (false) gesetzt
bool flag80 = false;
bool lastFlag10 = false;
bool isPumping = false;
unsigned long lastSensorCheck = 0;

// LED-Zustand
bool ledState = false;
unsigned long lastLedToggle = 0;

// Debug-Zähler
int pumpCycles = 0;

// Neue Hilfsvariablen:
int stable10 = 0, stable50 = 0, stable80 = 0;
const int stableLimit = 2; // wie viele Zyklen gleich sein müssen

// Funktion vorab deklarieren
void flashLED(int times);
void checkAllWaterLevels();
void pumpControl();
void handleRoot();

// Wifi Konfiguration
const char* ssidAP = "Wasserstandssensoren";
const char* ssid = "Freakymonkey";
const char* password = "84w8ldme";
ESP8266WebServer server(80);
String serialLog = "";

// ========== Setup ==========
void setup() {
  Serial.begin(115200);
  
  pinMode(pumpPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  delay(2000);

  Serial.println();
  Serial.println("Starte Wasserstandssensoren und Pumpensteuerung...");
  Serial.println("LittleFS initialisieren...");

  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed!");
  } else {
    Serial.println("LittleFS mount OK!");
    Dir dir = LittleFS.openDir("/");
    while (dir.next()) {
      Serial.print("Datei im FS: ");
      Serial.println(dir.fileName());
    }
  }

  flashLED(4); // LED blinkt 4x beim Start
  Serial.println();
  Serial.println("Wasserstandssensoren und Pumpensteuerung gestartet");
  pinMode(sensorCommonPin, INPUT_PULLUP); // Empfangspin

  digitalWrite(pumpPin, LOW);
  digitalWrite(ledPin, LOW); 
  delay(1000); // 1 Sekunde warten, um den Serial Monitor zu öffnen

  // WLAN-Verbindung herstellen
  Serial.print("Verbinde mit WLAN...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.printf("Verbunden mit WLAN: %s\n", ssid);
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());
  Serial.printf("Connection status: %d\n", WiFi.status());
  Serial.println();

  // Webserver Routen
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Webserver gestartet");
  Serial.print("erreichbar unter: http://");
  Serial.println(WiFi.localIP());
  Serial.println();
}

// ========== Sensorabfrage ==========
bool isTouched(int testPin, int commonPin) {
  pinMode(commonPin, INPUT_PULLUP);
  pinMode(testPin, OUTPUT);
  digitalWrite(testPin, LOW);
  delay(10); // debounce
  bool touched = digitalRead(commonPin) == LOW;
  pinMode(testPin, INPUT);
  pinMode(commonPin, INPUT);
  return touched;
}

// Mehrfachmessung für stabile Sensorwerte (5 Messungen, 500ms Abstand, 4 von 5 müssen stimmen)
bool isTouchedStable(int testPin, int commonPin) {
  int hits = 0;
  for (int i = 0; i < 4; i++) {
    if (isTouched(testPin, commonPin)) hits++;
    delay(500); // jetzt 500ms Abstand zwischen den Messungen
  }
  return hits >= 3;
}

void checkAllWaterLevels() {
  // Sensoren nacheinander mit 5-fach-Messung abfragen
  bool newFlag10 = isTouchedStable(sensor10Pin, sensorCommonPin);
  bool newFlag50 = isTouchedStable(sensor50Pin, sensorCommonPin);
  bool newFlag80 = isTouchedStable(sensor80Pin, sensorCommonPin);

  // Hysterese für 10%
  if (newFlag10 == flag10) {
    stable10 = 0;
  } else {
    stable10++;
    if (stable10 >= stableLimit) {
      flag10 = newFlag10;
      stable10 = 0;
    }
  }
  // Analog für 50% und 80%
  if (newFlag50 == flag50) {
    stable50 = 0;
  } else {
    stable50++;
    if (stable50 >= stableLimit) {
      flag50 = newFlag50;
      stable50 = 0;
    }
  }
  if (newFlag80 == flag80) {
    stable80 = 0;
  } else {
    stable80++;
    if (stable80 >= stableLimit) {
      flag80 = newFlag80;
      stable80 = 0;
    }
  }

  Serial.print("Füllstand Flags: ");
  Serial.print("10%:"); Serial.print(flag10 ? "1 " : "0 ");
  Serial.print("50%:"); Serial.print(flag50 ? "1 " : "0 ");
  Serial.print("80%:"); Serial.print(flag80 ? "1" : "0");
  Serial.println();

  // Loggen der Füllstand-Flags f. HTML Seite
  serialLog += "Füllstand Flags: 10%:" + String(flag10 ? "1 " : "0 ") +
               "50%:" + String(flag50 ? "1 " : "0 ") +
               "80%:" + String(flag80 ? "1" : "0") + "<br>";
  if (serialLog.length() > 2000) serialLog = ""; // Log begrenzen

  // Loggen der Pumpenzyklen
  // Intervall anpassen, wenn 80%-Flag aktiv wird
  if (flag80 && sensorCheckInterval != sensorCheckIntervalFast) {
    sensorCheckInterval = sensorCheckIntervalFast;
    Serial.println("80%-Flag erkannt, Sensor-Check-Intervall auf 1s gesetzt.");
  } else if (!flag80 && sensorCheckInterval != sensorCheckIntervalDefault) {
    sensorCheckInterval = sensorCheckIntervalDefault;
    Serial.println("Sensor-Check-Intervall zurück auf 3s gesetzt.");
  }

  // Pumpe starten, wenn 80%-Flag aktiv und Pumpe noch nicht läuft
  if (flag80 && !isPumping) {
    isPumping = true;
    digitalWrite(pumpPin, HIGH);
    Serial.println("Pumpe gestartet (80% erreicht)");
    flashLED(4); // 4x blinken beim Pumpenstart
  }

  // Pumpe stoppen, wenn 10%-Flag von 1 auf 0 wechselt, 50% und 80% sind 0 und Pumpe läuft
  if (lastFlag10 && !flag10 && isPumping && !flag50 && !flag80) {
    isPumping = false;
    digitalWrite(pumpPin, LOW);
    pumpCycles++;
    Serial.println("Pumpe gestoppt (10% unterschritten, 50% und 80% sind 0)");
    Serial.print("Gesamtstarts: ");
    Serial.println(pumpCycles);
    flashLED(4); // 4x blinken beim Pumpenstopp

    // Nach Erreichen von 10% nur noch alle 60 Sekunden messen
    sensorCheckInterval = 60000;
    Serial.println("10%-Flag gefallen, Sensor-Check-Intervall auf 60s gesetzt.");
  }

  lastFlag10 = flag10;
}

void pumpControl() {
  // Keine verzögerte Abschaltung mehr nötig
}

// ========== LED-Logik ==========

void updateLED(unsigned long now) {
  if (isPumping) {
    // Status-LED blinkt
    if (now - lastLedToggle > 500) {
      ledState = !ledState;
      digitalWrite(ledPin, ledState ? LOW : HIGH); // LOW = AN, HIGH = AUS
      lastLedToggle = now;
    }
    digitalWrite(LED_BUILTIN, LOW); // BUILTIN_LED AN
  }
  else if (flag50) {
    digitalWrite(ledPin, LOW);      // LED AN bei 50%
    digitalWrite(LED_BUILTIN, HIGH); // BUILTIN_LED AUS
  }
  else {
    digitalWrite(ledPin, HIGH);     // LED AUS
    digitalWrite(LED_BUILTIN, HIGH); // BUILTIN_LED AUS
  }
}

void flashLED(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(ledPin, LOW);   // LED AN
    delay(80);
    digitalWrite(ledPin, HIGH);  // LED AUS
    delay(80);
  }
}

void handleRoot() {
  File file = LittleFS.open("/status_page.html", "r");
  if (file) {
    String html = file.readString();
    html.replace("%10CLS%", flag10 ? "green" : "red");
    html.replace("%50CLS%", flag50 ? "green" : "red");
    html.replace("%80CLS%", flag80 ? "green" : "red");
    html.replace("%LOG%", serialLog);
    server.send(200, "text/html", html);
    file.close();
  } else {
    server.send(404, "text/plain", "File not found");
  }
}

void loop() {
  unsigned long now = millis();

  // Sensoren nacheinander alle 3 Sekunden prüfen
  if (now - lastSensorCheck > sensorCheckInterval) {
    lastSensorCheck = now;
    checkAllWaterLevels();
  }

  // Pumpensteuerung
  pumpControl();

  updateLED(now);

  // Webserver bedienen
  server.handleClient();
}