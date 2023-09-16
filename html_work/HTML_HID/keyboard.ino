//$ last work 16/Sep/23 [11:17 PM]
// # version 3.1.2
// # Almost all virtual keys are working now.

#include "arduino_secrets.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <WebServer.h>
#include <WiFi.h>

//+ HID part ...................... :
#include "USB.h"
#include "USBHIDKeyboard.h"
#include <esp_now.h>
String output = "";
USBHIDKeyboard Keyboard;
const int buttonPin = 0; // GPIO0 is the default boot button pin
int previousButtonState = LOW;
void println(String str);
void print(String str);

const char *ssid = MY_SSID;
const char *password = MY_PASSWORD;

WebServer server(80);

void BLE_inputManager(String input);

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>ESP32-S3 Virtual "
                "Keyboard</title></head><body>";
  html += "<h1>Virtual Keyboard</h1>";
  html += "<div id='keyboard'>";
  // Create a set of buttons for each key you want on the virtual keyboard
  // You can customize the buttons as needed
  html += "<button onclick='sendKey(\"esc\")'>Esc</button>";
  html += "<button onclick='sendKey(\"F1\")'>F1</button>";
  html += "<button onclick='sendKey(\"F2\")'>F2</button>";
  html += "<button onclick='sendKey(\"F3\")'>F3</button>";
  html += "<button onclick='sendKey(\"F4\")'>F4</button>";
  html += "<button onclick='sendKey(\"F5\")'>F5</button>";
  html += "<button onclick='sendKey(\"F6\")'>F6</button>";
  html += "<button onclick='sendKey(\"F7\")'>F7</button>";
  html += "<button onclick='sendKey(\"F8\")'>F8</button>";
  html += "<button onclick='sendKey(\"F9\")'>F9</button>";
  html += "<button onclick='sendKey(\"F10\")'>F10</button>";
  html += "<button onclick='sendKey(\"F11\")'>F11</button>";
  html += "<button onclick='sendKey(\"F12\")'>F12</button>";
  // next line
  html += "</div>";
  html += "<div>";
  html += "<button onclick='sendKey(\"1\")'>1</button>";
  html += "<button onclick='sendKey(\"2\")'>2</button>";
  html += "<button onclick='sendKey(\"3\")'>3</button>";
  html += "<button onclick='sendKey(\"4\")'>4</button>";
  html += "<button onclick='sendKey(\"5\")'>5</button>";
  html += "<button onclick='sendKey(\"6\")'>6</button>";
  html += "<button onclick='sendKey(\"7\")'>7</button>";
  html += "<button onclick='sendKey(\"8\")'>8</button>";
  html += "<button onclick='sendKey(\"9\")'>9</button>";
  html += "<button onclick='sendKey(\"0\")'>0</button>";
  html += "<button onclick='sendKey(\"-\")'>-</button>";
  html += "<button onclick='sendKey(\"=\")'>=</button>";
  html += "<button onclick='sendKey(\"back\")'>Backspace</button>";
  html += "<button onclick='sendKey(\"delete\")'>Delete</button>";
  html += "</div>";
  html += "<div>";
  // next line
  html += "<button onclick='sendKey(\"tab\")'>Tab</button>";
  html += "<button onclick='sendKey(\"q\")'>q</button>";
  html += "<button onclick='sendKey(\"w\")'>w</button>";
  html += "<button onclick='sendKey(\"e\")'>e</button>";
  html += "<button onclick='sendKey(\"r\")'>r</button>";
  html += "<button onclick='sendKey(\"t\")'>t</button>";
  html += "<button onclick='sendKey(\"y\")'>y</button>";
  html += "<button onclick='sendKey(\"u\")'>u</button>";
  html += "<button onclick='sendKey(\"i\")'>i</button>";
  html += "<button onclick='sendKey(\"o\")'>o</button>";
  html += "<button onclick='sendKey(\"p\")'>p</button>";
  html += "<button onclick='sendKey(\"[\")'>[</button>";
  html += "<button onclick='sendKey(\"]\")'>]</button>";
  html += "<button onclick='sendKey(\"home\")'>Home</button>";
  html += "<button onclick='sendKey(\"end\")'>End</button>";
  html += "<button onclick='sendKey(\"insert\")'>Insert</button>";
  html += "</div>";
  html += "<div>";
  // next line
  html += "<button onclick='sendKey(\"capsLock\")'>Caps Lock</button>";
  html += "<button onclick='sendKey(\"a\")'>a</button>";
  html += "<button onclick='sendKey(\"s\")'>s</button>";
  html += "<button onclick='sendKey(\"d\")'>d</button>";
  html += "<button onclick='sendKey(\"f\")'>f</button>";
  html += "<button onclick='sendKey(\"g\")'>g</button>";
  html += "<button onclick='sendKey(\"h\")'>h</button>";
  html += "<button onclick='sendKey(\"j\")'>j</button>";
  html += "<button onclick='sendKey(\"k\")'>k</button>";
  html += "<button onclick='sendKey(\"l\")'>l</button>";
  html += "<button onclick='sendKey(\";\")'>;</button>";
  html += "<button onclick='sendKey(\"'\")'>\'</button>";
  html += "<button onclick='sendKey(\"enter\")'>Enter</button>";
  html += "<button onclick='sendKey(\"pageUp\")'>Page Up</button>";
  html += "<button onclick='sendKey(\"pageDown\")'>Page Down</button>";
  html += "</div>";
  html += "<div>";
  // next line
  html += "<button onclick='sendKey(\"shift\")'>Shift</button>";
  html += "<button onclick='sendKey(\"z\")'>z</button>";
  html += "<button onclick='sendKey(\"x\")'>x</button>";
  html += "<button onclick='sendKey(\"c\")'>c</button>";
  html += "<button onclick='sendKey(\"v\")'>v</button>";
  html += "<button onclick='sendKey(\"b\")'>b</button>";
  html += "<button onclick='sendKey(\"n\")'>n</button>";
  html += "<button onclick='sendKey(\"m\")'>m</button>";
  html += "<button onclick='sendKey(\",\")'>,</button>";
  html += "<button onclick='sendKey(\".\")'>.</button>";
  html += "<button onclick='sendKey(\"/\")'>/</button>";
  html += "<button onclick='sendKey(\"upArrow\")'>Up</button>";
  html += "</div>";
  html += "<div>";
  html += "<button onclick='sendKey(\"ctrl\")'>Ctrl</button>";
  html += "<button onclick='sendKey(\"win\")'>Win</button>";
  html += "<button onclick='sendKey(\"alt\")'>Alt</button>";
  html += "<button onclick='sendKey(\" \")'></button>";
  html += "<button onclick='sendKey(\"spaceBar\")'>space</button>";
  html += "<button onclick='sendKey(\" \")'></button>";
  html += "<button onclick='sendKey(\" \")'></button>";
  html += "<button onclick='sendKey(\"leftArrow\")'>Left</button>";
  html += "<button onclick='sendKey(\"downArrow\")'>Down</button>";
  html += "<button onclick='sendKey(\"rightArrow\")'>Right</button>";

  // Add more buttons for other keys
  html += "</div>";
  html += "</body>";
  html +=
      "<script>function sendKey(key) { fetch('/key?key=' + key); }</script>";
  html += "</html>";
  server.send(200, "text/html", html);
}

