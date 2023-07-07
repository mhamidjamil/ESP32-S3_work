#include <SPIFFS.h>
#include <WebServer.h>
#include <WiFi.h>

// Replace with your network credentials
const char *ssid = "Archer 73";
const char *password = "Archer@73_102#";

WebServer server(80);

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
  // TODO: Execute command as they are mentioned in the web portal in there
  // pattern
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
  // Save command and input values to SPIFFS
  File file = SPIFFS.open("/history.txt", "a");
  if (file) {
    file.print(cmd);
    file.print(",");
    file.print(inputs);
    file.print(",");
    file.print(delays);
    file.println();
    file.close();
  }
  Serial.println("Command: " + cmd);
  Serial.println("Inputs: " + inputs);
  Serial.println("Delays: " + delays);
  server.send(200, "text/plain", "Command executed");
}

void handleHistory() {
  String html =
      "<!DOCTYPE html><html><head><title>ESP32-S3 History</title></head><body>";
  html += "<h1>Command History</h1>";
  html += "<ul>";
  File file = SPIFFS.open("/history.txt", "r");
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      html += "<li>" + line + "</li>";
    }
    file.close();
  }
  html += "</ul>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  SPIFFS.begin();

  server.on("/", handleRoot);
  server.on("/command", HTTP_POST, handleCommand);
  server.on("/history", handleHistory);

  server.begin();
  Serial.println("Web server started");
}

void loop() { server.handleClient(); }