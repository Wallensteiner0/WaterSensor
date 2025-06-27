#include <Arduino.h>
#include <LittleFS.h>
#include <FS.h>

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Starte LittleFS-Test...");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed!");
  } else {
    Serial.println("LittleFS mount OK!");
    Dir dir = LittleFS.openDir("/");
    while (dir.next()) {
      Serial.print("Datei im FS: ");
      Serial.println(dir.fileName());
    }
    Serial.println("Directory-Listing abgeschlossen.");
  }
}

void loop() {}