void handleKey() {
  String key = server.arg("key");
  // Process the received key (e.g., send it to your ESP32-S3 for keyboard
  // input)
  // ...
  Serial.println("Received Key: " + key);
  BLE_inputManager(key);
  server.send(200, "text/plain", "Key received");
}

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
std::string receivedData = "";

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) { deviceConnected = true; }

  void onDisconnect(BLEServer *pServer) { deviceConnected = false; }
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      receivedData = value;
      print("Received from BLE: ");
      Serial.println(receivedData.c_str());
      BLE_inputManager(String(receivedData.c_str()));

      // Print the received data back to the BLE connection
      receivedData = output.c_str();
      pCharacteristic->setValue("BLE: " + receivedData);
      pCharacteristic->notify();
    }
  }
};

void setup() {
  Serial.begin(115200);

  //~--------------------------------------------
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

  println("Waiting for Bluetooth connection...");
  //~--------------------------------------------

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/", handleRoot);
  server.on("/key", HTTP_GET, handleKey);

  server.begin();
  Serial.println("Web server started");
}

void loop() { //+.................................
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
      println("Connected to device");
      oldDeviceConnected = true;
    }
  } else {
    if (oldDeviceConnected) {
      println("Disconnected from device");
      pServer->startAdvertising(); // restart advertising
      println("Restart advertising");
      oldDeviceConnected = false;
    }
  }

  delay(100);
  server.handleClient();
}

