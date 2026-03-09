#define TRIG_PIN_links A13
#define ECHO_PIN_links A7
#define TRIG_PIN_rechts A8
#define ECHO_PIN_rechts A14

int get_ultra_distance_links() { 
  digitalWrite(TRIG_PIN_links, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_links, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_links, LOW);

  // Echo-Zeit messen
  long duration = pulseIn(ECHO_PIN_links, HIGH);

  // Entfernung berechnen
  float distance = (duration * 0.0343) / 2;
  /*
  Serial.print("Entfernung: ");
  Serial.print(distance);
  Serial.println(" cm");
  */

  return distance;
}

int get_ultra_distance_rechts() { 
  digitalWrite(TRIG_PIN_rechts, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_rechts, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_rechts, LOW);

  // Echo-Zeit messen
  long duration = pulseIn(ECHO_PIN_rechts, HIGH);

  // Entfernung berechnen
  float distance = (duration * 0.0343) / 2;
  /*
  Serial.print("Entfernung: ");
  Serial.print(distance);
  Serial.println(" cm");
  */

  return distance;
}


void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN_rechts, OUTPUT);
  pinMode(ECHO_PIN_rechts, INPUT);
  pinMode(TRIG_PIN_links, OUTPUT);
  pinMode(ECHO_PIN_links, INPUT);
}

void loop() {
  // Trigger-Puls senden
  Serial.print(get_ultra_distance_links());
  Serial.print("  ");
  Serial.println(get_ultra_distance_rechts());
}
