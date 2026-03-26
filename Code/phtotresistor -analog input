/*
  Arduino Voltage Threshold Detection
  Reads an analog voltage and checks if it exceeds 1.0 V.
  Works with Arduino UNO (10-bit ADC, 5V reference).
*/

const int analogPin = A0;       // Analog input pin
const float referenceVoltage = 5.0; // ADC reference voltage (V)
const int adcResolution = 1023; // 10-bit ADC max value
const float thresholdVoltage = 1.0; // Voltage threshold (V)
const int LED = 13;

void setup() {
  Serial.begin(9600); // Start serial communication
  pinMode(analogPin, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // Read raw ADC value
  int rawValue = analogRead(analogPin);

  // Convert ADC value to voltage
  float voltage = (rawValue * referenceVoltage) / adcResolution;

  // Print voltage
  Serial.print("Voltage: ");
  Serial.print(voltage, 3); // 3 decimal places
  Serial.println(" V");

  // Check if voltage exceeds threshold
  if (voltage > thresholdVoltage) {
    Serial.println("Voltage is ABOVE 1.0 V");
    digitalWrite(13,HIGH);

  } else {
    Serial.println("Voltage is BELOW or EQUAL to 1.0 V");
    digitalWrite(13,LOW);
  }

  delay(500); // Delay for readability
}
