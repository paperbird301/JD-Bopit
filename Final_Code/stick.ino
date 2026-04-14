extern int score;
extern int game_delay;
extern LiquidCrystal_I2C lcd;

bool passed = false;

void runStick() {
  long deadline = millis() + game_delay;
  bool pulled = false;
  bool passed = false;

  lcd.clear();
  printCenter(1, "Pull & release");
  printCenter(2, "the stick!");

  while (millis() < deadline) {
    drawFrame(2);

    //wait for pull
    if (!pulled && digitalRead(4) == LOW) {
      pulled = true;
    }

    //wait for release
    if (pulled && digitalRead(4) == HIGH) {
      score++;
      passed = true;
      break;
    }
  }

  lcd.clear();
  if (passed) {
    printCenter(1, "Score:");
    printCenter(2, String(score));
  } else {
    printCenter(1, "Too slow");
    printCenter(2, "Score: " + String(score));
  }

  long showUntil = millis() + 1000;
  while (millis() < showUntil) {
    drawFrame(2);
  }
}