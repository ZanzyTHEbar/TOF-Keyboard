#include "tof.hpp"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tof::Tof(TwoWire* pWire)
    : i2c_object(pWire),
      sensor_vl53l4cx_sat(),
      showGraphicly(true) {}

Tof::~Tof() {}

/* I2CDeviceAvailable --------------------------------------------------------*/
bool I2CDeviceAvailable(uint8_t address, TwoWire** wire) {
  byte error = 1;
  bool available = false;

  // Check if device is available at the expected address
  Wire.begin();
  Wire.beginTransmission(address);
  error = Wire.endTransmission();

  if (error == 0) {
    *wire = &Wire;
    available = true;

    Serial.printf("I2c Device Found at Address 0x%02X on Wire \n", address,
                  HEX);
    log_i("I2c Device Found at Address 0x%02X", address);
  }
  Wire.end();

#if defined(ARDUINO_ADAFRUIT_KB2040_RP2040) ||    \
    defined(ARDUINO_ADAFRUIT_ITSYBITSY_RP2040) || \
    defined(ARDUINO_ADAFRUIT_QTPY_RP2040) ||      \
    defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) ||   \
    defined(ARDUINO_ADAFRUIT_QTPY_ESP32S2) ||     \
    defined(ARDUINO_ADAFRUIT_QTPY_ESP32_PICO)
  if (error != 0) {
    Wire1.begin();
    Wire1.beginTransmission(address);
    byte error1 = Wire1.endTransmission();
    if (error1 == 0) {
      *wire = &Wire1;
      available = true;

      Serial.print("  I2c Device Found at Address 0x");
      Serial.print(address, HEX);
      Serial.println(" on Wire1");
    }
    Wire1.end();
  }
#endif
  return available;
}

/* Setup ---------------------------------------------------------------------*/
void Tof::begin() {
  VL53L4CX_Error error = VL53L4CX_ERROR_NONE;

  // delay to view any serial setup output before any loop output is displayed
  delay(10000);

#if defined(LED_BUILTIN)
  // Led.
  pinMode(LED_BUILTIN, OUTPUT);
#endif

  // Initialize serial for output.
  log_i("[Tof]: Starting up TOF sensors");

#if defined(ARDUINO_ADAFRUIT_QTPY_ESP32S2) ||         \
    defined(ARDUINO_ADAFRUIT_QTPY_ESP32S3_NOPSRAM) || \
    defined(ARDUINO_ADAFRUIT_QTPY_ESP32_PICO)
  // ESP32 is kinda odd in that secondary ports must be manually
  // assigned their pins with setPins()!
  Wire1.setPins(SDA1, SCL1);
#endif

#if defined(ARDUINO_ADAFRUIT_FEATHER_ESP32_V2)
  pinMode(NEOPIXEL_I2C_POWER, OUTPUT);
  digitalWrite(NEOPIXEL_I2C_POWER, HIGH);
#endif

  // Determine the ToF device's address and Twowire device to which it is
  // connected. The default address must be shifted right 1 bit to match the
  // expected physical I2c address.
  if (I2CDeviceAvailable(VL53L4CX_DEFAULT_DEVICE_ADDRESS >> 1, &i2c_object)) {
    i2c_object->begin();
    sensor_vl53l4cx_sat.setI2cDevice(i2c_object);
    sensor_vl53l4cx_sat.setXShutPin(XSHUT_PIN);
  } else {
    // TODO: Add support for multiplexer. For now, just fail.
    log_e("[TOF]: Failure Initializing I2c Port or No I2c device found");
    while (true)
      delay(10);
  }

  // Configure VL53L4CX satellite component.
  sensor_vl53l4cx_sat.begin();

  // Switch off VL53L4CX satellite component.
  sensor_vl53l4cx_sat.VL53L4CX_Off();

  // Initialize VL53L4CX satellite component.
  error = sensor_vl53l4cx_sat.InitSensor(VL53L4CX_DEFAULT_DEVICE_ADDRESS);

  if (error != VL53L4CX_ERROR_NONE) {
    log_e("[TOF]: Error Initializing Sensor: %s", errorCodesMap(error).c_str());
    while (true)
      delay(10);
  }

  // Start Measurements
  sensor_vl53l4cx_sat.VL53L4CX_StartMeasurement();
}

