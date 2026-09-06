#include <Arduino.h>
void setup() {}
void loop(){ neopixelWrite(48,255,0,0); delay(700); neopixelWrite(48,0,255,0); delay(700); neopixelWrite(48,0,0,255); delay(700); neopixelWrite(48,0,0,0); delay(700); }
