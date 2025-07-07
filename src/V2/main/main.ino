#include <Arduino.h>
#include <MeAuriga.h>
#include <Pixy2.h>
#include "irmove.h"
#include "anpassen.h"
#include "tormove.h"

MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links
Pixy2 pixy;

//void move(int m1speed, int m2speed) {
//void move(int m1speed, int m2speed) {

void move(int m1speed, int m2speed) {
  //m1.setMotorPwm(m1speed * -1);
  //m2.setMotorPwm(m2speed);
  //m1dataspeed = m1speed;
  //m2dataspeed = m2speed;
}


//--Setup und Loop--------------------------------------------------

void setup() {
  Serial.begin(9600);
  pixy.init();
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
}

void loop() {
  irmove();

  do {
    irmove();
  } while (true==false); // Schleife läuft, solange die Bedingung erfüllt (also Ball in fangeinrichtung)
}