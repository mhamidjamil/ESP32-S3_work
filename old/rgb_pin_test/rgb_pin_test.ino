/*
  ESP32-S3 dual-USB board RGB test

  Board target: ESP32-S3-WROOM-1 N8R2 / YD-ESP32-S3-style board
  The onboard WS2812/SK68XX RGB LED is normally on GPIO48.

  Serial monitor: 115200 baud
  Commands:
    help
    pin 48
    brightness 25
    pattern rainbow
    pattern solid
    pattern blink
    color 255 0 0
    off
*/

#include <Arduino.h>

// Change this if your board revision routes RGB to another GPIO.
uint8_t rgbPin = 48;
uint8_t brightness = 32;  // 0-255; 32 is intentionally not full brightness.

enum Pattern { SOLID, BLINK, RAINBOW };
Pattern pattern = RAINBOW;
uint8_t solidR = 255;
uint8_t solidG = 0;
uint8_t solidB = 0;

unsigned long lastFrame = 0;
uint16_t hue = 0;
bool blinkOn = true;

uint8_t scaled(uint8_t value) {
  return (uint16_t)value * brightness / 255;
}

void showRgb(uint8_t r, uint8_t g, uint8_t b) {
  // ESP32 Arduino's built-in NeoPixel driver sends the required WS2812 waveform.
  neopixelWrite(rgbPin, scaled(r), scaled(g), scaled(b));
}

void hsvToRgb(uint16_t h, uint8_t &r, uint8_t &g, uint8_t &b) {
  uint8_t region = h / 43;
  uint8_t remainder = (h - region * 43) * 6;
  uint8_t p = 0;
  uint8_t q = 255 - remainder;
  uint8_t t = remainder;

  switch (region) {
    case 0: r = 255; g = t; b = p; break;
    case 1: r = q; g = 255; b = p; break;
    case 2: r = p; g = 255; b = t; break;
    case 3: r = p; g = q; b = 255; break;
    case 4: r = t; g = p; b = 255; break;
    default: r = 255; g = p; b = q; break;
  }
}

void printHelp() {
  Serial.println();
  Serial.println("RGB test commands:");
  Serial.println("  pin <gpio>             e.g. pin 48");
  Serial.println("  brightness <0-255>    e.g. brightness 64");
  Serial.println("  pattern solid|blink|rainbow");
  Serial.println("  color <r> <g> <b>      e.g. color 255 0 0");
  Serial.println("  off");
  Serial.println("  help");
  Serial.println();
}

void handleCommand(String command) {
  command.trim();
  command.toLowerCase();
  if (command == "help") {
    printHelp();
  } else if (command == "off") {
    showRgb(0, 0, 0);
    pattern = SOLID;
    solidR = solidG = solidB = 0;
  } else if (command.startsWith("pin ")) {
    int value = command.substring(4).toInt();
    if (value >= 0 && value <= 48) {
      showRgb(0, 0, 0);
      rgbPin = value;
      Serial.printf("RGB data pin set to GPIO%d\n", rgbPin);
    }
  } else if (command.startsWith("brightness ")) {
    int value = command.substring(11).toInt();
    brightness = constrain(value, 0, 255);
    Serial.printf("Brightness set to %u/255\n", brightness);
  } else if (command == "pattern solid") {
    pattern = SOLID;
  } else if (command == "pattern blink") {
    pattern = BLINK;
  } else if (command == "pattern rainbow") {
    pattern = RAINBOW;
  } else if (command.startsWith("color ")) {
    int r, g, b;
    if (sscanf(command.c_str(), "color %d %d %d", &r, &g, &b) == 3) {
      solidR = constrain(r, 0, 255);
      solidG = constrain(g, 0, 255);
      solidB = constrain(b, 0, 255);
      pattern = SOLID;
      Serial.printf("Color set to %u, %u, %u\n", solidR, solidG, solidB);
    }
  } else {
    Serial.println("Unknown command. Type help.");
  }
}

void setup() {
  Serial.begin(115200);
  delay(300);
  showRgb(0, 0, 0);
  Serial.println("ESP32-S3 RGB test started");
  Serial.printf("GPIO: %u, brightness: %u/255\n", rgbPin, brightness);
  printHelp();
}

void loop() {
  if (Serial.available()) {
    handleCommand(Serial.readStringUntil('\n'));
  }

  unsigned long now = millis();
  if (now - lastFrame < 40) return;
  lastFrame = now;

  if (pattern == SOLID) {
    showRgb(solidR, solidG, solidB);
  } else if (pattern == BLINK) {
    blinkOn = !blinkOn;
    showRgb(blinkOn ? solidR : 0, blinkOn ? solidG : 0, blinkOn ? solidB : 0);
  } else {
    uint8_t r, g, b;
    hsvToRgb(hue, r, g, b);
    showRgb(r, g, b);
    hue += 2;
  }
}
