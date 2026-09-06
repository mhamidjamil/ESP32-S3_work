# ESP32-S3 RGB LED test

This is a standalone test for the dual-USB ESP32-S3-WROOM-1 N8R2 / YD-ESP32-S3-style board.

## Hardware note

The onboard LED is an addressable WS2812/SK68XX LED, not a three-pin analog RGB LED. The usual data pin is GPIO48. On the clone board, close the two pads marked `RGB` or `RGB CTRL` if the LED circuit is disconnected. Do this with USB power removed.

Do not bridge the `IN-OUT` or `USB-OTG` jumpers for this test.

## Upload

Select:

- Board: `ESP32S3 Dev Module`
- Port: the USB-UART port, commonly `/dev/ttyACM0` or `/dev/ttyUSB0`
- Upload speed: 115200 if the faster speed has trouble

## Serial commands

Open the serial monitor at 115200 baud:

```text
brightness 32
pattern rainbow
pattern solid
color 255 0 0
pattern blink
pin 48
off
```

Brightness is scaled from `0` to `255`; the default `32` is about 12.5% of full brightness. The program uses Arduino-ESP32's built-in `neopixelWrite()` function, which generates the timing required by the addressable LED.
