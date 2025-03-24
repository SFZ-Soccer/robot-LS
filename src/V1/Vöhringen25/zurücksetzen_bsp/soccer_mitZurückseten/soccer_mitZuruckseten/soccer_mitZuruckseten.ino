#include <MeEncoderOnBoard.h>
#include <MeAuriga.h>
#include <Pixy2.h>

volatile unsigned long alteZeitR, alteZeitL, neueZeitR, neueZeitL, zeitLowR, zeitLowL, zeitHighR, zeitHighL,zeitHighRneu,zeitHighLneu,Ra,Rb,Rc,Rd,Re,La,Lb,Lc,Ld,Le,erg1,erg2;
int m1dataspeed,m2dataspeed;
int torDeadzoneMin = 120; //200
int torDeadzoneMax = 200; //280
int torX;
long stehzeit = 0;
int width;
int x;
int Entfernung_Ball = 7;
int Drehen = 75; //60
int Fahren = 200; //120
const int wackelgrenze = 645;
MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links
MeUltrasonicSensor ultraSensor(PORT_7);
MePotentiometer potentiometer_6(8);
MeRGBLed rgb(0, 12);
Pixy2 pixy;

//---------------------------------------------

void isr_process_encoder1(void)
{
  if(digitalRead(m1.getPortB()) == 0)
  {
    m1.pulsePosMinus();
  }
  else
  {
    m1.pulsePosPlus();;
  }
}

void isr_process_encoder2(void)
{
  if(digitalRead(m2.getPortB()) == 0)
  {
    m2.pulsePosMinus();
  }
  else
  {
    m2.pulsePosPlus();
  }
}

void detectedR() {
  neueZeitR = micros();
  if(digitalRead(3) == HIGH) {
    zeitHighR = neueZeitR-alteZeitR;
  } else {  
    zeitLowR = neueZeitR-alteZeitR;
  }
  Ra = zeitHighR;
  Rb = zeitHighR;
  Rc =zeitHighR;
  Rd =zeitHighR;
  Re =zeitHighR;
  alteZeitR=neueZeitR;
}

void detectedL() {
  neueZeitL = micros();
  if(digitalRead(2) == HIGH) {
    zeitHighL = neueZeitL-alteZeitL;
  } else {  
    zeitLowL = neueZeitL-alteZeitL;
  }
  La = zeitHighL;
  Lb = zeitHighL;
  Lc =zeitHighL;
  Ld =zeitHighL;
  Le =zeitHighL;
  alteZeitL=neueZeitL;
}

void move(int m1speed, int m2speed) {
  m1.setMotorPwm(m1speed *-1);
  m2.setMotorPwm(m2speed);
  m1dataspeed = m1speed;
  m2dataspeed = m2speed;
}

void dataoutput() {
  Serial.print("IR-L-High:");
  Serial.println(zeitHighLneu);

  Serial.print(" --- IR-L-Low:");
  Serial.println(zeitLowL);

  Serial.print("IR-R-High:");
  Serial.println(zeitHighRneu);

  Serial.print(" --- IR-R-Low:");
  Serial.println(zeitLowR);

  Serial.print("M1R:");
  Serial.println(m1dataspeed);

  Serial.print("M2L:");
  Serial.println(m2dataspeed);

  Serial.print("X-TOR:");
  Serial.println(torX);
  
  Serial.print("DeadzoneMax:");
  Serial.println(torDeadzoneMax);

  Serial.print("DeadzoneMin:");
  Serial.println(torDeadzoneMin);
}


