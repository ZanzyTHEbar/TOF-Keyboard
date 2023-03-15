#ifndef LOCAL_KEYBOARD_HPP
#define LOCAL_KEYBOARD_HPP

#include <Arduino.h>
#include <usb_keyboard.hpp>
#include "data/utilities/observer.hpp"
#include "data/statemanager/StateManager.hpp"
using namespace esptinyusb;
class LocalUSBkeyboard : public IObserver<TOFSensors_e> {
  esptinyusb::USBkeyboard keyboard;

 public:
  LocalUSBkeyboard();
  ~LocalUSBkeyboard();
  void begin(void);
  void update(TOFSensors_e event) override;
  std::string getName(void) override;
};

#endif  // LOCAL_KEYBOARD_HPP
