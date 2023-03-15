#include "event_manager.hpp"

EventManager::EventManager()
    : multiplexer(WIRE_OBJECT, 0x70), _tof_sensors{nullptr} {}

EventManager::~EventManager() {}

void EventManager::begin(void) {
  //* initialize multiplexer
  multiplexer.begin();
  log_d("[Event Manager]: Multiplexer initialized");
  log_d("[Event Manager]: Scanning for connected sensors");
  // scan for sensors
  for (uint8_t port = 0; port < 8; port++) {
    Serial.printf("Scanning Port: %d \n", port);
    uint8_t* dev = multiplexer.scan(port);
    while (*dev) {
      Serial.printf("Found Device at i2c addr: 0x%02X \n", *dev);
      //* add sensor to the vector
      _tof_sensors.emplace_back(new Tof(WIRE_OBJECT));
      Serial.println("Adding sensor to vector");
      //* initialize sensor
      _tof_sensors.back()->begin();
      dev++;
    }
    Serial.printf("Size of vector: %d \n", _tof_sensors.size());
    Serial.println();
  }
}

void EventManager::loop(void) {
  // loop through multiplexer ports
  for (uint8_t port = 0; port < _tof_sensors.size(); port++) {
    // select the port
    multiplexer.selectPort(port);
    // communicate with the sensor
    _tof_sensors.at(port)->loop();

    // check the distance of the sensor and notify the observers if the distance
    // is under 2000mm
    auto range = _tof_sensors.at(port)->getRangeData();

    for (auto& r : *range) {
      Serial.printf("Sensor at port %d has range: %d \n", port,
                    r.rangeMilliMeter);
      if (r.rangeMilliMeter <= 2000) {
        notifyAll((TOFSensors_e)port);
      }
    }
  }
  // if the status is different from the previous status
  // notify the observers
}
