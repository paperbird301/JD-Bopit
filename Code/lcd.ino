#include <LiquidCrystal_I2C.h>
#include  <Wire.h>

//the first parameter is  the I2C address
//the second parameter is how many rows are on your screen
//the  third parameter is how many columns are on your screen
LiquidCrystal_I2C lcd(0x27,  16, 2);

void printCenter(int, String);
int int_score=0;
void setup() {
  lcd.init();
  lcd.backlight();
}
void loop() {
  delay(100);
  // tell the screen to write on the top row
  printCenter(1, "Score");
  String cur_score=String(int_score);
  int_score++;
  printCenter(2, cur_score);

  //win
  if(int_score==100)
  {
    printCenter(1, "You Win!!!");
    while(true)
    {}
  }
  
}

void printCenter(int row, String text) {
  int col = (16 - text.length()) / 2;  // 16 is the LCD width
  if (col < 0) col = 0;                // Avoid negative index if text is too long
  lcd.setCursor(col, row - 1);         // row-1 because LCD rows start at 0
  lcd.print(text);
}