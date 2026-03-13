#include <Arduino.h>
#include <MeAuriga.h>

MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links

void move(int m1speed, int m2speed) { //ohne Encoder //rechts, links
  m1.setMotorPwm(m1speed * 1 * -1); //Seiten Stimmen nicht unbedingt
  m2.setMotorPwm(m2speed * 1); //speedfaktor gelöscht
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  move(255, 255); //rechts, links
}
