# V2 ESP32-S3 work

This version separates board-specific smoke tests from the combined voice and automation firmware.

## Board profiles

| Folder | Board profile | RGB data pin | USB note |
|---|---|---:|---|
| `test/esp32s3usbc` | ESP32-S3-DevKitC-1 v1.0 style, native USB-C | GPIO48 | Native USB or USB-UART depending on the board |
| `test/esp32s3microusb` | ESP32-S3-DevKitC-1 v1.1 style, confirmed CP210x micro-USB board | GPIO38 | USB-UART bridge |

Each profile has an Adafruit NeoPixel test and an Arduino built-in `neopixelWrite` test.

The ESP32 chip can report its family, flash and PSRAM. It cannot report which PCB trace is connected to the onboard LED, so RGB pin selection remains a board profile choice. Espressif documents GPIO48 for DevKitC-1 v1.0 and GPIO38 for v1.1.

## Combined firmware

`work/RGB+voice_groq` combines the existing INMP441 and Groq Whisper flow with RGB status:

- red: Wi-Fi is unavailable
- green: Wi-Fi is connected and idle
- blue: recording or uploading
- three seconds of detected silence ends a recording

The first combined test remains BOOT-button triggered. A true `Jarvis` wake word needs a local wake-word model or repeated short cloud transcription, so it is kept as the next stage rather than silently consuming the Groq free-tier quota.

Copy `secrets.example.h` to `secrets.h`, fill in credentials, and compile with Espressif Arduino core 3.3.11. For GPIO48 use `-DBOARD_RGB_GPIO48`; GPIO38 is the default profile.

Older experiments are in `../old`.
