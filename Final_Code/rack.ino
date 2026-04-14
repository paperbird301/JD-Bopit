#include <NewPing.h>

extern int score;
extern int game_delay;
extern LiquidCrystal_I2C lcd;

#define TRIGGER_PIN 1
#define ECHO_PIN 0
#define MAX_DISTANCE 50 

#define RACK_UP_CM 10.0   // lift threshold
#define RACK_DOWN_CM 5.0  // to avoid deadzone

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void runRack() {
  lcd.clear();
  printCenter(1, "Lift the rack!");

  bool lifted = false;
  bool lowered = false;
  long deadline = millis() + game_delay;

  // Lift Phase
  while (millis() < deadline) {
    drawFrame(1);

    float cm = sonar.ping_cm();
    if (cm > RACK_UP_CM && cm != 0) {
      lifted = true;
      break;
    }
  }

  if (!lifted) {
    lcd.clear();
    printCenter(1, "Too slow");
    printCenter(2, "Score: " + String(score));
    delay(1000);
    return;
  }

  // Lower Phase
  lcd.clear();
  printCenter(1, "Now lower it!");
  deadline = millis() + game_delay;

  while (millis() < deadline) {
    drawFrame(1);

    float cm = sonar.ping_cm();
    if (cm < RACK_DOWN_CM && cm != 0) {
      lowered = true;
      break;
    }
  }

  if (lowered) {
    score++;
    lcd.clear();
    printCenter(1, "Score:");
    printCenter(2, String(score));
  } else {
    lcd.clear();
    printCenter(1, "Too slow");
    printCenter(2, "Score: " + String(score));
  }

  long showUntil = millis() + 1000;
  while (millis() < showUntil) {
    drawFrame(1);
  }
}