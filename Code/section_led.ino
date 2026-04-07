#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define STRIP_PIN 2
#define BUTTON_PIN 3
#define NUM_PIXELS 32  // Two 16-pixel strips daisy-chained
#define CENTER_MIN 4
#define CENTER_MAX 8

Adafruit_NeoPixel strip(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 16, 2);

uint8_t pos = 0;
bool lastButtonState = HIGH;
unsigned long lastMove = 0;
unsigned long lastDebounce = 0;
bool buttonHandled = false;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.show();
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

      lcd.clear();
      lcd.setCursor(0, 0);
      if (pos >= CENTER_MIN && pos <= CENTER_MAX && buttonState==HIGH) {
        lcd.print("HIT!");
        delay(1000);
      } 
      else {
        lcd.print("MISS!");
      }
      buttonHandled = true;

  lastButtonState = buttonState;

  Serial.print("btn=");
  Serial.print(buttonState);
  Serial.print(" pos=");
  Serial.println(pos);

  // Move pixel every 300ms without blocking
  if (millis() - lastMove >= 400) {
    lastMove = millis();

    pos++;
    if (pos >= 16) {
      pos = 0;
    }

    strip.clear();
    strip.setPixelColor(pos, strip.Color(255, 0, 64));       // First strip
    strip.setPixelColor(pos + 16, strip.Color(255, 0, 64));  // Second strip (mirror)
    strip.show();
  }
}