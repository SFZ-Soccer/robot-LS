#include <Arduino.h>
#include <MeAuriga.h>
#include <Pixy2.h>

#define relais_pin A11 //stimmt nicht
#define ldr_pin 66
#define startswitch_pin_grun 40
#define startswitch_pin_rot 41 
#define colorswitch_pin 44

//------------------------------------------------------------------------------
//main

float speedfaktor_l = 0.75;
float speedfaktor_r = 1;
int lichtWert = 1000;
const int ldr_schwelle = 970;

//tormove

int torcolor; //1= gelb; 2= blau

int torDeadzoneMin = 100; //200
int torDeadzoneMax = 240; //280
int Drehen = 100; //75
int Fahren = 220; //170

//irmove 

Pixy2 pixy;

MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links

void move(int m1speed, int m2speed) { //ohne Encoder //rechts, links
  m1.setMotorPwm(m1speed * speedfaktor_r * -1);
  m2.setMotorPwm(m2speed * speedfaktor_l); 
}

void isr_process_encoder1(void)
{
      if(digitalRead(m1.getPortB()) == 0){
            m1.pulsePosMinus();
      }else{
            m1.pulsePosPlus();
      }
}

void isr_process_encoder2(void)
{
      if(digitalRead(m2.getPortB()) == 0){
            m2.pulsePosMinus();
      }else{
            m2.pulsePosPlus();
      }
}


//--Setup und Loop--------------------------------------------------

void setup() {
  pixy.init();
  Serial.begin(9600);
  //attachInterrupt(m1.getIntNum(), isr_process_encoder1, RISING);
  //attachInterrupt(m2.getIntNum(), isr_process_encoder2, RISING);

  //rgb.setpin(44);
  // Setze die Pins (2 bis 9, 30 bis 33) als Eingang für die IR-Sensoren
  for (int i = 2; i <= 9; i++) {
    pinMode(i, INPUT);
  }
  for (int i = 30; i <= 33; i++) {
    pinMode(i, INPUT);
  }

  pinMode(startswitch_pin_grun, INPUT); //Buttons als INPUT deffinieren
  pinMode(startswitch_pin_rot, INPUT);
  pinMode(colorswitch_pin, INPUT);
  pinMode(relais_pin, OUTPUT);
  pinMode(ldr_pin, INPUT);
}

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



void _loop() {
  m1.loop();
  m2.loop();
}
