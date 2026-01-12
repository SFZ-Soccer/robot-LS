#define colorswitch_pin A10

void setup() {
  pinMode(colorswitch_pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(colorswitch_pin));
  /*if(analogRead(colorswitch_pin) <= 500) {
    Serial.println("HIGH");
  } else {
    Serial.println("LOW");
  }*/
}