void getTor(int torcolor) {

  if(torcolor == 1) { //blau
    pixy.ccc.getBlocks();
  // Wenn Blöcke erkannt wurden (mindestens ein Block)
  if (pixy.ccc.numBlocks) { 
    // Gehe durch alle erkannten Blöcke
    for (int i = 0; i < pixy.ccc.numBlocks; i++) {
      // Hol die X- und Y-Koordinaten des Blocks

      if(pixy.ccc.blocks[i].m_signature == 1) {
        torX = pixy.ccc.blocks[i].m_x;
      //width = pixy.ccc.blocks[i].m_width;  // Breite des Blocks

      //torX = x + width / 2;
      }

    }
  }  
  } else if(torcolor == 2) { //gelb _____________________________________________
    pixy.ccc.getBlocks();
    // Wenn Blöcke erkannt wurden (mindestens ein Block)
    if (pixy.ccc.numBlocks) {
    // Gehe durch alle erkannten Blöcke
      for (int i = 0; i < pixy.ccc.numBlocks; i++) {


        if(pixy.ccc.blocks[i].m_signature == 2) {
        torX = pixy.ccc.blocks[i].m_x;
      //width = pixy.ccc.blocks[i].m_width;  // Breite des Blocks

      //torX = x + width / 2;
      }

      }
    }   
  }
  
}

void moveTor() {
  //x = 0 bis 318
  //move(Drehen,Drehen *-1); //Tor suchen

  if(torX >= torDeadzoneMin && torX <= torDeadzoneMax) {
    //gerade aus
    move(Fahren,Fahren);
  } else {
    if(torX <= torDeadzoneMin) {
      //
      move(Drehen,Drehen *-1); //Panzerdrehung
    }
    if(torX >= torDeadzoneMax) {
      //rechts
      move(Drehen *-1,Drehen); //Panzerdrehung
    }
  }

  //torX = 0;
}

void IRmove() {
  if(abs(zeitHighLneu - zeitHighRneu) > 70){
          if(zeitHighLneu >= wackelgrenze || zeitHighRneu >= wackelgrenze) {
            //rgb.setColor(12,0,180,0); //grün
            //rgb.show();
            move(Fahren,Fahren);
          } else {
            if(zeitHighLneu >= zeitHighRneu) {
            //move(Drehen,0); //Normal
            move(Drehen,Drehen *-1); //Panzerdrehung
          } else {
             //move(0,Drehen); //Normal
             move(Drehen *-1,Drehen); //Panzerdrehung
          } 
          }
       } else {
          //rgb.setColor(12,180,0,0); //rot
          //rgb.show();
          move(Fahren,Fahren);
  }
}

//----------------------------------------------

void setup () {
  //rgb.setpin(44);
  Serial.begin (9600);
  pixy.init();
  attachInterrupt(digitalPinToInterrupt(3), detectedR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(2), detectedL, CHANGE);

  attachInterrupt(m1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(m2.getIntNum(), isr_process_encoder2, RISING);

  //Set PWM 8KHz
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  m1.setPulse(9);
  m2.setPulse(9);
  m1.setRatio(39.267);
  m2.setRatio(39.267);
  m1.setPosPid(0.18,0,0);
  m2.setPosPid(0.18,0,0);
  m1.setSpeedPid(0.18,0,0);
  m2.setSpeedPid(0.18,0,0);
}

void loop() {
  erg1 = Ra + Rb + Rc + Rd + Re;
  zeitHighRneu = erg1 / 4;
  erg2 = La + Lb + Lc + Ld + Le;
  zeitHighLneu = erg2 / 4;

  //dataoutput();

  if(m1.getCurrentSpeed() && m2.getCurrentSpeed() <= 10.00) {
      stehzeit++;
      Serial.println(stehzeit);
      if(stehzeit >= 200) {
        move(-100,-100);
        delay(2000);
      }
    } else {
      stehzeit = 0;
    }
  
  if(ultraSensor.distanceCm() >= Entfernung_Ball ){
    IRmove();
  } else {
    if((potentiometer_6.read()) < (500)){
      //blau
      getTor(1); //torcolor = 1
      moveTor(); 
    }
    if((potentiometer_6.read()) > (500)){
      //gelb
      getTor(2); //torcolor = 2
      moveTor(); 
    }
  }
  m1.loop();
  m2.loop();
}



