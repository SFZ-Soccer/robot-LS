#include <Arduino.h>
#include <MeAuriga.h>
#include <Pixy2.h>
#include "irmove.h"
#include "tormove.h"

bool startbutton_pressed = false;

Pixy2 pixy;
MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links

void move(int m1speed, int m2speed) {
  m1.setMotorPwm(m1speed * -1);
  m2.setMotorPwm(m2speed);
  m1dataspeed = m1speed;
  m2dataspeed = m2speed;
}

//--Setup und Loop--------------------------------------------------

void setup() {
  Serial.begin(9600);
  pixy.init();
  attachInterrupt(m1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(m2.getIntNum(), isr_process_encoder2, RISING);
}

void loop() {
  if (startbutton_pressed == false) {
    keyPressed = btn.pressed();
    if (keyPressed == KEY_3) {
      startbutton_pressed = true;
    }
  }
  else {
    m1.loop();
    m2.loop();

    if ((m1.getCurrentSpeed() <= 10.00 || m2.getCurrentSpeed() <= 10.00) && (m1.getCurrentSpeed() >= -10.00 || m2.getCurrentSpeed() >= -10.00)) { //10 - (-10)
      stehzeit++;
      Serial.println(stehzeit);
      if (stehzeit >= 1500) {
        move(-120, -120);
        delay(1000);
        move(-90, 90);
        delay(1050);

        stehzeit = 0;
        return;
      }
    }
    else {
      stehzeit = 0;
    }

    if () {
      irmove();
    }
    else {
      if ((/*Poti oder SchalterWert */ < (500))
      {
        tormove(1); //torfarbe ist gelb
      }
      if (/*Poti oder SchalterWert */ > (500))
      {
        tormove(2); //torfarbe ist blau
      }
    }
  }
}