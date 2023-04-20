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

  if (psensor->timeoutOccurred()) {
    log_e("[TOF Sensor]: Timeout Occurred");
    return;
  }
  average();
}

void Tof::average() {
  VL53L0X_RangingMeasurementData_t measure;
  int averaging = 0;  //  Holds value to average readings

  // Get a sampling of 5 readings from sensor
  for (int i = 0; i < NUM_SAMPLES; i++) {
    if (!psensor->rangingTest(&measure, false)) {
      log_e(
          "[TOF Sensor]: Ranging Test Failed - Sensor: 0x%02X is unresponsive",
          id);
      return;
    }

    averaging += measure.RangeMilliMeter;

    if (measure.RangeStatus != 4) {
      rangeData = {
          averaging / 5,
          measure.RangeStatus,
          measure.SignalRateRtnMegaCps,
      };
    }

    delay(55);  // Wait 55 ms between each read
                // According to datasheet time between each read
                //  is -38ms +/- 10ms. Waiting 55 ms assures each
                //  read is from a different sample
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
