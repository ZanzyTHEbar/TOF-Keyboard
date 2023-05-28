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
      rangeData(),
      _index(0),
      _total(0),
      _average(0),
      _sum{0} {
  _sum.reserve(NUM_SAMPLES);
}

Tof::~Tof() {
  log_d("[TOF Sensor]: Destroying TOF Sensor");
  delete psensor;
  delete i2c_object;
  // free the vector
  _sum.clear();
  log_d("[TOF Sensor]: TOF Sensor Destroyed");
}

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

  if (psensor->timeoutOccurred()) {
    log_e("[TOF Sensor]: Timeout Occurred");
    return;
  }
  average();
}

void Tof::average() {
  VL53L0X_RangingMeasurementData_t measure;

  //* Rolling Average
  int distance = 0;
  this->_total = _total - _sum.at(_index);
  _sum[_index] = measure.RangeMilliMeter;
  _total = _total + _sum.at(_index);
  _index++;

  if (_index >= NUM_SAMPLES) {
    _index = 0;
  }

  distance = _total / NUM_SAMPLES;
  //* End Rolling Average

  if (measure.RangeStatus != 4) {
    rangeData = {
        distance,
        measure.RangeStatus,
        measure.SignalRateRtnMegaCps,
    };
    measure.RangeMilliMeter = 0;
  }
}

/*
 * Comparison function for qsort(): return an integer "less than, equal
 * to, or greater than zero if the first argument is considered to be
 * respectively less than, equal to, or greater than the second."
 */
int compare(const void* pa, const void* pb) {
  int a = *(int*)pa;
  int b = *(int*)pb;
  return a < b ? -1 : a > b ? +1 : 0;
}

#define FILTER_LENGTH 11  // length of the median filter, should be odd

/*
 * Input: raw sample.
 * Ouput: filtered sample.
 */
int median_filter(int data) {
  static int input_buffer[FILTER_LENGTH];
  static int input_index;
  int sorted_buffer[FILTER_LENGTH];

  // Store the current data point.
  input_buffer[input_index] = data;
  if (++input_index >= FILTER_LENGTH)
    input_index = 0;

  // Sort a copy of the input buffer.
  for (int i = 0; i < FILTER_LENGTH; i++)
    sorted_buffer[i] = input_buffer[i];
  qsort(sorted_buffer, FILTER_LENGTH, sizeof *sorted_buffer, compare);

  // Return median.
  return sorted_buffer[FILTER_LENGTH / 2];
}

Tof::RangeData_t* Tof::getRangeData() {
  // get range data
  return &rangeData;
}