/* Loop ---------------------------------------------------------------------*/
void Tof::loop() {
  VL53L4CX_MultiRangingData_t MultiRangingData;
  VL53L4CX_MultiRangingData_t* pMultiRangingData = &MultiRangingData;
  uint8_t NewDataReady = 0;
  int no_of_object_found = 0, j;
  char report[64];
  int status;

  do {
    status =
        sensor_vl53l4cx_sat.VL53L4CX_GetMeasurementDataReady(&NewDataReady);
  } while (!NewDataReady);

#if defined(LED_BUILTIN)
  // Led on
  digitalWrite(LED_BUILTIN, HIGH);
#endif

  if ((!status) && (NewDataReady != 0)) {
    status =
        sensor_vl53l4cx_sat.VL53L4CX_GetMultiRangingData(pMultiRangingData);
    no_of_object_found = pMultiRangingData->NumberOfObjectsFound;

    // assign values to the RangeData_t struct
    for (j = 0; j < no_of_object_found; j++) {
      this->rangeData.emplace_back(
          pMultiRangingData->RangeData[j].RangeStatus,
          pMultiRangingData->RangeData[j].RangeMilliMeter,
          pMultiRangingData->RangeData[j].SignalRateRtnMegaCps,
          pMultiRangingData->RangeData[j].AmbientRateRtnMegaCps);
    }

    if (showGraphicly) {
      for (j = 0; j < no_of_object_found; j++) {
        if (pMultiRangingData->RangeData[j].RangeStatus ==
                VL53L4CX_RANGESTATUS_RANGE_VALID ||
            pMultiRangingData->RangeData[j].RangeStatus ==
                VL53L4CX_RANGESTATUS_RANGE_VALID_MERGED_PULSE) {
          int16_t mm = pMultiRangingData->RangeData[j].RangeMilliMeter;
          for (int k = 0; k < mm / 10; k++)
            Serial.print(" ");
          Serial.println(mm);
        }
      }
    } else {
      snprintf(report, sizeof(report),
               "VL53L4CX Satellite: Count=%d, #Objs=%1d ",
               pMultiRangingData->StreamCount, no_of_object_found);
      log_i("%s", report);
      for (j = 0; j < no_of_object_found; j++) {
        log_i("\r\n                               ");
        log_i("status=%d, D=%dmm, Signal=%f Mcps, Ambient=%f Mcps, %s",
              pMultiRangingData->RangeData[j].RangeStatus,
              pMultiRangingData->RangeData[j].RangeMilliMeter,
              (float)pMultiRangingData->RangeData[j].SignalRateRtnMegaCps /
                  65536.0,
              (float)pMultiRangingData->RangeData[j].AmbientRateRtnMegaCps /
                  65536.0,
              rangeStatusCode(pMultiRangingData->RangeData[j].RangeStatus)
                  .c_str());
      }
    }

    if (status == 0) {
      status = sensor_vl53l4cx_sat.VL53L4CX_ClearInterruptAndStartMeasurement();
    }
  }

#if defined(LED_BUILTIN)
  digitalWrite(LED_BUILTIN, LOW);
#endif
}

//****************************************************************************
//!                               Utilities
//****************************************************************************

