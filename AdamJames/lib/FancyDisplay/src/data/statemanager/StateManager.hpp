#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

/*
 * StateManager
 * All Project States are managed here
 */
namespace ProgramStates {
  struct DeviceStates {
    enum State_e { Starting, Started, Stopping, Stopped, Error };

    enum TOFSensors_e {
      tof1,
      tof2,
      tof3,
      tof4,
      tof5,
      tof6,
      tof7,
      tof8
    };

    enum Event_e {
      configLoaded,
      deviceConfigUpdated,
      sensorConfigUpdated,
      keyboardConfigUpdated,
    };
  };
};  // namespace ProgramStates

/*
 * EventManager
 * All Project Events are managed here
 */
template <class T>
class StateManager {
 public:
  StateManager() {}

  virtual ~StateManager() { this->_current_state = static_cast<T>(0); }

  /*
   * @brief Sets the  state of the stateManager
   * @param T state - the state to be set
   */
  void setState(T state) { _current_state = state; }

  /*
   * Get States
   * Returns the current state of the device
   */
  T getCurrentState() { return _current_state; }

 private:
  T _current_state;
};

typedef ProgramStates::DeviceStates::State_e State_e;
typedef ProgramStates::DeviceStates::Event_e Event_e;
typedef ProgramStates::DeviceStates::TOFSensors_e TOFSensors_e;

extern StateManager<State_e> states;
extern StateManager<Event_e> events;
extern StateManager<TOFSensors_e> tof;

#endif  // STATEMANAGER_HPP
