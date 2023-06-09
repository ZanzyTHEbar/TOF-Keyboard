# Project for TOF based keyboard emulation

## Description

This is a projectthat was contracted, and uses IR based Time of Flight sensors to trigger Keyboard presses via USB HID when certain thresholds are met. It is a simple esp32s3 USB application that will:

* Multiplex some TOF sensors
* Act as a keyboard for a PC
* Send data to a PC via USB CDC or HID

> **Note**: This project can use any HID based MCU - like the Arduino Pro Micro.

## Hardware

* ESP32-S3-WROVER-1
* [vl53l4cx TOF sensor (x8)](https://learn.adafruit.com/adafruit-vl53l4cx-time-of-flight-distance-sensor)
* [I2C Multiplexer](https://www.dfrobot.com/product-1780.html)

## Software

* [platformio](https://platformio.org/)
* [vscode](https://code.visualstudio.com/)
* [arduino](https://www.arduino.cc/)
* [github](https://github.com)

## Setup

### `Hardware`

* Connect the TOF sensors to the multiplexer
* Connect the multiplexer to the ESP32-S3-WROVER-1
* Connect the ESP32-S3-WROVER-1 to your PC

### `Software`

* Install [vscode](https://code.visualstudio.com/)
* Install [platformio](https://platformio.org/)
* Clone this repo
* Open the project in vscode
* Build and upload the project to the ESP32-S3-WROVER-1

## Usage
