#include <Arduino.h>
void setup() {}
void loop(){ neopixelWrite(38,255,0,0); delay(700); neopixelWrite(38,0,255,0); delay(700); neopixelWrite(38,0,0,255); delay(700); neopixelWrite(38,0,0,0); delay(700); }
