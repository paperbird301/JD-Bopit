int xPin = A0; // VRx
int yPin = A1; // VRy
int swPin = 8; // SW

void setup() {
  Serial.begin(9600);
}

void loop() {
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);
  int swVal = digitalRead(swPin);
  Serial.print("X: "); Serial.print(xVal);
  Serial.print(" | Y: "); Serial.println(yVal);
  delay(100);
}
