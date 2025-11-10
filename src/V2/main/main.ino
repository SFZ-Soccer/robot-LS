#include <Arduino.h>
#include <MeAuriga.h>
#include "irmove.h"
#include "anpassen.h"
#include "tormove.h"
#include <Pixy2.h>

#define relais_pin 41

Pixy2 pixy;

MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links

void move(int m1speed, int m2speed) { //ohne Encoder
  m1.setMotorPwm(m1speed * l_speedfaktor * -1); //Seiten Stimmen nicht unbedingt
  m2.setMotorPwm(m2speed * r_speedfaktor);
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
}

void loop() {
  _loop(); //loop für Encoder
  //digitalWrite(relais_pin, LOW);
  //Serial.println("Dribbler-OFF");
  // Schleife läuft solange der Ball nicht erkannt wurde
  //while (true==false) { //!ballDetected()
    irmove();
    //Serial.println("IR");
  //}

  //digitalWrite(relais_pin, HIGH);
  //Serial.println("Dribbler-ON");
}

void _loop() {
  m1.loop();
  m2.loop();
}
