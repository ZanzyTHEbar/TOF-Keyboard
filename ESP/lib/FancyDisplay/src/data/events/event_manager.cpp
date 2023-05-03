#include "event_manager.hpp"

EventManager::EventManager() : multiplexer(WIRE_OBJECT, 0x70), _tof_sensors{} {}

EventManager::~EventManager() {}

void EventManager::begin(void) {
  //* initialize multiplexer and i2c bus
  multiplexer.begin();
  log_d("[Event Manager]: Multiplexer initialized");
  log_d("[Event Manager]: Scanning for connected sensors");

  _tof_sensors.reserve(8);
  // scan for sensors
  for (uint8_t port = 0; port <= 8; port++) {
    log_i("[Event Manager]: Scanning Port: %d \n", port);
    uint8_t* dev = multiplexer.scan(port);
    while (*dev) {
      log_i("[Event Manager]: Found Device at i2c addr: 0x%02X \n", *dev);
      //* add sensor to the vector
      _tof_sensors.emplace_back(
          new Tof(new Adafruit_VL53L0X(), WIRE_OBJECT, *dev, 0, 1));
      log_d("[Event Manager]: Adding sensor to vector");
      //* initialize sensor
      multiplexer.selectPort(port);
      _tof_sensors.at(port)->begin();
      dev++;
    }
    log_d("[Event Manager]: Size of vector: %d \n", _tof_sensors.size());
  }
}

void EventManager::loop(void) {
  // loop through multiplexer ports
  for (uint8_t port = 0; port < _tof_sensors.size(); port++) {
    // select the port
    log_d("[Event Manager]: Selecting port %d", port);
    multiplexer.selectPort(port);
    // communicate with the sensor
    log_d("[Event Manager]: Communicating with sensor at port %d", port);
    _tof_sensors.at(port)->loop();

    // check the distance of the sensor and notify the observers if the distance
    // is under 2000mm
    auto range = _tof_sensors.at(port)->getRangeData();

    log_i("[Event Manager]: Sensor at port %d has range: %d \n", port,
          range->range);
    if (range->range > 0 && range->range <= MAX_RANGE) {
      notifyAll((TOFSensors_e)port);
    }
  }
}
