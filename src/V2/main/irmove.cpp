#include <Arduino.h>
#include "irmove.h"
#include "tormove.h"
#define NUM_SENSORS 12 //Anzahl Sensoren

//MeRGBLed rgb(0, 12);

float irValue[NUM_SENSORS] = {0,0,0,0,0,0,0,0,0,0,0,0};  // Array für 12 Sensoren //Speicherplatz für die Sensorenwerte
const float irYcon[NUM_SENSORS] = { //Y constanten zur Berechnung von PowwerY
=======
#include <MeAuriga.h>
#include "irmove.h"
#include "tormove.h"
#define NUM_SENSORS 12

MeRGBLed rgb(0, 12);

float irValue[NUM_SENSORS] = {0,0,0,0,0,0,0,0,0,0,0,0};  // Array für 12 Sensoren
const float irXcon[NUM_SENSORS] = { 
>>>>>>> 6cbc0f656c87e51c22c3525a176c31ed5147b23b
    1.0000, 0.8660, 0.5000, 0.0000, -0.5000, -0.8660, 
    -1.0000, -0.8660, -0.5000, -0.0000, 0.5000, 0.8660 
};

const float irXcon[NUM_SENSORS] = { //X constanten zur Berehnung von PowerX
=======
const float irYcon[NUM_SENSORS] = { 
>>>>>>> 6cbc0f656c87e51c22c3525a176c31ed5147b23b
    0.0000, 0.5000, 0.8660, 1.0000, 0.8660, 0.5000, 
    0.0000, -0.5000, -0.8660, -1.0000, -0.8660, -0.5000 
};

long zeit, zeitA, zeitN, zeitH, zeitL, messungen, highs, lows, wechsel; //Benötigt für die Messung
bool zustand; //Zustand des IR-Sensors HIGH/LOW
=======
long zeit, zeitA, zeitN, zeitH, zeitL, messungen, highs, lows, wechsel;
bool zustand;
int rgbRingPin = 0;
>>>>>>> 6cbc0f656c87e51c22c3525a176c31ed5147b23b

float powerY = 0; //Durschnittlicher Wert nach vorne/hinten berechnet aus den irWerten und den Constanten
float powerX = 0; //Durschnittlicher Wert nach links/rechts berechnet aus den irWerten und den Constanten

int irmove() {
    get_irValue(); //Ir Sensor Werte einmal Durchmessen
    get_irX(); //powerX berechnen
    get_irY(); //powerY berechnen
    Serial.print("Null:"); //Serial Plotter ausgabe
    Serial.print(0);
    Serial.print(",");
    Serial.print("X:");
    Serial.print(powerX);
    Serial.print(",");
    Serial.print("Y:");
    Serial.println(powerY);

    powerY = 0; // Zurücksetzen der Werte
    powerX = 0;

    delay(100); //Wartepause für die Lesbarkeit
}   

//--Nebenfunktionen von IRmove ------------------------

void get_irValue() {
    // Schleife über alle Sensorpins (2 bis 9 und 30 bis 33) (2 for-Schleiefen)
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
    
  zustand = digitalRead(i);
  

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
    
    zustand = digitalRead(i);
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
>>>>>>> 6cbc0f656c87e51c22c3525a176c31ed5147b23b
}

void get_irX() {
    for (int i = 0; i < NUM_SENSORS; i++) //Berechnen des powerX Wertes aller 12 Sensoren
    {
        powerX += irValue[i] * irXcon[i]; //Der Wert des Sensors * den Einfluss des Sensors auf die X-Entfernung zum Ball (X-Constante)
    }
    powerX / NUM_SENSORS;
}

void get_irY() {
    for (int i = 0; i < NUM_SENSORS; i++) //Berechnen des powerX Wertes aller 12 Sensoren
    {
        powerY += irValue[i] * irYcon[i]; //Der Wert des Sensors * den Einfluss des Sensors auf die Y-Entfernung zum Ball (Y-Constante)
    }

    powerY / NUM_SENSORS;
}       