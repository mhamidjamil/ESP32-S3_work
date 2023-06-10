#include <WebServer.h>
#include <WiFi.h>

// Replace with your network credentials
// WIFI:T:WPA;P:Archer@73_102#;S:Archer 73;H:false;
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
  Serial.println("Command: " + cmd);
  Serial.println("Inputs: " + inputs);
  Serial.println("Delays: " + delays);
  Serial.println("=====================================");

  server.send(200, "text/plain", "Command executed");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/", handleRoot);
  server.on("/command", HTTP_POST, handleCommand);

  server.begin();
  Serial.println("Web server started");
}

void loop() { server.handleClient(); }