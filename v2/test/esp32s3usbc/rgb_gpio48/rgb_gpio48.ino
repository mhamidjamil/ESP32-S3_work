#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel led(1, 48, NEO_GRB + NEO_KHZ800);
void setup(){ led.begin(); led.setBrightness(32); }
void loop(){ led.setPixelColor(0, led.Color(255,0,0)); led.show(); delay(700); led.setPixelColor(0, led.Color(0,255,0)); led.show(); delay(700); led.setPixelColor(0, led.Color(0,0,255)); led.show(); delay(700); led.clear(); led.show(); delay(700); }
