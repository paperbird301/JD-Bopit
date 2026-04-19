#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "frames.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int frame = 0;
int sequence = 0; // 0=rack, 1=stick, 2=win
unsigned long lastFrameTime = 0;

#define FRAME_DELAY 1000
#define FRAME_WIDTH 64
#define FRAME_HEIGHT 64

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  unsigned long now = millis();

  if (now - lastFrameTime >= FRAME_DELAY) {
    lastFrameTime = now;
    drawFrame(frame, sequence);

    frame++;
    if (frame >= FRAME_COUNT) {
      frame = 0;
      sequence++;
      if (sequence > 2) sequence = 0;
    }
  }
}

void drawFrame(int f, int seq) {
  const unsigned char* frameData;
  if (seq == 0)
    frameData = (const unsigned char*)pgm_read_ptr(&rack_frames[f]);
  else if (seq == 1)
    frameData = (const unsigned char*)pgm_read_ptr(&stick_frames[f]);
  else
    frameData = (const unsigned char*)pgm_read_ptr(&win_frames[f]);

  display.clearDisplay();
  display.drawBitmap(32, 0, frameData, FRAME_WIDTH, FRAME_HEIGHT, WHITE);
  display.display();
}