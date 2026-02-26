#define colorswitch_pin 40

void setup() {
  pinMode(colorswitch_pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  //Serial.println(analogRead(colorswitch_pin));
  if(digitalRead(colorswitch_pin) == 1) {
    Serial.println("HIGH");
  } else {
    Serial.println("LOW");
  }
}
