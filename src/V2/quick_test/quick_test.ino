#include <MeAuriga.h>

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if(Serial.available())
  {
    char a = Serial.read();
    switch(a)
    {
      case '0':
      Encoder_1.setTarPWM(0);
      Encoder_2.setTarPWM(0);
      break;
      case '1':
      Encoder_1.setTarPWM(100);
      Encoder_2.setTarPWM(-100);
      break;
      case '2':
      Encoder_1.setTarPWM(200);
      Encoder_2.setTarPWM(-200);
      break;
      case '3':
      Encoder_1.setTarPWM(255);
      Encoder_2.setTarPWM(-255);
      break;
      case '4':
      Encoder_1.setTarPWM(-100);
      Encoder_2.setTarPWM(100);
      break;
      case '5':
      Encoder_1.setTarPWM(-200);
      Encoder_2.setTarPWM(200);
      break;
      case '6':
      Encoder_1.setTarPWM(-255);
      Encoder_2.setTarPWM(255);
      break;
      default:
      break;
    }
  }
}

