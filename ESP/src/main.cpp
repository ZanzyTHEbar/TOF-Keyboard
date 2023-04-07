#include <Arduino.h>
//* Data
#include "data/events/event_manager.hpp"

//* IO
#include "io/usb/local_keyboard.hpp"

EventManager eventManager;
LocalUSBkeyboard localUSBkeyboard;

void setup() {
  Serial.begin(115200);
  eventManager.begin();
  localUSBkeyboard.begin();
  eventManager.attach(&localUSBkeyboard);
}

void loop() {
  eventManager.loop();
}

/*
#include <Adafruit_VL53L0X.h>
#define TCAADDR 0x70

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Helper function for changing TCA output channel
void tcaselect(uint8_t channel) {
  if (channel > 7)
    return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("ESP32 S3 + VL53L0X + TCA Test");

  // NOTE!!! VERY IMPORTANT!!!
  // Must call this once manually before first call to tcaselect()
  Wire.begin();

  // Before using any BME280, call tcaselect to set the channel.
  tcaselect(0);  // TCA channel for bme1
  lox.begin();   // use the default address
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;

  Serial.print("Reading a measurement... ");
  tcaselect(0);
  lox.rangingTest(&measure,
                  false);  // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }

  delay(100);
} */
