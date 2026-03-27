#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

const int triggerPin = 7;
const int echoPin = 8;
const int maxDistance = 50;

NewPing sonar(triggerPin, echoPin, maxDistance);
LiquidCrystal_I2C lcd(0x27, 16, 2);

float cm;
float inches;
int score;
bool locked = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("-> GAME START <-");
  delay(1000);
  lcd.clear();
}

void loop() {
  unsigned int duration = sonar.ping();

  cm = sonar.convert_cm(duration);
  inches = cm / 2.54;

  Serial.print("Duration: ");
  Serial.print(duration);
  Serial.print(" us  |  Distance: ");
  Serial.print(cm, 1);
  Serial.println(" cm");

  if (cm < 5.0 && !locked && duration != 0) {
    // Object just came close — score and lock
    locked = true;
    score++;
    lcd.clear();
    printScore();
  } 
  else if (cm >= 5.0 && locked) {
    // Object moved away — unlock so next pass can score
    locked = false;
    printScore();  // keep score visible after unlock
  }
delay(100);
}
void printScore() {
  printCenter(1, "Score");
  String cur_score = String(score);
  printCenter(2, cur_score);
}
void printCenter(int row, String text) {
  int col = (16 - text.length()) / 2;  // 16 is the LCD width
  if (col < 0) col = 0;                // Avoid negative index if text is too long
  lcd.setCursor(col, row - 1);         // row-1 because LCD rows start at 0
  lcd.print(text);
}