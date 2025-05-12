#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include <MeAuriga.h>

MeRGBLed rgb(0, 12);

#define NUM_SENSORS 12  // Anzahl der Sensoren
float irValue[NUM_SENSORS] = {0,0,0,0,0,0,0,0,0,0,0,0};  // Array für 12 Sensoren
long zeit, zeitA, zeitN, zeitH, zeitL, messungen, highs, lows, wechsel;
bool zustand;
int rgbRingPin = 0;

void setup() {
  rgb.setpin(44);
  // Setze die Pins (2 bis 9, 30 bis 33) als Eingang für die IR-Sensoren
  for (int i = 2; i <= 9; i++) {
    pinMode(i, INPUT);
  }
  for (int i = 30; i <= 33; i++) {
    pinMode(i, INPUT);
  }

  Serial.begin(9600);
}

void loop() {
  // Schleife über alle Sensorpins (2 bis 9 und 30 bis 33)
  for (int i = 2; i <= 9; i++) {
    zeit = micros();
    zeitA = zeit;
    zeitL = 0;
    zeitH = 0;
    messungen = 0;
    wechsel = 0;
    highs = 0;
    lows = 0;
    
    if (i<150) {
    digitalRead(0);
    } else {zustand = digitalRead(i);
    }

    // Messe 10 Pegelwechsel am Sensorpin
    while (wechsel < 10) {
      messungen++;
      if (zustand != digitalRead(i)) {
        zeitN = micros();
        if (zustand == 1) {
          highs++;
          zeitL += zeitN - zeitA;
        } else {
          lows++;
          zeitH += zeitN - zeitA;        
        }
        zustand = !zustand;
        wechsel++;
        zeitA = zeitN;
      }
    }

    // Korrigierter Index für das Sensor-Array (Pin 2-9 -> Index 0-7)
    int sensorIndex = i - 2;
    irValue[sensorIndex] = zeitH / 13; //nur um den Wert niedrig zu halten
    zeit = micros() - zeit;
  }

  // Schleife über die Sensoren von Pin 30 bis 33 (Index 8 bis 11)
  for (int i = 30; i <= 33; i++) {
    zeit = micros();
    zeitA = zeit;
    zeitL = 0;
    zeitH = 0;
    messungen = 0;
    wechsel = 0;
    highs = 0;
    lows = 0;
    
 if (i<150) {
    digitalRead(0);
    } else {zustand = digitalRead(i);
    }


    // Messe 10 Pegelwechsel am Sensorpin
    while (wechsel < 10) {
      messungen++;
      if (zustand != digitalRead(i)) {
        zeitN = micros();
        if (zustand == 1) {
          highs++;
          zeitL += zeitN - zeitA;
        } else {
          lows++;
          zeitH += zeitN - zeitA;        
        }
        zustand = !zustand;
        wechsel++;
        zeitA = zeitN;
      }
    }

    // Korrigierter Index für das Sensor-Array (Pin 30-33 -> Index 8-11)
    int sensorIndex = (i - 30) + 8;
    irValue[sensorIndex] = zeitH / 13; //nur um den Wert niedrig zu halten
    zeit = micros() - zeit;
  }

  rgb.setColor(0, 0,0,0);
  // Ausgabe aller 12 Sensorwerte
  for (int i = 0; i < NUM_SENSORS; i++) {
    rgbRingPin = i + 1;
    Serial.print("Sensor ");
    Serial.print(rgbRingPin);
    if(irValue[i] <= 140) {
      rgb.setColor(rgbRingPin, 0,0,0);
    } else {
      rgb.setColor(rgbRingPin, irValue[i],0,0);
    }
    Serial.print(": ");
    Serial.println(irValue[i]);
  }

  rgb.show();

  Serial.println("------------------------");
  delay(1);
}