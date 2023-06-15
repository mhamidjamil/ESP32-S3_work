//$ 1:22 AM 11/JUNE/2023

//` this code will get input form web browser and use it with HID protocol

//! BLE work is still in pending state from BLE branch. we can't send a response
//! to BLE connected device

//+ HTML part ..................... +:
#include <WebServer.h>
#include <WiFi.h>
const char *ssid = "Archer 73";
const char *password = "Archer@73_102#";

WebServer server(80);
//+ ............................... -.

//* BLE part ...................... +:
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
//* BLE part ...................... -.

//+ HID part ...................... +:
#include "USB.h"
#include "USBHIDKeyboard.h"
#include <WiFi.h>
#include <esp_now.h>
String username = "hamid";
USBHIDKeyboard Keyboard;
const int buttonPin = 0; // GPIO0 is the default boot button pin
int previousButtonState = LOW;
//+ ............................... -.

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

//+ HTML func ...................... +.
void handleRoot() {
  String html =
      "<!DOCTYPE html><html><head><title>ESP32-S3 Form</title></head><body>";
  html += "<form method='POST' action='/command'>";
  html += "<label for='cmd'>Command:</label><br>";
  html += "<input type='text' id='cmd' name='cmd'><br>";
  html +=
      "<button type='button' onclick='addInput()'>Add Input Field</button><br>";
  html += "<div id='inputs'></div>";
  html += "<button type='submit'>Execute Command</button>";
  html += "</form>";
  html +=
      "<script>function addInput() { var delay = prompt('Enter delay (in "
      "seconds)'); var input = document.createElement('input'); input.type = "
      "'text'; input.name = 'input[]'; input.placeholder = 'Input'; var "
      "inputDelay = document.createElement('input'); inputDelay.type = 'text'; "
      "inputDelay.name = 'delay[]'; inputDelay.placeholder = 'Delay (s)'; "
      "inputDelay.value = delay; var div = document.getElementById('inputs'); "
      "div.appendChild(input); div.appendChild(inputDelay); }</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleCommand() {
  String cmd = server.arg("cmd");
  String inputs = "";
  String delays = "";
  for (int i = 0; i < server.args(); i++) {
    if (server.argName(i) == "input[]") {
      inputs += server.arg(i) + " ";
    }
    if (server.argName(i) == "delay[]") {
      delays += server.arg(i) + " ";
    }
  }
  // Execute command with inputs and delays
  // ...
  Serial.println("=====================================");
  Serial.println("Command: " + cmd);
  Serial.println("Inputs: " + inputs);
  Serial.println("Delays: " + delays);
  Serial.println("=====================================");

  server.send(200, "text/plain", "Command executed");
}
//+ HTML func ...................... -.
void setup() {
  Serial.begin(115200);
  Serial.println("Booting device...");
  //+.................................
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() >= 15000) {
      Serial.println("Timeout: Unable to connect to WiFi");
      break;
    }
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/", handleRoot);
  server.on("/command", HTTP_POST, handleCommand);

  server.begin();
  Serial.println("Web server started");
  //+.................................

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
  server.handleClient();
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
  // TODO: will have to change according to new requirements
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
}
void CMD() { // will open cmd
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(700);
  Keyboard.print("cmd");
  delay(300);
  ENTER();
  delay(300);
}
void cmdCommand(String command) {
  CMD();
  enter_command(command, 300);
}
void cmdCommand(String command, int DELAY) {
  CMD();
  enter_command(command, DELAY);
}
void enter_command(String command, int DELAY) {
  write_command(command);
  ENTER();
  delay(DELAY);
}
void write_command(String command) { Keyboard.print(command); }
void ENTER() {
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.release(KEY_RETURN);
  delay(100);
}