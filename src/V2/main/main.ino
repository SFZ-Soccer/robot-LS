#include <Arduino.h>
#include <MeAuriga.h>
#include <Pixy2.h>

#define relais_pin 41

//------------------------------------------------------------------------------
//main

float speedfaktor_l = 1;
float speedfaktor_r = 1;

//tormove

int torcolor = 1; //1= gelb; 2= blau

int torDeadzoneMin = 120; //200
int torDeadzoneMax = 220; //280
int Drehen = 120; //75
int Fahren = 220; //170

//irmove



Pixy2 pixy;

MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links

void move(int m1speed, int m2speed) { //ohne Encoder
  m1.setMotorPwm(m1speed * speedfaktor_l * -1); //Seiten Stimmen nicht unbedingt
  m2.setMotorPwm(m2speed * speedfaktor_r); //speedfaktor gelöscht
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
