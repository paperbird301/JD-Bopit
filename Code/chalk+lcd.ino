#include <LiquidCrystal_I2C.h>
#include  <Wire.h>

LiquidCrystal_I2C lcd(0x27,  16, 2);

int score=0;
int chalk_delay=0;

void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  //rand algo from claude
  long seed = 0;
  for (int i = 0; i < 32; i++) {
    seed ^= (long)analogRead(A1) << (i % 16);
    delay(1);
  }
  randomSeed(seed);

  lcd.init();
  lcd.backlight();
}

void loop() {
  //change final number to change timeout delay
  chalk_delay=5000-score*30;
  long target=random(180.0);
  // Serial.print("Rand = ");
  // Serial.println(target);

  lcd.clear();
  String cur_target = String(target);
  printCenter(1, "Turn chalk to:");
  printCenter(2, cur_target);

  delay(chalk_delay);

  int value = analogRead(A0);
  float degree = value*180.0/1023.0;
  // Serial.print("Degrees = ");
  // Serial.println(degree);

  //leeway 
  long lower_lim=degree-10;
  long upper_lim=degree+10;

  if(lower_lim<=target&&target<=upper_lim)
  {
    lcd.clear();
    printScore();
    Serial.println("Chalk hit");
  }
  else
  {
    lcd.clear();
    printCenter(1, "Miss");
    Serial.println("Chalk miss");
  }
}

void printScore() {
  printCenter(1, "Score");
  score+=1;
  String cur_score = String(score);
  printCenter(2, cur_score);
  delay(1000);
}
void printCenter(int row, String text) {
  int col = (16 - text.length()) / 2;  
  if (col < 0) col = 0;               
  lcd.setCursor(col, row - 1);      
  lcd.print(text);
}