/*
  1 = get ir Values
  2 = ultraschall r+l
  3 = motor
  4 = irvalue
  5 = colorswitc
*/




#include <MeAuriga.h>



MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links

void move(int m1speed, int m2speed) { //ohne Encoder //rechts, links
  m1.setMotorPwm(m1speed * 1 * -1); //Seiten Stimmen nicht unbedingt
  m2.setMotorPwm(m2speed * 1); //speedfaktor gelöscht
}


//----------------------------------------------------------------------------------------------------------------------------------------------


#define TRIG_PIN_links A13
#define ECHO_PIN_links A7
#define TRIG_PIN_rechts A8
#define ECHO_PIN_rechts A14

int get_ultra_distance_links() { 
  digitalWrite(TRIG_PIN_links, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_links, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_links, LOW);

  // Echo-Zeit messen
  long duration = pulseIn(ECHO_PIN_links, HIGH);

  // Entfernung berechnen
  float distance = (duration * 0.0343) / 2;
  /*
  Serial.print("Entfernung: ");
  Serial.print(distance);
  Serial.println(" cm");
  */

  return distance;
}

int get_ultra_distance_rechts() { 
  digitalWrite(TRIG_PIN_rechts, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_rechts, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_rechts, LOW);

  // Echo-Zeit messen
  long duration = pulseIn(ECHO_PIN_rechts, HIGH);

  // Entfernung berechnen
  float distance = (duration * 0.0343) / 2;
  /*
  Serial.print("Entfernung: ");
  Serial.print(distance);
  Serial.println(" cm");
  */

  return distance;
}


//----------------------------------------------------------------------------------------------------------------------------------------------

#define ldr_pin 66
int lichtWert = 0;

//----------------------------------------------------------------------------------------------------------------------------------------------

#define NUM_SENSORS 12

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
  Serial.println(""); 
}





//__________________________________________________________________________________________________________________





void setup()
{
  Serial.begin(9600);

  for (int i = 2; i <= 9; i++) {
    pinMode(i, INPUT);
  }
  for (int i = 30; i <= 33; i++) {
    pinMode(i, INPUT);
  }

  pinMode(TRIG_PIN_rechts, OUTPUT);
  pinMode(ECHO_PIN_rechts, INPUT);
  pinMode(TRIG_PIN_links, OUTPUT);
  pinMode(ECHO_PIN_links, INPUT);
  //pinMode(lichtPin, INPUT);
}

void loop()
{
  if(Serial.available())
  {
    char a = Serial.read();
    switch(a)
    {
      case '1': 
      get_irValue();
      Serial.println("#############################");
      break;
      case '2':
      Serial.print(get_ultra_distance_links());
      Serial.print("  ");
      Serial.println(get_ultra_distance_rechts());
      Serial.println("#############################");
      break;
      case'3':
      move(255,255);
      Serial.println("255,255");
      delay(1900);
      move(0,255);
      Serial.println("0,255");
      delay(1900);
      move(255,0);
      Serial.println("255,0");
      delay(1900);
      move(-100,-100);
      Serial.println("-100,-100");
      delay(1900);
      move(0,0);
      Serial.println("#############################");
      break;
      case '4':
      //lichtWert = analogRead(lichtPin); 
      Serial.println(lichtWert);        
      delay(500); 
      Serial.println("#############################");
      break;
      case '5':
      default:
      break;
    }
  }
}

