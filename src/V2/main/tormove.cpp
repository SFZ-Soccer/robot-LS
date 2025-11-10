#include "tormove.h"
#include <MeAuriga.h>
#include <Wire.h>


int torX;
int torDeadzoneMin = 120; //200
int torDeadzoneMax = 220; //280
int Drehen = 120; //75
int Fahren = 220; //170

void tormove(int torcolor) {
  getTor(torcolor);
  moveTor();
}

//--Nebenfunktionen von tormove------------------

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