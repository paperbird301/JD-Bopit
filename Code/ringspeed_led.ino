#include <Adafruit_NeoPixel.h>

#define STRIP_PIN 2
#define NUM_PIXELS 60

Adafruit_NeoPixel strip(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800);

uint8_t pos = 0;
unsigned long lastMove = 0;
int INTERVAL = 50;  // ms per step — tune this
int numpixels = 3;

int r = random(0, 256);
int g = random(0, 256);
int b = random(0, 256);

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void loop() {
  if (millis() - lastMove >= INTERVAL) {
    lastMove = millis();

    strip.clear();
    for (int i = 0; i < numpixels; i++) {
      strip.setPixelColor(pos + i, strip.Color(r, g, b));
    }

    strip.show();

    pos++;
    if (pos >= 60) {
      pos = 0;
      INTERVAL -= 5;
      if (INTERVAL == -5)
        INTERVAL = 50;
    }

    Serial.print("pos=");
    Serial.println(pos);
  }
}