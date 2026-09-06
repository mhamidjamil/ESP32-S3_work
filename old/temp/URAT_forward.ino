
void setup() {
  // Initialize the software serial communication with Orange Pi (115200 baud)
  Serial2.begin(1500000, SERIAL_8N1, 4, 5);

  // Initialize the serial communication with the computer (Serial Monitor)
  Serial.begin(115200);
  Serial.println("Starting monitoring");
}

void loop() {
  // Check if data is available on the Orange Pi's UART
  if (Serial2.available()) {
    // Read the data from Orange Pi's UART
    char data = Serial2.read();

    // Forward the data to the computer's serial terminal (Serial Monitor)
    Serial.write(data);
  }

  // Check if data is available on the computer's serial terminal
  if (Serial.available()) {
    // Read the data from the computer's serial terminal
    char data = Serial.read();

    // Forward the data to the Orange Pi's UART
    Serial2.write(data);
  }
}
