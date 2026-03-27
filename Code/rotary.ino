#define ENCODER_CLK 2
#define ENCODER_DT  3
#define ENCODER_BTN 4

void setup() {
  Serial.begin(9600);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_CLK), readEncoder, FALLING);

  pinMode(ENCODER_BTN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void readEncoder() {
  int dtValue = digitalRead(ENCODER_DT);
  if (dtValue == HIGH) {
    Serial.println("Rotated clockwise ⏩");
  }
  if (dtValue == LOW) {
    Serial.println("Rotated counterclockwise ⏪");
  }
}

void loop() {
  if (digitalRead(ENCODER_BTN) == LOW) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}