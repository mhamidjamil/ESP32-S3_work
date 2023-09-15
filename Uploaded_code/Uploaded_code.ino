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
  html += "<button onclick='sendKey(\"\\x1b\")'>Esc</button>";
  html += "<button onclick='sendKey(\"\\x1b[1~\")'>Esc</button>";
  html += "<button onclick='sendKey(\"\\x1b[11~\")'>F1</button>";
  html += "<button onclick='sendKey(\"\\x1b[12~\")'>F2</button>";
  html += "<button onclick='sendKey(\"\\x1b[13~\")'>F3</button>";
  html += "<button onclick='sendKey(\"\\x1b[14~\")'>F4</button>";
  html += "<button onclick='sendKey(\"\\x1b[15~\")'>F5</button>";
  html += "<button onclick='sendKey(\"\\x1b[17~\")'>F6</button>";
  html += "<button onclick='sendKey(\"\\x1b[18~\")'>F7</button>";
  html += "<button onclick='sendKey(\"\\x1b[19~\")'>F8</button>";
  html += "<button onclick='sendKey(\"\\x1b[20~\")'>F9</button>";
  html += "<button onclick='sendKey(\"\\x1b[21~\")'>F10</button>";
  html += "<button onclick='sendKey(\"\\x1b[23~\")'>F11</button>";
  html += "<button onclick='sendKey(\"\\x1b[24~\")'>F12</button>";
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
  html += "<button onclick='sendKey(\"\\x08\")'>Backspace</button>";
  html += "<button onclick='sendKey(\"\\x7f\")'>Delete</button>";
  html += "</div>";
  html += "<div>";
  // next line
  html += "<button onclick='sendKey(\"\\t\")'>Tab</button>";
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
  html += "<button onclick='sendKey(\"\\x1b[H\")'>Home</button>";
  html += "<button onclick='sendKey(\"\\x1b[F\")'>End</button>";
  html += "<button onclick='sendKey(\"\\x1b[2~\")'>Insert</button>";
  html += "</div>";
  html += "<div>";
  // next line
  html += "<button onclick='sendKey(\"\\x1b[2~\")'>Caps Lock</button>";
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
  html += "<button onclick='sendKey(\"\\n\")'>Enter</button>";
  html += "<button onclick='sendKey(\"\\x1b[5~\")'>Page Up</button>";
  html += "<button onclick='sendKey(\"\\x1b[6~\")'>Page Down</button>";
  html += "</div>";
  html += "<div>";
  // next line
  html += "<button onclick='sendKey(\"\\x1b[2~\")'>Shift</button>";
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
  html += "<button onclick='sendKey(\"\\x1b[A\")'>Up</button>";
  html += "</div>";
  html += "<div>";
  html += "<button onclick='sendKey(\"\\x1b[2~\")'>Ctrl</button>";
  html += "<button onclick='sendKey(\"\\x1b[2~\")'>Win</button>";
  html += "<button onclick='sendKey(\"\\x1b[2~\")'>Alt</button>";
  html += "<button onclick='sendKey(\" \")'></button>";
  html += "<button onclick='sendKey(\" \")'>space</button>";
  html += "<button onclick='sendKey(\" \")'></button>";
  html += "<button onclick='sendKey(\" \")'></button>";
  html += "<button onclick='sendKey(\"\\x1b[C\")'>Right</button>";
  html += "<button onclick='sendKey(\"\\x1b[B\")'>Down</button>";
  html += "<button onclick='sendKey(\"\\x1b[D\")'>Left</button>";

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
  BLE_inputManager(key);
  Serial.println("Received Key: " + key);
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
    println("Sending: " + input);
    Keyboard.print(input);
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