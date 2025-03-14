#include <MeEncoderOnBoard.h>
#include <MeAuriga.h>
#include <Pixy2.h>
#include <Wire.h>

volatile unsigned long alteZeitR, alteZeitL, neueZeitR, neueZeitL, zeitLowR, zeitLowL, zeitHighR, zeitHighL,zeitHighRneu,zeitHighLneu,Ra,Rb,Rc,Rd,Re,La,Lb,Lc,Ld,Le,erg1,erg2;
int m1dataspeed,m2dataspeed;
int torDeadzoneMin = 120; //200
int torDeadzoneMax = 220; //280
int torX;
int width;
int dribblerspeed = 100;
long stehzeit = 0;
int x;
int Entfernung_Ball = 4; //4.6
int Drehen = 90; //75
int Fahren = 190; //170
const int wackelgrenze = 645;
bool startbutton_pressed = false;
MeEncoderOnBoard m1(SLOT1); // rechts
MeEncoderOnBoard m2(SLOT2); // links
MeUltrasonicSensor ultraSensor(PORT_7);
Me4Button btn(PORT_9);
MePotentiometer potentiometer_6(8);
MeEncoderMotor dribbler(0x09,1);
MeRGBLed rgb(0, 12);
Pixy2 pixy;

uint8_t keyPressed = KEY_NULL;
uint8_t keyPressedPrevious = KEY_NULL;

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

//--------------------------------------------- IR Detection

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

//--------------------------------------------- MOVE

void move(int m1speed, int m2speed) {
  m1.setMotorPwm(m1speed *-1);
  m2.setMotorPwm(m2speed);
  m1dataspeed = m1speed;
  m2dataspeed = m2speed;
}

//--------------------------------------------- DATA

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

//--------------------------------------------- TOR

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
  //rgb.setColor(12,0,0,180); //grün
  //rgb.show();
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

//--------------------------------------------- IR MOVEMENT

void IRmove() {
  if(abs(zeitHighLneu - zeitHighRneu) > 70){ // Sehr unterschiedliche Werte der Sensoren
      //rgb.setColor(12,0,180,0); //grün
      //rgb.show();
      if(zeitHighLneu >= zeitHighRneu) {
        //move(Drehen,0); //Normal
        rgb.setColor(4,0,50,0); //grün
        rgb.show();
        move(Drehen,Drehen *-1); //Panzerdrehung
        rgb.setColor(4,0,0,0); //grün
        rgb.show();
      } else {
        //move(0,Drehen); //Normal
        rgb.setColor(8,0,50,0); //grün
        rgb.show();
        move(Drehen *-1,Drehen); //Panzerdrehung
        rgb.setColor(8,0,0,0); //grün
        rgb.show();
      } 
    //}
  } else {
    rgb.setColor(4,0,50,0); //grün
    rgb.setColor(8,0,50,0); //grün
    rgb.show();
    if(zeitHighLneu >= 100){
      //rgb.setColor(12,180,0,0); //rot
      //rgb.show();
      move(Fahren,Fahren);
    } else {
      //rgb.setColor(6,180,0,0); //rot
      //rgb.show();
      move(Drehen,Drehen *-1); //Panzerdrehung
      rgb.setColor(12,50,50,50); //grün
    }
    rgb.setColor(4,0,0,0); //grün
    rgb.setColor(8,0,0,0); //grün
    rgb.show();
  }
}

//----------------------------------------------

void setup () {
  rgb.setpin(44);
  Serial.begin (9600);
  pixy.init(); 
  rgb.setColor(0,0,0,0); 
  rgb.show();
  dribbler.begin();
  dribbler.runSpeed(dribblerspeed);
  attachInterrupt(digitalPinToInterrupt(2), detectedR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), detectedL, CHANGE);

  attachInterrupt(m1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(m2.getIntNum(), isr_process_encoder2, RISING);
}

void loop() {
  keyPressed = btn.pressed();
 
    if(startbutton_pressed == false){
      if (keyPressed == KEY_3){
        startbutton_pressed = true;
      }
    } else {
        erg1 = Ra + Rb + Rc + Rd + Re;
        zeitHighRneu = erg1 / 4;
        erg2 = La + Lb + Lc + Ld + Le;
        zeitHighLneu = erg2 / 4;

        m1.loop();
        m2.loop();

        //dataoutput();

        if((m1.getCurrentSpeed() <= 10.00 || m2.getCurrentSpeed() <= 10.00) && (m1.getCurrentSpeed() >= -10.00 || m2.getCurrentSpeed() >= -10.00)) { //10 - (-10)
          stehzeit++;
          Serial.println(stehzeit);
          if(stehzeit >= 1500) {
            rgb.setColor(6,50,0,0); //rot
            rgb.show();
            move(-100,-100);
            delay(1000);
            move(-70,70);
            delay(350);

            rgb.setColor(6,0,0,0); //rot
            rgb.show();

            stehzeit = 0;
            return;
          }
        } else {
            stehzeit = 0;
        }
        
        if(ultraSensor.distanceCm() >= Entfernung_Ball){
          rgb.setColor(6,25,25,0); //gelb
          rgb.show();
          IRmove();
          rgb.setColor(6,0,0,0); //gelb = 0
          rgb.show();
        } else {
          rgb.setColor(6,0,0,50); //blau
          rgb.show();
          if((potentiometer_6.read()) < (500)){
            getTor(1); //torcolor = 1
            moveTor(); 
          }
          if((potentiometer_6.read()) > (500)){
            //gelb
            getTor(2); //torcolor = 2
            moveTor(); 
          }

          rgb.setColor(6,0,0,0); //blau
          rgb.show();
        }
    }
}