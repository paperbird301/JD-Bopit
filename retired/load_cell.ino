#include "HX711.h"

const int LOADCELL_DOUT_PIN = 6;
const int LOADCELL_SCK_PIN  = 5;
const float CALIBRATION_FACTOR = 500.0; // Adjust until known weight reads correctly

HX711 scale;

void setup() {
  Serial.begin(9600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  // Wait for HX711 to be ready
  while (!scale.is_ready()) {
    Serial.println("HX711 not found. Check wiring...");
    delay(1000);
  }

  scale.set_scale(CALIBRATION_FACTOR);

  delay(500);             
  scale.tare(20);          // Average 20 readings for a stable zero
  Serial.println("Scale ready. Place weight...");
}

void loop() {
  if (scale.is_ready()) {
    float weight = scale.get_units(20); 
    if (abs(weight) < 0.01) weight = 0.0; // Deadband: ignore noise under 10g
    Serial.print("Weight: ");
    Serial.print(weight, 1);
    Serial.println(" kg");
  } else {
    Serial.println("HX711 not ready.");
  }
  delay(500);
}