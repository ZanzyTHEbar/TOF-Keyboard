#include "local_keyboard.hpp"

LocalUSBkeyboard::LocalUSBkeyboard() : keyboard() {}

LocalUSBkeyboard::~LocalUSBkeyboard() {}

/**
 * @brief Begin the USB keyboard device
 * @note Setups the Device Descriptor
 *
 */
void LocalUSBkeyboard::begin(void) {
  USB.usbClass(0);
  USB.usbSubClass(0);
  USB.usbProtocol(0);
  keyboard.begin();
  USB.begin();
  log_i("[USB KeyBoard]: USB Keyboard initialized");
}

void LocalUSBkeyboard::keyEvent(uint8_t event) {
  uint8_t key = 49 + event;
  keyboard.press(key);
  log_i("[USB KeyBoard]: Sending key code %d", key);
  keyboard.release(key);
}

void LocalUSBkeyboard::update(TOFSensors_e event) {
  keyEvent(event);
}

std::string LocalUSBkeyboard::getName(void) {
  return std::string();
}
