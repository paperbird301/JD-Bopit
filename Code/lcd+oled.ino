#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "triangle_pulse.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3c

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
//the first parameter is  the I2C address
//the second parameter is how many rows are on your screen
//the  third parameter is how many columns are on your screen
LiquidCrystal_I2C lcd(0x27,  16, 2);

//gloabl var
int int_score=0;
int frame;
unsigned long lastFrameTime = 0;

#define FRAME_DELAY 100 
#define FRAME_WIDTH (64)
#define FRAME_HEIGHT (64)
void setup() 
{
  //lcd
  lcd.init();
  lcd.backlight();

  //oled
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.display();
  display.clearDisplay();
}
void loop() 
{
  unsigned long now = millis();

  // Update animation frame based on FRAME_DELAY
  if (now - lastFrameTime >= FRAME_DELAY) {
    lastFrameTime = now;
    drawFrame(frame);
    frame = (frame + 1) % FRAME_COUNT;
  }
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
void drawFrame(int f) {
  // pgm_read_ptr is required because the frames[] pointer array itself is in PROGMEM
  const unsigned char* frameData = (const unsigned char*)pgm_read_ptr(&frames[f]);
  display.clearDisplay();
  display.drawBitmap(32, 0, frameData, FRAME_WIDTH, FRAME_HEIGHT, WHITE);
  display.display();
}