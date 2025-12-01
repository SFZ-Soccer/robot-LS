#include <Arduino.h>
#include <MeAuriga.h>
#include <Pixy2.h>

#define relais_pin 41
#define startswitch_pin 6 //Pin stimmt noch nicht
#define colorswitch_pin 7 //Pin stimmt noch nicht

//------------------------------------------------------------------------------
//main

float speedfaktor_l = 1;
float speedfaktor_r = 1;

//tormove

int torcolor; //1= gelb; 2= blau

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

  pinMode(startswitch_pin, INPUT); //Buttons als INPUT deffinieren
  pinMode(colorswitch_pin, INPUT);
}

void loop() {
  while (digitalRead(startswitch_pin) == 1) {
    _loop(); //loop für Encoder
  
    if (true==true) { //!ballDetected()
      if (digitalRead(colorswitch_pin) == 1) { 
        tormove(1); //gelb
      } else if (digitalRead(colorswitch_pin) == 0) {
        tormove(0); //blau
      }
    } else {
      irmove();
    }

    digitalWrite(relais_pin, HIGH); //Dribbler
  }
}

void _loop() {
  m1.loop();
  m2.loop();
}
