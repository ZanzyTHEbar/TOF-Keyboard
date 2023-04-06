#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <vl53l4cx_class.h>
#include <vector>

#define XSHUT_PIN A1

class Tof {
  // Components.
  TwoWire* i2c_object;

  // The following object assumes a zero parameter constructor exists.
  VL53L4CX sensor_vl53l4cx_sat;
  struct RangeData_t {
    RangeData_t(uint8_t rangeStatus,
                uint16_t rangeMilliMeter,
                uint32_t signalRateRtnMegaCps,
                uint32_t ambientRateRtnMegaCps)
        : rangeStatus(rangeStatus),
          rangeMilliMeter(rangeMilliMeter),
          signalRateRtnMegaCps(std::move(signalRateRtnMegaCps)),
          ambientRateRtnMegaCps(std::move(ambientRateRtnMegaCps)) {}
    uint8_t rangeStatus;
    uint16_t rangeMilliMeter;
    uint32_t signalRateRtnMegaCps;
    uint32_t ambientRateRtnMegaCps;
  };

  std::vector<RangeData_t> rangeData;

 public:
  Tof(TwoWire* pWire);
  ~Tof();
  void begin();
  void loop();

  std::string errorCodesMap(VL53L4CX_Error status);
  std::string rangeStatusCode(uint8_t status);
  std::vector<RangeData_t>* getRangeData();

  //* Variables
  // Display valid results "graphically" when true instead of details
  bool showGraphicly;
};
