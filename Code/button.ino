#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Common I2C address: 0x27 (use 0x3F if that doesn't work)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonPin = 3;
int lastState = -1;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Button Status:");
}

void loop() {
  int currentState = digitalRead(buttonPin);

  if (currentState != lastState) {
    lcd.setCursor(0, 1);
    lcd.print("                "); // clear second line
    lcd.setCursor(0, 1);

    if (currentState == LOW) {
      lcd.print("Pressed");
    } else {
      lcd.print("Not Pressed");
    }

    lastState = currentState;
  }

  delay(50);
}