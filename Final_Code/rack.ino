#include <NewPing.h>

extern int score;
extern int game_delay;
extern LiquidCrystal_I2C lcd;

#define TRIGGER_PIN   7
#define ECHO_PIN      8
#define MAX_DISTANCE  50 // cm

#define RACK_UP_CM    15.0  // lift threshold
#define RACK_DOWN_CM  5.0   // to avoid deadzone

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void runRack() {
  lcd.clear();
  printCenter(1, "Lift the rack!");
  printCenter(2, "");

  bool lifted  = false;
  bool lowered = false;

  // Phase 1: wait for rack to be lifted
  long deadline = millis() + game_delay;

  while (millis() < deadline) {
    float cm = sonar.ping_cm();

    Serial.print("Phase1 cm: ");
    Serial.println(cm);

    if (cm > RACK_UP_CM && cm != 0) {
      lifted = true;
      break;
    }
    delay(50); // wait for sensor
  }

  if (!lifted) 
  {
    lcd.clear();
    printCenter(1, "Too slow");
    printCenter(2, "Score: " + String(score));
    Serial.println("Rack miss - not lifted");
    delay(1000);
    return;
  }

  // Phase 2: wait for rack to be lowered
  lcd.clear();
  printCenter(1, "Now lower it!");
  printCenter(2, "");

  deadline = millis() + game_delay;

  while (millis() < deadline) 
  {
    float cm = sonar.ping_cm();

    Serial.print("Phase2 cm: ");
    Serial.println(cm);

    if (cm < RACK_DOWN_CM && cm != 0) 
    {
      lowered = true;
      break;
    }
    delay(50);
  }

  if (lowered) 
  {
    score++;
    lcd.clear();
    printCenter(1, "Score:");
    printCenter(2, String(score));
    Serial.print("Rack hit | Score: ");
    Serial.println(score);
    delay(1000);
  } 
  else 
  {
    lcd.clear();
    printCenter(1, "Too slow");
    printCenter(2, "Score: " + String(score));
    Serial.println("Rack miss - not lowered");
    delay(1000);
  }
}
