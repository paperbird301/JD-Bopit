const int TRIG_PIN = 10;
const int ECHO_PIN = 9;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  delay(500);
}

long getEchoDuration() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(4);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long start = micros();
  while (digitalRead(ECHO_PIN) == LOW) {
    if (micros() - start > 10000) return -1;
  }

  unsigned long echoStart = micros();
  while (digitalRead(ECHO_PIN) == HIGH) {
    if (micros() - echoStart > 100000) return -2;
  }

  return micros() - echoStart;
}

void loop() {
  long duration = getEchoDuration();
  Serial.print("Raw: ");
  Serial.println(duration);

  if (duration == -1) {
    Serial.println("No echo received (check wiring)");
  } else if (duration == -2) {
    Serial.println("Object too far (>4m)");
  } else {
    float distance_cm = duration * 0.0343 / 2.0;
    Serial.print("Duration: ");
    Serial.print(duration);
    Serial.print(" us  |  Distance: ");
    Serial.print(distance_cm, 1);
    Serial.println(" cm");
  }

  delay(100);
}