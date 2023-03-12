# Project for Adam James

## Description

This is a project for Adam James. It is a simple esp32s3 USB application that will:

* Multiplex some TOF sensors
* Act as a keyboard for a PC
* Send data to a PC via USB CDC or HID

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

{
    "name": "OpenIris",
    "keywords": "esp32S, openiris, openiris-esp32s, webcam, streaming server",
    "description": "openiris library",
    "authors": [
        {
            "name": "lorow",
            "url": "https://github.com/lorow"
        },
        {
            "name": "ZanzyTHEbar",
            "url": "https://github.com/ZanzyTHEbar"
        }
    ],
    "repository": {
        "type": "git",
        "url": "https://github.com/lorow/OpenIris.git"
    },
    "export": {
        "include": "/lib/src"
    },
    "dependencies": {},
    "version": "1.10.5",
    "frameworks": "arduino",
    "platforms": "espressif32"
}
