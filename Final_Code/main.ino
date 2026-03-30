#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int score = 0;
int game_delay = 5000;
bool challenge_mode; // maybe if time

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);

  // Entropy-based random seed
  long seed = 0;
  for (int i = 0; i < 32; i++) {
    seed ^= (long)analogRead(A1) << (i % 16);
    delay(1);
  }
  randomSeed(seed);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  //wait for button press (WIP)
  delay(2000);
}

void loop() {
  if (score >= 99) 
  {
    winScreen();
    return;
  }

  game_delay = max(1000, 5000 - score * 30); // currently set to 5 sec -> 2 sec

  int game = random(2);// speaker (WIP) & Game 3 (WIP) 
  if (game == 0) 
  {
    runChalk();
  } 
  else 
  {
    runRack();
  }
}

void winScreen() {
  lcd.clear();
  printCenter(1, "YOU WIN!!!");
  String cur_score = String(score);
  printCenter(2, cur_score);
  // Halt forever
  while (true) {}
}

void printCenter(int row, String text) {
  int col = (16 - (int)text.length()) / 2;
  if (col < 0) col = 0;
  lcd.setCursor(col, row - 1);
  lcd.print(text);
}
