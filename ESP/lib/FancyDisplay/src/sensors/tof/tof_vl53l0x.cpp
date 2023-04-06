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
      rangeData(0, 0) {}

Tof::~Tof() {}

void Tof::begin() {
  pinMode(13, OUTPUT);
  pinMode(shutdown_pin, OUTPUT);
  digitalWrite(shutdown_pin, LOW);
  if (interrupt_pin >= 0) {
    pinMode(interrupt_pin, INPUT_PULLUP);
  }

  // initialize sensor
  delay(10);
  digitalWrite(shutdown_pin, HIGH);
  delay(10);
  if (!psensor->begin(id, false, i2c_object, rangeData.sensor_config)) {
    log_e("[TOF Sensor]: Failed to boot VL53L0X: %d", id);
    log_e("[TOF Sensor]: Pausing execution until reset");
    while (1)
      ;
  }
}

void Tof::loop() {
  log_d("[TOF Sensor]: Looping");
}

Tof::RangeData_t* Tof::getRangeData() {
  // get range data
  return &rangeData;
}
