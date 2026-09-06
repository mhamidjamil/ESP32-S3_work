# ESP32-S3 automation workspace

This repository contains the historical keyboard, BLE, RF, Wi-Fi and sensor experiments in [`old/`](old/), and the organized current work in [`v2/`](v2/).

The current boards are both ESP32-S3 devices, but their onboard RGB LED wiring differs. The USB-C DevKitC-1 style profile uses GPIO48. The confirmed CP210x micro-USB board uses GPIO38. The chip can be detected at runtime, but the RGB trace is a PCB choice and cannot be inferred safely from silicon IDs. Use the matching profile under [`v2/test/`](v2/test/).

The first combined firmware is [`v2/work/RGB+voice_groq/`](v2/work/RGB+voice_groq/). It keeps the INMP441 to Groq Whisper transcription flow and adds RGB state feedback. Keyboard and mouse automation can build on the same board later through native USB HID, BLE HID, or the existing web keyboard work preserved under `old/`.

See [`v2/README.md`](v2/README.md) for board selection, pins, upload commands and the current wake-word plan.
