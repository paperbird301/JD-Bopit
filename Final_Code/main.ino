#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "frames.h"
#include <Adafruit_NeoPixel.h>

//audio
#define Start_Byte 0x7E
#define Version_Byte 0xFF
#define Command_Length 0x06
#define End_Byte 0xEF
#define Acknowledge 0x00

//oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define FRAME_DELAY 200
#define FRAME_WIDTH 64
#define FRAME_HEIGHT 64

//strip leds
#define STRIP_PIN 1
#define NUM_PIXELS 55

int img = 0;  // 0=chalk, 1=rack, 2=stick, 3=win, 4=lsoe
unsigned long lastFrameTime = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_NeoPixel strip(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800);

//track game stuff
int score = 0;
int game_delay = 5000;
bool start = false;
bool lost = false;
 
void setup() {
  Serial.begin(9600);
  delay(1000);

  execute_CMD(0x3F, 0, 0);  //init dfplayer
  delay(1000);
  setVolume(10);
  delay(500);
  execute_CMD(0x08, 0, 2);  //loop backing track 4
  delay(1000);

  //oled
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;)
      ;
  }
  display.clearDisplay();
  display.display();

  //led strip
  strip.begin();
  strip.show();

  //start button
  pinMode(A0, INPUT);
  pinMode(12, INPUT_PULLUP);

  //stick button
  pinMode(A1, INPUT);
  pinMode(13, INPUT_PULLUP);

  // Entropy-based random seed from claude
  long seed = 0;
  for (int i = 0; i < 32; i++) {
    seed ^= (long)analogRead(A5) << (i % 16);
    delay(1);
  }
  randomSeed(seed);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  printCenter(1, "Press Start");
}

void loop() {
  while (!start) {
    rainbow_swirl();
    if (digitalRead(12) == LOW) {
      printCenter(2, "3");
      delay(700);
      printCenter(2, "2");
      delay(700);
      printCenter(2, "1");
      delay(700);
      lcd.clear();
      strip.clear();
      strip.show();
      start = true;
    }
  }

  strip.clear();
  strip.show();

  if (score >= 99) {
    winScreen();
    return;
  }

  game_delay = max(1000, 5000 - score * 30);  // currently set to 5 sec -> 2 sec

  int game = random(3);
  losecheck();
  if (game == 0) {
    execute_CMD(0x03, 0, 3);  //play chalk it
    runChalk();
  } else if (game == 1) {
    execute_CMD(0x03, 0, 5);  //play rack it
    runRack();
  } else {
    execute_CMD(0x03, 0, 4);  //play pull it
    runStick();
  }
}

void winScreen() {
  lcd.clear();
  strip.clear();
  strip.show();
  execute_CMD(0x03, 0, 1);  //play yippie
  printCenter(1, "YOU WIN!!!");
  String cur_score = "Score: " + String(score);
  printCenter(2, cur_score);
  
  delay(5000);
  execute_CMD(0x08, 0, 2);  //loop backing track 4
  lcd.clear();
  printCenter(1, "Press START");
  printCenter(2, "to reset");
  win_led();
  // Halt forever
  while (true) {
    drawFrame(3);
    if (digitalRead(12) == LOW) 
      softReset();
  }
}

void losecheck() {
  if (lost == true) {
    lcd.clear();
    strip.clear();
    strip.show();
    execute_CMD(0x03, 0, 6);  //play lose audio
    printCenter(1, "GAME OVER");
    String cur_score = "Score: " + String(score);
    printCenter(2, cur_score);
    
    delay(5000);
    execute_CMD(0x08, 0, 2);  //loop backing track 4
    lcd.clear();
    printCenter(1, "Press START");
    printCenter(2, "to try again");
    lose_led();
    // Halt forever
    while (true) {
      drawLose();
      if (digitalRead(12) == LOW) 
        softReset();
    }
  }
}

void softReset() {
  strip.clear();
  strip.show();
  display.clearDisplay();
  display.display();
  delay(500);             // let everything settle before jumping
  void (*resetFunc)(void) = 0;
  resetFunc();
}

void printCenter(int row, String text) {
  int col = (16 - (int)text.length()) / 2;
  if (col < 0) col = 0;
  lcd.setCursor(col, row - 1);
  lcd.print(text);
}

void setVolume(int volume) {
  execute_CMD(0x06, 0, volume);
}

void execute_CMD(byte CMD, byte Par1, byte Par2) {
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);

  byte Command_line[10] = {
    Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
    Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte
  };

  for (byte k = 0; k < 10; k++) {
    Serial.write(Command_line[k]);
  }
}

bool drawFrame(int img) {
  static int frame = 0;
  static int lastImg = -1;
  static unsigned long lastFrameTime = 0;

  if (img != lastImg) {
    frame = 0;
    lastImg = img;
  }

  unsigned long now = millis();
  if (now - lastFrameTime < FRAME_DELAY) return false;
  lastFrameTime = now;

  const unsigned char** frames;
  if (img == 0) frames = chalk_frames;
  else if (img == 1) frames = rack_frames;
  else if (img == 2) frames = stick_frames;
  else frames = win_frames;

  display.clearDisplay();
  display.drawBitmap(32, 0, (const unsigned char*)pgm_read_ptr(&frames[frame]), FRAME_WIDTH, FRAME_HEIGHT, WHITE);
  display.display();

  frame++;
  if (frame >= 4) {
    frame = 0;
    return true;
  }
  return false;
}
bool drawLose() {
  static int frame = 0;
  static int lastImg = -1;
  static unsigned long lastFrameTime = 0;

  if (img != lastImg) {
    frame = 0;
    lastImg = img;
  }

  unsigned long now = millis();
  if (now - lastFrameTime < FRAME_DELAY) return false;
  lastFrameTime = now;

  display.clearDisplay();
  display.drawBitmap(32, 0, (const unsigned char*)pgm_read_ptr(&lose_frames[frame]), FRAME_WIDTH, FRAME_HEIGHT, WHITE);
  display.display();

  frame++;
  if (frame >= 6) {
    frame = 0;
    return true;
  }
  return false;
}
void rainbow_swirl() {
  strip.rainbow(0, 3, 255, 120, true);
  strip.show();
}
void lose_led() {
  strip.setBrightness(100);
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, 255, 0, 0);
  }
  strip.show();
}

void win_led() {
  strip.setBrightness(100);
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, 0, 255, 0);
  }
  strip.show();
}