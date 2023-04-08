#ifndef LOCAL_KEYBOARD_HPP
#define LOCAL_KEYBOARD_HPP

#include <Arduino.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include "data/statemanager/StateManager.hpp"
#include "data/utilities/observer.hpp"

class LocalUSBkeyboard : public IObserver<TOFSensors_e> {
  USBHIDKeyboard keyboard;

  void keyEvent(uint8_t event);

 public:
  LocalUSBkeyboard();
  ~LocalUSBkeyboard();
  void begin(void);
  void update(TOFSensors_e event) override;
  std::string getName(void) override;
};

#endif  // LOCAL_KEYBOARD_HPP
