extern int score;
extern int game_delay;
extern LiquidCrystal_I2C lcd;

void runChalk() {

  long target = random(180);

  lcd.clear();
  printCenter(1, "Turn chalk to:");
  printCenter(2, String(target));

  delay(game_delay);

  int value = analogRead(A0);
  float degree = value * 180.0 / 1023.0;

  long lower_lim = (long)(degree - 10);
  long upper_lim = (long)(degree + 10);

  if (lower_lim <= target && target <= upper_lim) 
  {
    score++;
    lcd.clear();
    printCenter(1, "Score:");
    printCenter(2, String(score));
    Serial.print("Chalk hit | Score: ");
    Serial.println(score);
    delay(1000);
  } else 
  {
    lcd.clear();
    printCenter(1, "Miss!");
    printCenter(2, "Score: " + String(score));
    Serial.println("Chalk miss");
    delay(1000);
  }
}
