#include <Arduino.h>

#define NUM_SENSORS 12

int irDeadzoneMin_x = 120;
int irDeadzoneMax_x = 220;

float irValue[NUM_SENSORS] = {0};

const float irXcon[NUM_SENSORS] = { 
    1.0000, 0.8660, 0.5000, 0.0000, -0.5000, -0.8660, 
   -1.0000, -0.8660, -0.5000, -0.0000,  0.5000,  0.8660 
};

const float irYcon[NUM_SENSORS] = { 
    0.0000,  0.5000,  0.8660,  1.0000,  0.8660,  0.5000, 
    0.0000, -0.5000, -0.8660, -1.0000, -0.8660, -0.5000 
};

long zeit, zeitA, zeitN;
long zeitH, zeitL;
long messungen, highs, lows, wechsel;
bool zustand;

float powerY = 0;
float powerX = 0;

// -------------------------------------------------

int irmove() {
    get_irValue();
    get_irX();
    get_irY();

    Serial.print("Null,");
    Serial.print("X:");
    Serial.print(powerX);
    Serial.print(",");
    Serial.print("Y:");
    Serial.println(powerY);

    driveToBall(powerX, powerY);

    powerX = 0;
    powerY = 0;

    return 0;
}

// -------------------------------------------------

void get_irValue() {

  // Pins 2–9
  for (int i = 2; i <= 9; i++) {

    zeitA = micros();
    zeitH = 0;
    zeitL = 0;
    wechsel = 0;

    zustand = digitalRead(i);
    unsigned long startZeit = micros();

    while (wechsel < 10 && micros() - startZeit < 3000) {
      if (zustand != digitalRead(i)) {
        zeitN = micros();
        if (zustand == HIGH) zeitL += zeitN - zeitA;
        else zeitH += zeitN - zeitA;

        zustand = !zustand;
        wechsel++;
        zeitA = zeitN;
      }
    }

    int sensorIndex = i - 2;
    irValue[sensorIndex] = zeitH / 13.0;
  }

  // Pins 30–33
  for (int i = 30; i <= 33; i++) {

    zeitA = micros();
    zeitH = 0;
    zeitL = 0;
    wechsel = 0;

    zustand = digitalRead(i);
    unsigned long startZeit = micros();

    while (wechsel < 10 && micros() - startZeit < 3000) {
      if (zustand != digitalRead(i)) {
        zeitN = micros();
        if (zustand == HIGH) zeitL += zeitN - zeitA;
        else zeitH += zeitN - zeitA;

        zustand = !zustand;
        wechsel++;
        zeitA = zeitN;
      }
    }

    int sensorIndex = (i - 30) + 8;
    irValue[sensorIndex] = zeitH / 13.0;
  }

  // Debug-Ausgabe
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(irValue[i]);
  }
  Serial.println("------------------------");
}

// -------------------------------------------------

void get_irX() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        powerX += irValue[i] * irXcon[i];
    }
    powerX /= NUM_SENSORS;
}

void get_irY() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        powerY += irValue[i] * irYcon[i];
    }
    powerY /= NUM_SENSORS;
}

// -------------------------------------------------

void driveToBall(float powerX, float powerY) {
    if (powerX < irDeadzoneMin_x) {
        // links
    } 
    else if (powerX > irDeadzoneMax_x) {
        // rechts
    } 
    else {
        // geradeaus
    }
}
