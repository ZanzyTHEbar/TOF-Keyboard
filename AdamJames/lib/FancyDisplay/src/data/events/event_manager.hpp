#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <Wire.h>
#define WIRE_OBJECT &Wire

#include <vector>
#include "data/statemanager/StateManager.hpp"
#include "data/utilities/observer.hpp"

#include "io/i2c/multiplexer.hpp"
#include "sensors/tof/tof.hpp"

//! These two objects must use the same instance of Wire

class EventManager : public ISubject<TOFSensors_e> {
  I2C_Multiplexer multiplexer;
  std::vector<Tof*> _tof_sensors;

 public:
  EventManager();
  ~EventManager();
  void loop(void);
  void begin(void);
};

#endif  // EVENT_MANAGER_HPP
