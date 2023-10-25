# Using ESP32S3 as a HID device.

This project enables communication between an ESP32-S3 board and a connected BLE device. The ESP32-S3 board acts as a BLE/Web server, receiving commands from the connected device and performing specific actions based on the received commands. Additionally, it functions as an HID, allowing it to act as a keyboard and execute commands on the CMD or run applications.

## Prerequisites

- Arduino IDE
- ESP32-S3 board
- BLE device (e.g., smartphone, tablet)

## Getting Started

1. Install the necessary libraries:

   - `BLEDevice`
   - `BLEServer`
   - `BLEUtils`
   - `USB`
   - `USBHIDKeyboard`
   - `WiFi`
   - `esp_now`

2. Upload the code to your ESP32-S3 board using the Arduino IDE.

3. Connect your ESP32-S3 board to a power source.

4. Scan for available Bluetooth devices on your BLE device (e.g., smartphone or tablet), or use web portal (localhost)

5. Connect to the ESP32-S3 BLE device from your BLE device.

6. Send commands to the ESP32-S3 BLE device using the connected BLE device (or use ip address assigned to esp32s3 for web base keyboard). Supported commands include:

   - "test" - Print a defined word to the serial monitor.
   - "#run\_[command]" - Execute a command on the ESP32-S3 board.
   - "#cmd\_[command]" - Execute a command in the Windows command prompt.
   - Other keywords and string will be send as it is to usb connected device.

7. The ESP32-S3 board will perform the specified actions based on the received commands. It can act as an HID and execute commands on the CMD or run applications.

## Troubleshooting

If you encounter any issues or errors, please refer to the documentation or seek support from the ESP32-S3 and BLE community.

**Note:** I implementing new functionalities in this project, so this readme file might be outdated, so you better check completed issues labled with _feature_ to know the latest implemented feature.
