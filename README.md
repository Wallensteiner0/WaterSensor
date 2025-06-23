# WaterSensor – Automatische Wasserstandserkennung & Pumpensteuerung

**Autor:** wallensteiner0@gmail.com  
**Stand:** Juni 2025

Dieses Projekt steuert eine Pumpe anhand von drei kapazitiven Wasserstandssensoren (10 %, 50 %, 80 %) mit einem ESP8266 (NodeMCU). Die Steuerung ist robust gegen Störungen und sorgt für zuverlässigen Betrieb.

---

## Pinbelegung (NodeMCU)

| Funktion         | Pin (NodeMCU) |
|------------------|:-------------:|
| Sensor 10 %      | D1            |
| Sensor 50 %      | D2            |
| Sensor 80 %      | D3            |
| Status-LED       | D4            |
| Sensor Common    | D5            |
| Pumpe/Relais     | D7            |

---

## Hardware

- ESP8266 (NodeMCU)
- Kapazitive Wasserstandssensoren (10 %, 50 %, 80 %)
- Pumpe
- Relaismodul
- Widerstände
- Transistor
- Diode
- LED
- Buzzer
- Jumper Kabel
- Steckbrett

## Software

- Arduino IDE **oder** PlatformIO (empfohlen)
- ESP8266 Board Paket
- Bibliotheken für Sensoren und WiFi

## Installation

1. Schließe die Hardware gemäß dem Schaltplan an.
2. Installiere die benötigten Bibliotheken in der Arduino IDE oder PlatformIO.
3. Lade den Sketch auf den ESP8266 hoch.
4. Konfiguriere ggf. die WiFi-Zugangsdaten im Sketch.
5. Kalibriere die Sensoren bei Bedarf.
6. Teste die Funktionalität der Pumpe und Sensoren.

## Nutzung

Die Pumpe wird automatisch gesteuert, um den Wasserstand im gewünschten Bereich zu halten.  
- Bei zu niedrigem Wasserstand wird die Pumpe eingeschaltet, bei zu hohem Wasserstand ausgeschaltet.  
- Die Sensoren überwachen kontinuierlich den Wasserstand und melden Änderungen an den ESP8266.

## Wartung

- Überprüfe regelmäßig die Funktion der Sensoren und der Pumpe.
- Reinige die Sensoren bei Bedarf, um falsche Messwerte zu vermeiden.
- Achte darauf, dass die Pumpe und die elektrischen Komponenten vor Wasser geschützt sind.

## Anhang

- Schaltplan
- Gehäusebauanleitung
- Kalibrierungsanleitung für die Sensoren
- Fehlerbehebung bei häufigen Problemen

---

## Kontakt

Bei Rückfragen: **wallensteiner0@gmail.com**

---

## Lizenz

Dieses Projekt ist lizenziert unter der MIT-Lizenz. Siehe die Datei LICENSE für weitere Informationen.

## Danksagungen

- Vielen Dank an die Entwickler der verwendeten Bibliotheken.
- Danke an die Community für die Unterstützung und Inspiration.