void BLE_inputManager(String input) {
  if (input == "test") {
    println("defined word: test");
  } else if (input.startsWith("#run_")) {
    String command = input.substring(5);
    runCommand(command);
  } else if (input.startsWith("#cmd_")) {
    String command = input.substring(5);
    cmdCommand(command);
  } else {
    println("in else part now");
    if (input == "F1") {
      println("F1 key pressed");
      Keyboard.press(KEY_F1);   // Press F1 key
      Keyboard.release(KEY_F1); // Release F1 key
    } else if (input == "F2") {
      println("F2 key pressed");
      Keyboard.press(KEY_F2);   // Press F2 key
      Keyboard.release(KEY_F2); // Release F2 key
    } else if (input == "F3") {
      println("F3 key pressed");
      Keyboard.press(KEY_F3);   // Press F3 key
      Keyboard.release(KEY_F3); // Release F3 key
    } else if (input == "F4") {
      println("F4 key pressed");
      Keyboard.press(KEY_F4);   // Press F4 key
      Keyboard.release(KEY_F4); // Release F4 key
    } else if (input == "F5") {
      println("F5 key pressed");
      Keyboard.press(KEY_F5);   // Press F5 key
      Keyboard.release(KEY_F5); // Release F5 key
    } else if (input == "F6") {
      println("F6 key pressed");
      Keyboard.press(KEY_F6);   // Press F6 key
      Keyboard.release(KEY_F6); // Release F6 key
    } else if (input == "F7") {
      println("F7 key pressed");
      Keyboard.press(KEY_F7);   // Press F7 key
      Keyboard.release(KEY_F7); // Release F7 key
    } else if (input == "F8") {
      println("F8 key pressed");
      Keyboard.press(KEY_F8);   // Press F8 key
      Keyboard.release(KEY_F8); // Release F8 key
    } else if (input == "F9") {
      println("F9 key pressed");
      Keyboard.press(KEY_F9);   // Press F9 key
      Keyboard.release(KEY_F9); // Release F9 key
    } else if (input == "F10") {
      println("F10 key pressed");
      Keyboard.press(KEY_F10);   // Press F10 key
      Keyboard.release(KEY_F10); // Release F10 key
    } else if (input == "F11") {
      println("F11 key pressed");
      Keyboard.press(KEY_F11);   // Press F11 key
      Keyboard.release(KEY_F11); // Release F11 key
    } else if (input == "F12") {
      println("F12 key pressed");
      Keyboard.press(KEY_F12);   // Press F12 key
      Keyboard.release(KEY_F12); // Release F12 key
    } else if (input == "esc") {
      println("Esc key pressed");
      Keyboard.press(KEY_ESC);
    } else if (input == "upArrow") {
      println("Up Arrow key pressed");
      Keyboard.press(KEY_UP_ARROW);   // Press Up Arrow key
      Keyboard.release(KEY_UP_ARROW); // Release Up Arrow key
    } else if (input == "downArrow") {
      println("Down Arrow key pressed");
      Keyboard.press(KEY_DOWN_ARROW);   // Press Down Arrow key
      Keyboard.release(KEY_DOWN_ARROW); // Release Down Arrow key
    } else if (input == "rightArrow") {
      println("Right Arrow key pressed");
      Keyboard.press(KEY_RIGHT_ARROW);   // Press Right Arrow key
      Keyboard.release(KEY_RIGHT_ARROW); // Release Right Arrow key
    } else if (input == "leftArrow") {
      println("Left Arrow key pressed");
      Keyboard.press(KEY_LEFT_ARROW);   // Press Left Arrow key
      Keyboard.release(KEY_LEFT_ARROW); // Release Left Arrow key
    } else if (input == "home") {
      println("Home key pressed");
      Keyboard.press(KEY_HOME);   // Press Home key
      Keyboard.release(KEY_HOME); // Release Home key
    } else if (input == "end") {
      println("End key pressed");
      Keyboard.press(KEY_END);   // Press End key
      Keyboard.release(KEY_END); // Release End key
    } else if (input == "insert") {
      println("Insert key pressed");
      Keyboard.press(KEY_INSERT);   // Press Insert key
      Keyboard.release(KEY_INSERT); // Release Insert key
    } else if (input == "pageUp") {
      println("Page Up key pressed");
      Keyboard.press(KEY_PAGE_UP);   // Press Page Up key
      Keyboard.release(KEY_PAGE_UP); // Release Page Up key
    } else if (input == "pageDown") {
      println("Page Down key pressed");
      Keyboard.press(KEY_PAGE_DOWN);   // Press Page Down key
      Keyboard.release(KEY_PAGE_DOWN); // Release Page Down key
    } else if (input == "back") {
      println("Backspace key pressed");
      Keyboard.press(KEY_BACKSPACE);   // Press Backspace key
      Keyboard.release(KEY_BACKSPACE); // Release Backspace key
    } else if (input == "delete") {
      println("Delete key pressed");
      Keyboard.press(KEY_DELETE);   // Press Delete key
      Keyboard.release(KEY_DELETE); // Release Delete key
    } else if (input == "tab") {
      println("Tab key pressed");
      Keyboard.press(KEY_TAB);   // Press Tab key
      Keyboard.release(KEY_TAB); // Release Tab key
    } else if (input == "enter") {
      println("Enter key pressed");
      Keyboard.press(KEY_RETURN); // Press Enter key
      Keyboard.release(KEY_RETURN);
    } else if (input == "spaceBar") {
      println("Space key pressed");
      Keyboard.press(' ');   // Press Space key
      Keyboard.release(' '); // Release Space key
    } else if (input == "capsLock") {
      println("Caps Lock key pressed");
      Keyboard.press(KEY_CAPS_LOCK);   // Press Caps Lock key
      Keyboard.release(KEY_CAPS_LOCK); // Release Caps Lock key
    } else if (input == "shift") {
      println("Shift key pressed");
      Keyboard.press(KEY_LEFT_SHIFT);   // Press Shift key
      Keyboard.release(KEY_LEFT_SHIFT); // Release Shift key
    } else if (input == "ctrl") {
      println("Ctrl key pressed");
      Keyboard.press(KEY_LEFT_CTRL);   // Press Ctrl key
      Keyboard.release(KEY_LEFT_CTRL); // Release Ctrl key
    } else if (input == "alt") {
      println("Alt key pressed");
      Keyboard.press(KEY_LEFT_ALT);   // Press Alt key
      Keyboard.release(KEY_LEFT_ALT); // Release Alt key
    } else if (input == "win") {
      println("Win key pressed");
      Keyboard.press(KEY_LEFT_GUI);   // Press Win key
      Keyboard.release(KEY_LEFT_GUI); // Release Win key
    } else {
      println("Sending: " + input);
      Keyboard.print(input);
    }
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

void println(String str) {
  Serial.println(str);
  output = str;
}

void print(String str) {
  Serial.print(str);
  output = str;
}