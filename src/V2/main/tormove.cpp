#include <Arduino.h>

void tormove(int torcolor) {
  getTor(torcolor);
  moveTor();
}

//--Nebenfunktionen von tormove------------------

void getTor(int torcolor) {
  if (torcolor == 1) { //gelb
    pixy.ccc.getBlocks();
    if (pixy.ccc.numBlocks) {
      for (int i = 0; i < pixy.ccc.numBlocks; i++) {
        if (pixy.ccc.blocks[i].m_signature == 1) {
          torX = pixy.ccc.blocks[i].m_x;
        }

      }
    }
  }
  else if (torcolor == 2) { //blau
    pixy.ccc.getBlocks();
    if (pixy.ccc.numBlocks) {
      for (int i = 0; i < pixy.ccc.numBlocks; i++) {
        if (pixy.ccc.blocks[i].m_signature == 2) {
          torX = pixy.ccc.blocks[i].m_x;
        }

      }
    }
  }
}

void moveTor() {
  if (torX >= torDeadzoneMin && torX <= torDeadzoneMax) {
    move(Fahren, Fahren);
  }
  else {
    if (torX <= torDeadzoneMin) {
      move(Drehen, Drehen * -1); //Panzerdrehung
    }


    if (torX >= torDeadzoneMax) {
      move(Drehen * -1, Drehen); //Panzerdrehung
    }
  }
}