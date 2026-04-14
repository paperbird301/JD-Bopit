#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "frames.h"

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

#define FRAME_DELAY 400
#define FRAME_WIDTH 64
#define FRAME_HEIGHT 64

int img = 0;  // 0=chalk, 1=rack, 2=stick, 3=win
unsigned long lastFrameTime = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//track game stuff
int score = 0;
int game_delay = 5000;
bool challenge_mode;  // maybe if time
bool start = false;

void setup() {
  Serial.begin(9600);
  delay(1000);

  execute_CMD(0x3F, 0, 0);  //init dfplayer
  delay(1000);
  setVolume(10);
  delay(500);
  execute_CMD(0x03, 0, 4);  //play backing track 4
  delay(2000);

  //oled
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;)
      ;
  }
  display.clearDisplay();
  display.display();

  //start button
  pinMode(A0, INPUT);
  pinMode(3, INPUT_PULLUP);

  //stick button
  pinMode(A1, INPUT);
  pinMode(4, INPUT_PULLUP);

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
}

void loop() {
  while (!start) {
    printCenter(1, "Press Start");
    if (digitalRead(3) == HIGH) {
      printCenter(2, "3");
      delay(700);
      printCenter(2, "2");
      delay(700);
      printCenter(2, "1");
      delay(700);
      lcd.clear();
      start = true;
      execute_CMD(0x0E, 0, 2);  //stop
    }
  }

  if (score >= 99) {
    winScreen();
    return;
  }

  game_delay = max(1000, 5000 - score * 30);  // currently set to 5 sec -> 2 sec

  int game = random(3);  //Game 3 (WIP)
  if (game == 0) {
    execute_CMD(0x03, 0, 1);  //play track 1
    runChalk();
    execute_CMD(0x0E, 0, 2);  //stop
  } else if (game == 1) {
    execute_CMD(0x03, 0, 2);  //play track 2
    runRack();
    execute_CMD(0x0E, 0, 2);  //stop
  } else {
    execute_CMD(0x03, 0, 3);  //play track 3
    runStick();
    execute_CMD(0x0E, 0, 2);  //stop
  }
}

void winScreen() {
  lcd.clear();
  printCenter(1, "YOU WIN!!!");
  String cur_score = String(score);
  printCenter(2, cur_score);
  // Halt forever
  while (true) {
    drawFrame(3);
  }
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