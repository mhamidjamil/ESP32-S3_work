// #include "Keyboard.h"

// declaring button pins
#include "USB.h"
#include "USBHIDKeyboard.h"
#include <WiFi.h>
#include <esp_now.h>
String username = "hamid";
USBHIDKeyboard Keyboard;

const int buttonPin = 2;

int previousButtonState = LOW;

void setup() {
  // declare the buttons as input_pullup
  pinMode(buttonPin, INPUT_PULLDOWN);
  Keyboard.begin();
  USB.begin();
}

void loop() {
  // checking the state of the button
  int buttonState = digitalRead(buttonPin);

  // replaces button press with UP arrow
  if (buttonState == HIGH && previousButtonState == LOW) {
    // and it's currently pressed:
    // Keyboard.press(97);
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(1000);

    // Keyboard.press(KEY_LEFT_GUI);
    // delay(500);
    // Keyboard.release(KEY_LEFT_GUI);
    // delay(1000);

    Keyboard.print("cmd");
    delay(500);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.release(KEY_RETURN);
    delay(1000);

    // Keyboard.print("cd C:\\Users\\" + username
    // +"\\AppData\\Roaming\\Microsoft\\Windows\\Start
    // ""Menu\\Programs\\Startup");

    //  %userprofile%\AppData\Roaming\Microsoft\Windows\Start
    //  Menu\Programs\Startup
    Keyboard.print(
        "cd %userprofile%\\AppData\\Roaming\\Microsoft\\Windows\\Start "
        "Menu\\Programs\\Startup");

    delay(500);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.release(KEY_RETURN);
    delay(2000);
    // // Keyboard.press(KEY_RETURN);
    // // delay(100);

    // Keyboard.println("cd C:\\Users\\Pavilion "
    //                "Gaming\\AppData\\Roaming\\Microsoft\\Windows\\Start "
    //                "Menu\\Programs\\Startup");
    // delay(100);

    // Keyboard.press(KEY_RETURN);
    // delay(100);
    // no press enter key
  }

  if (buttonState == LOW && previousButtonState == HIGH) {
    // and it's currently released:
    Keyboard.release(97);
    delay(50);
  }
  // Keyboard.releaseAll();
  previousButtonState = buttonState;
}