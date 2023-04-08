#include "tof_vl53l0x.hpp"

Tof::Tof(Adafruit_VL53L0X* psensor,
         TwoWire* pWire,
         int id,
         int shutdown_pin,
         int interrupt_pin)
    : psensor(psensor),
      i2c_object(pWire),
      id(id),
      shutdown_pin(shutdown_pin),
      interrupt_pin(interrupt_pin),
      rangeData() {}

Tof::~Tof() {}

void Tof::begin() {
  if (!psensor->begin(id, false, i2c_object,
                      Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE)) {
    log_e("[TOF Sensor]: Failed to boot VL53L0X:  0x%02X", id);
    log_e("[TOF Sensor]: Pausing execution until reset");
    while (1)
      ;
  }
}

void Tof::loop() {
  log_d("[TOF Sensor]: Looping");

  VL53L0X_RangingMeasurementData_t measure;
  psensor->rangingTest(&measure, false);

  if (measure.RangeStatus != 4) {
    rangeData = {
        measure.RangeMilliMeter,
        measure.RangeStatus,
        measure.SignalRateRtnMegaCps,
    };
  }
}

Tof::RangeData_t* Tof::getRangeData() {
  // get range data
  return &rangeData;
}
