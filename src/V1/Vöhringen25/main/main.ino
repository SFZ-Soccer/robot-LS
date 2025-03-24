#include <MeEncoderOnBoard.h>
#include <MeAuriga.h>
#include <Pixy2.h>

volatile unsigned long alteZeitR, alteZeitL, neueZeitR, neueZeitL, zeitLowR, zeitLowL, zeitHighR, zeitHighL,zeitHighRneu,zeitHighLneu,Ra,Rb,Rc,Rd,Re,La,Lb,Lc,Ld,Le,erg1,erg2;
int m1dataspeed,m2dataspeed;
int torDeadzoneMin = 120; //200
int torDeadzoneMax = 220; //280
int torX;
int width;
long stehzeit = 0;
int x;
int Entfernung_Ball = 7;
int Drehen = 75; //75
int Fahren = 170; //170
const int wackelgrenze = 645;
MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links
MeUltrasonicSensor ultraSensor(PORT_7);
MePotentiometer potentiometer_6(8);
MeRGBLed rgb(0, 12);
Pixy2 pixy;

//--------------------------------------------- Encoder init

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

//--------------------------------------------- OWN FUNKTIONS

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

  if(torcolor == 1) { //gelb
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
  } else if(torcolor == 2) { //blau
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
  rgb.setColor(12,0,0,180); //grün
  rgb.show();
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

}

void IRmove() {
  if(abs(zeitHighLneu - zeitHighRneu) > 70){ // Sehr unterschiedliche Werte der Sensoren
    /*if(zeitHighLneu >= wackelgrenze || zeitHighRneu >= wackelgrenze) {
      rgb.setColor(12,0,180,0); //grün
      rgb.show();
      move(Fahren,Fahren);
    } else {*/
      rgb.setColor(12,0,180,0); //grün
      rgb.show();
      if(zeitHighLneu >= zeitHighRneu) {
      //move(Drehen,0); //Normal
      move(Drehen,Drehen *-1); //Panzerdrehung
      } else {
        //move(0,Drehen); //Normal
        move(Drehen *-1,Drehen); //Panzerdrehung
      } 
    //}
  } else {
    if(zeitHighLneu >= 100){
      rgb.setColor(12,180,0,0); //rot
      rgb.show();
      move(Fahren,Fahren);
    } else {
      rgb.setColor(6,180,0,0); //rot
      rgb.show();
      move(Drehen,Drehen *-1); //Panzerdrehung
    }
  }
}

//----------------------------------------------

void setup () {
  rgb.setpin(44);
  Serial.begin (9600);
  pixy.init();
  attachInterrupt(digitalPinToInterrupt(3), detectedR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(2), detectedL, CHANGE);

  attachInterrupt(m1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(m2.getIntNum(), isr_process_encoder2, RISING);
}

void loop() {
  erg1 = Ra + Rb + Rc + Rd + Re;
  zeitHighRneu = erg1 / 4;
  erg2 = La + Lb + Lc + Ld + Le;
  zeitHighLneu = erg2 / 4;

  m1.loop();
  m2.loop();

  //dataoutput();

  if(m1.getCurrentSpeed() || m2.getCurrentSpeed() <= 10.00 && m1.getCurrentSpeed() || m2.getCurrentSpeed() >= -10.00) {
    stehzeit++;
    Serial.println(stehzeit);
    if(stehzeit >= 1500) {
      rgb.setColor(0,50,50,50); //rot
      rgb.show();
      move(-100,-100);
      delay(1000);
      move(-70,70);
      delay(350);

      rgb.setColor(0,0,0,0); //rot
      rgb.show();

      stehzeit = 0;
      return;
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
}



