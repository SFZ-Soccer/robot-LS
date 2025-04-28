//rot = 2
#define NUM_SENSORS 16

float irValue[NUM_SENSORS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
long zeit, zeitA, zeitN, zeitH, zeitL, messungen, highs, lows, wechsel;

bool zustand;

void setup() {
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  Serial.begin(9600);
}

void loop() {
  for(int i = 2; i<13; i++) { //weil der erste sensor an 2 ist geht es von 2 bis 13
    zeit = micros();
    zeitA = zeit;
    zeitL = 0;
    zeitH = 0;
    messungen = 0;
    wechsel = 0;
    highs = 0;
    lows = 0;
    
    zustand = digitalRead(i);

    while(wechsel<10){
      messungen++;
      if(zustand != digitalRead(i)){
        zeitN = micros();
        if(zustand == 1){
          highs++;
          zeitL += zeitN - zeitA;
        } else {
          lows++;
          zeitH += zeitN - zeitA;        
        }
        zustand = not zustand;
        wechsel++;
        zeitA = zeitN;
      }
    }

    irValue[i] = zeitH;
    zeit = micros()-zeit;
  }

  
  Serial.print("Micros:");
  Serial.println(zeit);

  Serial.print("Messungen:");
  Serial.println(messungen);
    
  Serial.print("Wechsel:");
  Serial.println(wechsel);
    
  Serial.print("Highs und Zeit:");
  Serial.print(highs);
  Serial.print(" im Schnitt: ");
  Serial.println(zeitH/5);
      
  Serial.print("Lows und Zeit:");
  Serial.print(lows);
  Serial.print(" im Schnitt: ");
  Serial.println(zeitL/5);

  Serial.println("------------------------");

  delay(1000);
}
