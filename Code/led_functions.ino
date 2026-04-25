#include <Adafruit_NeoPixel.h>

#define STRIP_PIN 2
#define NUM_PIXELS 55

Adafruit_NeoPixel strip(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800);

uint8_t pcenter = 0;

void setup() {
  Serial.begin(9600);
  strip.begin();
  pocket();
}

void loop() {
  gradient(pcenter);

}

void rainbow_swirl() {
  static uint16_t hue = 0;
  static unsigned long lastMove = 0;

  if (millis() - lastMove >= 15) {  //speed control
    lastMove = millis();
    strip.rainbow(hue, 3, 255, 120, true);
    strip.show();
    hue += 256;
  }
}
void lose_led() {
  static long unsigned lastMove = 0;
  static uint8_t brightness = 0;
  static int8_t direction = 1;  // 1 = fade in / -1 = fade out

  if (millis() - lastMove >= 15) {  //speed control
    lastMove = millis();

    brightness += direction;
    if (brightness >= 162)
      direction = -1;
    if (brightness <= 35)
      direction = 1;

    strip.setBrightness(brightness);
    for (int i = 0; i < NUM_PIXELS; i++) {
      strip.setPixelColor(i,255, 0, 0);
    }
    strip.show();
  }
}
void pocket() {
  uint8_t pocket = random(6);
  switch (pocket) {
    case 0:
      pcenter = 4;//mid left
      break;
    case 1:
      pcenter = 12;//top left
      break;
    case 2:
      pcenter = 20;//top right
      break;
    case 3:
      pcenter = 28;//mid right
      break;
    case 4:
      pcenter = 36;//bot right
      break;
    case 5:
      pcenter = 44;//bot left
      break;
  }
}
void gradient(uint8_t pcenter) {
  static unsigned long lastMove = 0;
  static uint8_t brightness = 0;
  static int8_t direction = 1;

  if (millis() - lastMove >= 5) {
    lastMove = millis();

    brightness += direction;
    if (brightness >= 200) direction = -1;
    if (brightness <= 20)  direction =  1;

    strip.clear();
    for (int i = 0; i < NUM_PIXELS; i++) {
      int dist = abs(i - pcenter);
      float falloff = 1.0 - (dist / 3.0); // 10 = how wide the gradient is
      if (falloff < 0) falloff = 0;

      uint8_t b = brightness * falloff;
      strip.setPixelColor(i, b, 0, b); 
    }
    strip.show();
  }
}