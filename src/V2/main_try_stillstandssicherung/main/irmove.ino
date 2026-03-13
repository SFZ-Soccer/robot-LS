#define NUM_SENSORS 12

int irDeadzoneMin_x = 120;
int irDeadzoneMax_x = 220;

const int xm = -14;
const int x0m = -5; //8
//0
const int x0p = 5;
const int xp = 14;

//---------------

const int ym = -14;
const int y0m = -8;
//0
const int y0p = 8;
const int yp = 9;

int speed = 408; //408 = 100% //alt:270

float vorne = 0.6;
float hinten = 0.5;
float vorneseite = 0.19;
float seite = 0.27;
float hintenseite = 0.4;

float irValue[NUM_SENSORS] = {0};

const float irYcon[NUM_SENSORS] = { 
    1.0000, 0.8660, 0.5000, 0.0000, -0.5000, -0.8660, 
   -1.0000, -0.8660, -0.5000, -0.0000,  0.5000,  0.8660 
};

const float irXcon[NUM_SENSORS] = { 
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

    /*Serial.print("X:");
    Serial.print(powerX);
    Serial.print(",");
    Serial.print("Y:");
    Serial.println(powerY);*/

    driveToBall();

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
  /*
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(irValue[i]);
  }
  Serial.println("------------------------"); 
  */
}

// -------------------------------------------------

void get_irX() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        powerX += irValue[i] * irXcon[i];
    }
    powerX = powerX / NUM_SENSORS;
}

void get_irY() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        powerY += irValue[i] * irYcon[i];
    }
    powerY = powerY / NUM_SENSORS;
}

// -------------------------------------------------

void driveToBall() {

    int currentDir = -1;

    //move(rechts,links)

    if(powerX < x0p && powerX > x0m && powerY > 0) { //N
      move(speed * vorne, speed * vorne);
      currentDir = 0;
    }

    else if(powerX < xp && powerX > x0p && powerY > 0) { //NO
      move(speed * vorneseite * -1, speed * vorneseite);
      currentDir = 1;
    }

    else if(powerX > xp && powerY > y0m && powerY < y0p) { //O
      move(speed * seite * -1, speed * seite);
      currentDir = 2;
    }

    else if(powerX > x0p && powerX < xp && powerY < 0) { //SO
      move(speed * hintenseite * -1, speed * hintenseite);
      currentDir = 3;
    }

    else if(powerX < x0p && powerX > x0m && powerY < 0) { //S
      move(speed * hinten * -1, speed * hinten);
      currentDir = 4;
    }

    else if(powerX < x0m && powerX > xm && powerY < 0) { //SW
      move(speed * hintenseite, speed * hintenseite * -1);
      currentDir = 5;
    }

    else if(powerX < xm && powerY > y0m && powerY < y0p) { //W
      move(speed * seite, speed * seite * -1);
      currentDir = 6;
    }

    else if(powerX < x0m && powerX > xm && powerY > 0) { //NW
      move(speed * vorneseite, speed * vorneseite * -1);
      currentDir = 7;
    }

    // -------- Timer Logik --------

    if(currentDir != -1) {

        if(currentDir != lastDir) {
            // neue Bewegung → Timer neu starten
            dirStartTime = millis();
            lastDir = currentDir;
        }

        if(millis() - dirStartTime > ir_stand_timer) {
            change_dir(1);
            dirStartTime = millis(); // optional neu starten
        }
    }
}