/* VL53L4CX_RangeStatusCode --------------------------------------------------*/
std::string Tof::rangeStatusCode(uint8_t status) {
  switch (status) {
    case VL53L4CX_RANGESTATUS_RANGE_VALID:
      return "VL53L4CX_RANGESTATUS_RANGE_VALID";
    case VL53L4CX_RANGESTATUS_SIGMA_FAIL:
      return "VL53L4CX_RANGESTATUS_SIGMA_FAIL";
    case VL53L4CX_RANGESTATUS_RANGE_VALID_MIN_RANGE_CLIPPED:
      return "VL53L4CX_RANGESTATUS_RANGE_VALID_MIN_RANGE_CLIPPED";
    case VL53L4CX_RANGESTATUS_OUTOFBOUNDS_FAIL:
      return "VL53L4CX_RANGESTATUS_OUTOFBOUNDS_FAIL";
    case VL53L4CX_RANGESTATUS_HARDWARE_FAIL:
      return "VL53L4CX_RANGESTATUS_HARDWARE_FAIL";
    case VL53L4CX_RANGESTATUS_RANGE_VALID_NO_WRAP_CHECK_FAIL:
      return "VL53L4CX_RANGESTATUS_RANGE_VALID_NO_WRAP_CHECK_FAIL";
    case VL53L4CX_RANGESTATUS_WRAP_TARGET_FAIL:
      return "VL53L4CX_RANGESTATUS_WRAP_TARGET_FAIL";
    case VL53L4CX_RANGESTATUS_PROCESSING_FAIL:
      return "VL53L4CX_RANGESTATUS_PROCESSING_FAIL";
    case VL53L4CX_RANGESTATUS_XTALK_SIGNAL_FAIL:
      return "VL53L4CX_RANGESTATUS_XTALK_SIGNAL_FAIL";
    case VL53L4CX_RANGESTATUS_SYNCRONISATION_INT:
      return "VL53L4CX_RANGESTATUS_SYNCRONISATION_INT";
    case VL53L4CX_RANGESTATUS_RANGE_VALID_MERGED_PULSE:
      return "VL53L4CX_RANGESTATUS_RANGE_VALID_MERGED_PULSE";
    case VL53L4CX_RANGESTATUS_TARGET_PRESENT_LACK_OF_SIGNAL:
      return "VL53L4CX_RANGESTATUS_TARGET_PRESENT_LACK_OF_SIGNAL";
    case VL53L4CX_RANGESTATUS_MIN_RANGE_FAIL:
      return "VL53L4CX_RANGESTATUS_MIN_RANGE_FAIL";
    case VL53L4CX_RANGESTATUS_RANGE_INVALID:
      return "VL53L4CX_RANGESTATUS_RANGE_INVALID";
    case VL53L4CX_RANGESTATUS_NONE:
      return "VL53L4CX_RANGESTATUS_NONE";
    default:
      return ("UNKNOWN STATUS: " + std::to_string(status));
  }
}

/* VL53L4CX_ErrorCodesMap ------------------------------------------------*/
std::string Tof::errorCodesMap(VL53L4CX_Error status) {
  switch (status) {
    case VL53L4CX_ERROR_NONE:
      return "VL53L4CX_ERROR_NONE";
    case VL53L4CX_ERROR_CALIBRATION_WARNING:
      return "VL53L4CX_ERROR_CALIBRATION_WARNING";
    case VL53L4CX_ERROR_MIN_CLIPPED:
      return "VL53L4CX_ERROR_MIN_CLIPPED";
    case VL53L4CX_ERROR_UNDEFINED:
      return "VL53L4CX_ERROR_UNDEFINED";
    case VL53L4CX_ERROR_INVALID_PARAMS:
      return "VL53L4CX_ERROR_INVALID_PARAMS";
    case VL53L4CX_ERROR_NOT_SUPPORTED:
      return "VL53L4CX_ERROR_NOT_SUPPORTED";
    case VL53L4CX_ERROR_RANGE_ERROR:
      return "VL53L4CX_ERROR_RANGE_ERROR";
    case VL53L4CX_ERROR_TIME_OUT:
      return "VL53L4CX_ERROR_TIME_OUT";
    case VL53L4CX_ERROR_MODE_NOT_SUPPORTED:
      return "VL53L4CX_ERROR_MODE_NOT_SUPPORTED";
    case VL53L4CX_ERROR_BUFFER_TOO_SMALL:
      return "VL53L4CX_ERROR_BUFFER_TOO_SMALL";
    case VL53L4CX_ERROR_GPIO_NOT_EXISTING:
      return "VL53L4CX_ERROR_GPIO_NOT_EXISTING";
    case VL53L4CX_ERROR_GPIO_FUNCTIONALITY_NOT_SUPPORTED:
      return "VL53L4CX_ERROR_GPIO_FUNCTIONALITY_NOT_SUPPORTED";
    case VL53L4CX_ERROR_CONTROL_INTERFACE:
      return "VL53L4CX_ERROR_CONTROL_INTERFACE";
    case VL53L4CX_ERROR_INVALID_COMMAND:
      return "VL53L4CX_ERROR_INVALID_COMMAND";
    case VL53L4CX_ERROR_DIVISION_BY_ZERO:
      return "VL53L4CX_ERROR_DIVISION_BY_ZERO";
    case VL53L4CX_ERROR_REF_SPAD_INIT:
      return "VL53L4CX_ERROR_REF_SPAD_INIT";
    case VL53L4CX_ERROR_NOT_IMPLEMENTED:
      return "VL53L4CX_ERROR_NOT_IMPLEMENTED";
    default:
      return ("UNKNOWN STATUS: " + std::to_string(status));
  }
}

std::vector<Tof::RangeData_t>* Tof::getRangeData() {
  return &this->rangeData;
}
