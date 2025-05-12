long zeit, zeitA, zeitN, zeitH, zeitL, messungen, highs, lows, wechsel;

bool zustand;

void setup() {
  pinMode(41, INPUT);
  Serial.begin(9600);
}

void loop() {
  zeit = micros();
  zeitA = zeit;
  zeitL = 0;
  zeitH = 0;
  messungen = 0;
  wechsel = 0;
  highs = 0;
  lows = 0;
  
  zustand = digitalRead(41);
  
  while(wechsel<10){
    messungen++;
    if(zustand != digitalRead(41)){
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
  
  zeit = micros()-zeit;
  
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