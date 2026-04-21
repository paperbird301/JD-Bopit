extern int score;
extern int game_delay;
extern bool lost;
extern LiquidCrystal_I2C lcd;

void runChalk() {
  long target = random(180);

  lcd.clear();
  printCenter(1, "Turn chalk to:");
  printCenter(2, String(target));

  //while game delay play animation
  long deadline = millis() + game_delay;
  while (millis() < deadline) {
    drawFrame(0);
  }

  //after game delay
  int value = analogRead(A0);
  float degree = value * 180.0 / 1023.0;

  long lower_lim = (long)(degree - 10);
  long upper_lim = (long)(degree + 10);

  if (lower_lim <= target && target <= upper_lim) {
    score++;
    execute_CMD(0x03, 0, 7);  //play score+ audio
    lcd.clear();
    printCenter(1, "Score:");
    printCenter(2, String(score));
  } else {
    lcd.clear();
    printCenter(1, "Miss!");
    printCenter(2, "Score: " + String(score));
    lost=true;
  }

  //show result for 1 sec
  long showUntil = millis() + 1000;
  while (millis() < showUntil) {
    drawFrame(0);
  }
}