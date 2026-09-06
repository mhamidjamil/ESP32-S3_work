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

`work/RGB+voice_groq` combines the existing INMP441 and Groq Whisper flow with RGB status and local ESP-SR wake detection:

- red: Wi-Fi is unavailable
- green: Wi-Fi is connected and idle
- blue: recording or uploading
- recording lasts at least ten seconds, then three seconds of detected silence ends it
- a 60-second safety limit prevents an unattended recording from running forever
- the bundled offline wake word is `Hi ESP`; it does not consume Groq quota while idle

BOOT remains available as a manual fallback. The ESP-SR model is stored in flash, so compile this sketch with the `ESP SR 16M` partition scheme, `32MB` flash setting, and `OPI PSRAM`. The current sketch uses the bundled `Hi ESP` model. An exact `Jarvis` wake word requires replacing it with a custom WakeNet model; it is not something the board learns from one spoken example.

Copy `secrets.example.h` to `secrets.h`, fill in credentials, and compile with Espressif Arduino core 3.3.11. Select `ESP SR 16M` in the Arduino IDE partition menu. For GPIO48 use `-DBOARD_RGB_GPIO48`; GPIO38 is the default profile. RGB brightness is set to 32/255, half the previous firmware level.

Older experiments are in `../old`.

## Which work folder should be uploaded?

- `work/RGB+voice_groq` is the lightweight Groq-only firmware. It waits for BOOT, records the INMP441 clip, and sends one transcription request.
- `work/RGB+voice_groq_wakeword` is the phase-two firmware. It runs ESP-SR locally for the bundled `Hi ESP` wake word, then pauses the detector and sends the spoken clip to Groq. It needs the `ESP SR 16M` partition scheme because the wake model is stored in flash.

Keep these folders separate so a future Groq-only upload does not accidentally include the wake-word model or partition requirements.
