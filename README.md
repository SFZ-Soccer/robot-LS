# RoboCup Soccer Robot

Dieses Projekt (stand 12.01.2026) enthält die Software für unseren **Soccer RoboCup Robot**, entwickelt von unserem Team:

- **Simon** – Programmierer  
- **Luka** – Hardware

Der Roboter kann anhand von IR-Sensoren dem Ball folgen und über die Pixy2 Kamera Tore erkennen und ansteuern. Die Steuerung erfolgt über einen **MeAuriga Robotercontroller**.

---

## Inhaltsverzeichnis

- [Funktionen](#funktionen)  
- [Hardware](#hardware)  
- [Softwarestruktur](#softwarestruktur)  
- [Installation](#installation)  
- [Nutzung](#nutzung)  
- [Lizenz](#lizenz)

---

## Funktionen

- **Ballverfolgung (IR-Sensoren)**:  
  12 IR-Sensoren (Pins 2–9, 30–33) erkennen den Ball. Der Roboter berechnet die X- und Y-Richtung und fährt entsprechend darauf zu.

- **Torverfolgung (Pixy2 Kamera)**:  
  Der Roboter erkennt Tore anhand der Farben **Gelb** (Signature 1) und **Blau** (Signature 2) und steuert das Tor direkt an.

- **Motorsteuerung**:  
  Zwei Encoder-gesteuerte Motoren (rechts: SLOT1, links: SLOT2) werden über PWM angesteuert.  

- **Lichtabhängige Modi**:  
  Ein LDR-Sensor entscheidet, ob der Roboter IR-Modus (Ballverfolgung) oder Tormodus (Pixy2) aktiviert.

- **Einfache Konfigurierbarkeit**:  
  Variablen für Drehgeschwindigkeit, Fahrgeschwindigkeit, Deadzones und Schwellwerte sind direkt im Code anpassbar.

---

## Hardware

- **MeAuriga Roboterboard**  
- **Pixy2 Kamera**  
- **12 IR-Sensoren**  
- **Encoder-Motoren (SLOT1 & SLOT2)**  
- **Lichtsensor (LDR)**  
- **Relais für Zusatzfunktionen**  
- **Taster für Start/Stopp und Farbmodus**

**Anschlussübersicht:**

| Komponente        | Pin / Slot                |
|------------------|--------------------------|
| Motor rechts      | SLOT1                    |
| Motor links       | SLOT2                    |
| IR-Sensoren       | Pins 2–9, 30–33          |
| LDR               | Pin 66                   |
| Startknöpfe       | A15 (grün), A9 (rot)     |
| Farbmodus Switch  | A10                      |
| Relais            | Pin 41                   |
| Pixy2 I2C         | Standard I2C Pins        |

---

## Softwarestruktur

Das Projekt besteht aus drei Hauptdateien:

### 1. `main.ino`

- Initialisiert die **Pixy2 Kamera**, die **IR-Sensoren** und die **Motoren**.  
- Liest den **Lichtwert** vom LDR aus und entscheidet zwischen IR- und Tormodus.  
- Enthält die Grundfunktion `move(int m1speed, int m2speed)` für die Motorsteuerung.  

```cpp
void loop() {
  lichtWert = analogRead(ldr_pin);

  if (lichtWert > ldr_schwelle) {
    irmove();
    Serial.println("IR");
  } else {
    tormove(1);
    Serial.println("TOR");
  }
}
```

### 2. `ir_sensors.ino`

Dieses Modul liest die **IR-Sensoren** ein und berechnet die Richtungsvektoren `powerX` und `powerY`.  
Die Funktion `driveToBall()` steuert die Motoren entsprechend zur Ballposition.

**Hauptfunktionen:**

- `get_irValue()` – Liest die Werte aller 12 IR-Sensoren ein.  
- `get_irX()` – Berechnet den X-Richtungsvektor aus den Sensorwerten.  
- `get_irY()` – Berechnet den Y-Richtungsvektor aus den Sensorwerten.  
- `irmove()` – Hauptfunktion für die Ballverfolgung; ruft die oben genannten Funktionen auf und steuert die Motoren.

```cpp
int irmove() {
    get_irValue();
    get_irX();
    get_irY();
    driveToBall(powerX, powerY);
    powerX = 0;
    powerY = 0;
    return 0;
}
```

### 3. `tor_tracking.ino`

Dieses Modul steuert die **Pixy2 Kamera** zur Torerkennung und sorgt dafür, dass der Roboter das Tor korrekt anfährt.  
Die Steuerung basiert auf der X-Position des Tors, die von der Kamera ermittelt wird.

**Hauptfunktionen:**

- `tormove(int torcolor)` – Hauptfunktion für die Toransteuerung.  
  - Parameter `torcolor`: 1 = Gelb, 2 = Blau  
  - Ruft die Funktionen `getTor(torcolor)` und `moveTor()` auf.

- `getTor(int torcolor)` – Liest die X-Position des Tors aus der Pixy2-Kamera aus:
  - Prüft alle erkannten Blöcke der entsprechenden Farbe
  - Speichert die X-Koordinate in der Variablen `torX`

- `moveTor()` – Steuert die Motoren abhängig von der Torposition:
  - Wenn `torX` innerhalb der Deadzone → geradeaus  
  - Wenn `torX` links von der Deadzone → Drehung nach links  
  - Wenn `torX` rechts von der Deadzone → Drehung nach rechts  

**Beispiel:**

```cpp
void tormove(int torcolor) { //1 = gelb; 2= blau
  getTor(torcolor);
  moveTor();
}

void getTor(int torcolor) {
  pixy.ccc.getBlocks();
  if(pixy.ccc.numBlocks) { 
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      if ((torcolor == 1 && pixy.ccc.blocks[i].m_signature == 1) ||
          (torcolor == 2 && pixy.ccc.blocks[i].m_signature == 2)) {
        torX = pixy.ccc.blocks[i].m_x;
      }
    }
  }
}

void moveTor() {
  if(torX >= torDeadzoneMin && torX <= torDeadzoneMax) {
    move(Fahren,Fahren); // Geradeaus
  } else {
    if(torX <= torDeadzoneMin) {
      move(Drehen, Drehen * -1); // Links drehend
    }
    if(torX >= torDeadzoneMax) {
      move(Drehen * -1, Drehen); // Rechts drehend
    }
  }
}
```
**Erklärung:**

- **tormove(int torcolor)**  
  Diese Funktion ist der Einstieg für die Toransteuerung. Sie entscheidet anhand der übergebenen Farbe (`1 = Gelb`, `2 = Blau`), welche Blöcke von der Pixy2-Kamera berücksichtigt werden. Anschließend ruft sie `moveTor()` auf, um die Motoren entsprechend zu steuern.

- **getTor(int torcolor)**  
  Liest die Blöcke, die von der Pixy2-Kamera erkannt werden, aus.  
  - Filtert nach der gewünschten Farbe (Torfarbe).  
  - Speichert die X-Position des erkannten Tors in der Variablen `torX`.  
  - Diese X-Position wird als Grundlage für die Steuerung genutzt.

- **moveTor()**  
  Steuert die Motoren abhängig von der Position des Tors (`torX`):  
  - **Innerhalb der Deadzone** → Motoren fahren **geradeaus**.  
  - **Links von der Deadzone** → Dreht den Roboter nach **links**.  
  - **Rechts von der Deadzone** → Dreht den Roboter nach **rechts**.  

- **Variablen und Konstanten**:  
  - `Fahren` – Geschwindigkeit für geradeaus fahren.  
  - `Drehen` – Geschwindigkeit für Drehungen.  
  - `torDeadzoneMin` / `torDeadzoneMax` – Definieren die horizontale Toleranz für die Geradeausbewegung.  
  - `move(int m1speed, int m2speed)` – Funktion zum direkten Ansteuern der Motoren.  

**Hinweis:**  
Die Pixy2-Kamera liefert kontinuierlich Blockdaten. Das Modul prüft in jedem Durchlauf, ob ein passender Block erkannt wurde, und passt die Fahrtrichtung des Roboters dynamisch an.



