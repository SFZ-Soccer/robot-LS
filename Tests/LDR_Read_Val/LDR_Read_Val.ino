int lichtPin = A12;   
int lichtWert = 0;

void setup() {
  Serial.begin(9600);
  pinMode(lichtPin, INPUT);  
}

void loop() {
  lichtWert = analogRead(lichtPin); 
  Serial.println(lichtWert);        
  delay(500);                       
}
