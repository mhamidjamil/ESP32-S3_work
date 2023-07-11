//$ 11/JULY/2023 10:45 PM
//* Code to test the functionality of LCD using i2c protocol with ESP32-S3
/*
  PIN
SDA	SCL
21	19
25	24
32	33
12	13
*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  // Initialize the I2C interface with your desired SDA and SCL pins
  Wire.begin(21, 19);

  // Initialize the LCD display
  lcd.begin(LCD_COLUMNS, LCD_ROWS);

  // Turn on the backlight
  lcd.setBacklight(HIGH);

  // Print a message on the LCD
  lcd.print("Hello, ESP32-S3!");
}

void loop() {
  // No additional functionality in the loop
}
