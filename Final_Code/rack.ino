#include <NewPing.h>

extern int score;
extern int game_delay;
extern LiquidCrystal_I2C lcd;

#define TRIGGER_PIN 15
#define ECHO_PIN 0
#define MAX_DISTANCE 20.0 

#define RACK_UP_CM 12.0   // lift threshold
#define RACK_DOWN_CM 4.0  // lower threshold

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

    float cm = sonar.ping_median(10); // average of 5 pings
    cm = sonar.convert_cm(cm);
    if (cm > RACK_UP_CM && cm <= MAX_DISTANCE) {
      lifted = true;
      break;
    }
  }

  if (!lifted) {
    lcd.clear();
    printCenter(1, "Too slow");
    printCenter(2, "Score: " + String(score));
    lost=true;
    return;
  }

  // Lower Phase
  lcd.clear();
  printCenter(1, "Now lower it!");
  deadline = millis() + game_delay;

  while (millis() < deadline) {
    drawFrame(1);

    float cm = sonar.ping_median(10); // average of 5 pings
    cm = sonar.convert_cm(cm);
    if (cm < RACK_DOWN_CM && cm != 0) {
      lowered = true;
      break;
    }
  }

  if (lifted&&lowered) {
    score++;
    execute_CMD(0x03, 0, 7);  //play score+ audio
    lcd.clear();
    printCenter(1, "Score:");
    printCenter(2, String(score));
  } else {
    lcd.clear();
    printCenter(1, "Too slow");
    printCenter(2, "Score: " + String(score));
    lost=true;
  }

  long showUntil = millis() + 1000;
  while (millis() < showUntil) {
    drawFrame(1);
  }
}