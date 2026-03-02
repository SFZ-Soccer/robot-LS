#include <Arduino.h>
#include <MeAuriga.h>
#include <Pixy2.h>

#define relais_pin A11
#define ldr_pin 66
#define startswitch_pin_grun 44
#define startswitch_pin_rot 41 
#define colorswitch_pin 40

//------------------------------------------------------------------------------
//main

float speedfaktor_l = 0.95;
bool program_run = false;
float speedfaktor_r = 1;
int lichtWert = 1000;
const int ldr_schwelle = 970;

//tormove

int torcolor; //1= blau; 2= gelb //signature

int torDeadzoneMin = 100; //200
int torDeadzoneMax = 240; //280
int Drehen = 60; //100 //70
int Fahren = 170; //220

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
  Serial.begin(19200);
  pixy.setLamp(false, false);
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

  pinMode(startswitch_pin_grun, INPUT_PULLUP); //Buttons als INPUT deffinieren
  pinMode(startswitch_pin_rot, INPUT_PULLUP);
  pinMode(colorswitch_pin, INPUT);
  pinMode(relais_pin, OUTPUT);
  pinMode(ldr_pin, INPUT);
}

void loop() {
  //Serial.println("NO RUUUNN");
  move(0,0);

  if(digitalRead(startswitch_pin_grun) == 1) {
      program_run = true;
      if(digitalRead(colorswitch_pin) == 1) { //blau
        torcolor = 1;
      } else if (digitalRead(colorswitch_pin) == 0) {
        torcolor = 2;
      }
    } else if (digitalRead(startswitch_pin_rot) == 1) {
      program_run = false;
      analogWrite(relais_pin, 0); 
    }

  
  while(program_run == true) {
    //_loop();
    if(digitalRead(startswitch_pin_grun) == 1) {
      program_run = true;
    } else if (digitalRead(startswitch_pin_rot) == 1) {
      program_run = false;
      analogWrite(relais_pin, 0); 
    }

    //Serial.println("RUN");
    lichtWert = analogRead(ldr_pin);

    if (lichtWert > ldr_schwelle) {
      irmove();
      analogWrite(relais_pin, 200); 
      Serial.println("IR");
    } else {
      tormove(torcolor);
      analogWrite(relais_pin, 255); //Dribbler an
      Serial.println("TOR");
    }
  }
}



void _loop() {
  m1.loop();
  m2.loop();
}
