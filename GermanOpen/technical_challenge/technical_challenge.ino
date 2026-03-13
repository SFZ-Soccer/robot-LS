#include <Arduino.h>
#include <MeAuriga.h>

float speedfaktor_l = 1; //0.95
float speedfaktor_r = 1;

MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links

void move(int m1speed, int m2speed) { //ohne Encoder //rechts, links
  m1.setMotorPwm(m1speed * speedfaktor_r * -1);
  m2.setMotorPwm(m2speed * speedfaktor_l); 
}

void setup() {
  // put your setup code here, to run once:
  move(100, 200);
  delay(400);
  move(220,220);
  delay(2000);
  move(200,110);
  delay(3520);
  move(200,200);
  delay(4000);
  move(0,0);
}

void loop() {
  // put your main code here, to run repeatedly:
 
}
/*

  move(100, 200);
  delay(400);
  move(200,200);
  delay(2000);
  move(200,110);
  delay(3520);
  move(200,200);
  delay(4000);
  move(0,0);
  
*/
