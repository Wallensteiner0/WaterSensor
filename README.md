# WaterSensor – Automatische Wasserstandserkennung & Pumpensteuerung


## Changelog

### Version 2.0 (Juni 2025)
- **Bootstrap-Design:** Die Weboberfläche ist jetzt modern, responsiv und übersichtlich.
- **Mehr WLANs:** Unterstützung für zwei WLAN-Netzwerke (automatischer Fallback).
- **WLAN-Daten ausgelagert:** Zugangsdaten werden sicher in `include/wifi_secrets.h` gespeichert (nicht im Repository).
- **Debug-Mode:** Einfacher Wechsel zwischen Debug- und Normalbetrieb, beeinflusst das Sensor-Intervall.
- **Serial- und Web-Log:** Die letzten 10 Log-Einträge werden mit Zeitstempel im Webinterface angezeigt (neueste oben).
- **Manueller Pumpenstart:** Pumpe kann über die Webseite unabhängig vom Füllstand für 10 Sekunden aktiviert werden.
- **Statusanzeige:** Sensor- und Pumpenstatus werden farbig und übersichtlich dargestellt.
- **Code-Optimierung:** Entfernen von AJAX, klassisches HTML-Reload für bessere Performance und Zuverlässigkeit.
- **Sicherer Umgang mit Zugangsdaten:** `wifi_secrets.h` ist in `.gitignore` eingetragen.

### Version 1.0
- Grundfunktion: Sensoren auslesen, Pumpe automatisch steuern, einfache Weboberfläche.

---

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
- Wasserstandssensoren (10 %, 50 %, 80 %) in Form von Kabel-Enden welche in das Wasser (in unterschiedl. Tiefen) ragen.
- Pumpe
- Relaismodul
- Widerstände
- LED
- Steckbrett
- Jumper Kabel


## Software

- Arduino IDE **oder** PlatformIO (empfohlen)
- ESP8266 Board Paket
- Bibliotheken für Sensoren und WiFi

## Installation

1. Schließe die Hardware gemäß dem Schaltplan an.
2. Installiere die benötigten Bibliotheken in der Arduino IDE oder PlatformIO.
3. Lade den Sketch auf den ESP8266 hoch.
4. Konfiguriere die WiFi-Zugangsdaten unter <wifi_secrets.h>
5. Kalibriere die Sensoren bei Bedarf.
6. Teste die Funktionalität der Pumpe und Sensoren.


## Nutzung

Die Pumpe wird automatisch gesteuert, um den Wasserstand im gewünschten Bereich zu halten.  
- Bei zu hohem Wasserstand wird die Pumpe eingeschaltet, bei zu niedigem Wasserstand ausgeschaltet.  
- Die Sensoren überwachen kontinuierlich den Wasserstand und melden Änderungen an den ESP8266.

## Wartung

- Überprüfe regelmäßig die Funktion der Sensoren und der Pumpe.
- Es wird empfohlen die Pumpe in einen Filter zu wickeln.
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