void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
}

void loop() {
  int value = analogRead(A0);
  float degree = value*180.0/1023.0;
  Serial.print("Degrees = ");
  Serial.println(degree);

  Serial.print("Res = ");
  Serial.println(value);

  delay(1000);
}