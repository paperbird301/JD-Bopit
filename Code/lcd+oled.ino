#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "frames.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// I2C address 0x27, 16 columns, 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// global vars
int int_score = 0;
int frame = 0;
unsigned long lastFrameTime = 0;

#define FRAME_DELAY 1000
#define FRAME_WIDTH 64
#define FRAME_HEIGHT 64

void setup() {
  // lcd
  lcd.init();
  lcd.backlight();

  // oled
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;)
      ;  // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  unsigned long now = millis();

  // Update animation frame based on FRAME_DELAY
  if (now - lastFrameTime >= FRAME_DELAY) {
    lastFrameTime = now;
    drawFrame(frame);
    frame = (frame + 1) % FRAME_COUNT;
  }

  // Display score on LCD
  printCenter(1, "Score");
  String cur_score = String(int_score);
  printCenter(2, cur_score);
  int_score++;

  // win
  if (int_score >= 100) {
    lcd.clear();
    printCenter(1, "You Win!!!");
    while (true) {}
  }
}

void printCenter(int row, String text) {
  int col = (16 - text.length()) / 2;
  if (col < 0) col = 0;
  lcd.setCursor(col, row - 1);
  lcd.print(text);
}

void drawFrame(int f) {
  const unsigned char* frameData = (const unsigned char*)pgm_read_ptr(&frames[f]);
  display.clearDisplay();
  display.drawBitmap(32, 0, frameData, FRAME_WIDTH, FRAME_HEIGHT, WHITE);
  display.display();
}