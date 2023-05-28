#pragma once

#include <Adafruit_VL53L0X.h>
#include <Arduino.h>
#include <Wire.h>
#include <vector>

#define XSHUT_PIN A1

class Tof {
  enum runmode_t {
    RUN_MODE_DEFAULT = 1,
    RUN_MODE_ASYNC,
    RUN_MODE_GPIO,
    RUN_MODE_CONT
  };

  runmode_t runMode;

  // The following object assumes a zero parameter constructor exists.
  Adafruit_VL53L0X* psensor;
  TwoWire* i2c_object;

  int id;             // id for the sensor
  int shutdown_pin;   // which pin for shutdown;
  int interrupt_pin;  // which pin to use for interrupts.

  struct RangeData_t {
    RangeData_t(uint16_t range = 0,
                uint8_t sensor_status = 0,
                FixPoint1616_t signal_rate = 0)
        : range(range),
          sensor_status(sensor_status),
          signal_rate(signal_rate) {}
    uint16_t range;              // range value used in continuous mode stuff.
    uint8_t sensor_status;       // status from last ranging in continuous.
    FixPoint1616_t signal_rate;  // signal rate in mega counts per second
  };

  RangeData_t rangeData;

  byte _index;
  int _total;
  int _average;
  std::vector<uint16_t> _sum;

  void average();

 public:
  Tof(Adafruit_VL53L0X* psensor,
      TwoWire* pWire,
      int id,
      int shutdown_pin,
      int interrupt_pin);
  ~Tof();
  void begin();
  void loop();
  RangeData_t* getRangeData();
};
