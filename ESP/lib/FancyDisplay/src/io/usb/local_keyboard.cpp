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

void LocalUSBkeyboard::update(TOFSensors_e event) {
  switch (event) {
    case TOFSensors_e::tof1:
      keyboard.press('1');
      log_i("[USB KeyBoard]: Sending key code 1");
      keyboard.release('1');

      break;
    case TOFSensors_e::tof2:
      keyboard.press('2');
      log_i("[USB KeyBoard]: Sending key code 2");
      keyboard.release('2');

      break;
    case TOFSensors_e::tof3:
      keyboard.press('3');
      log_i("[USB KeyBoard]: Sending key code 3");
      keyboard.release('3');

      break;
    case TOFSensors_e::tof4:
      keyboard.press('4');
      log_i("[USB KeyBoard]: Sending key code 4");
      keyboard.release('4');

      break;
    case TOFSensors_e::tof5:
      keyboard.press('5');
      log_i("[USB KeyBoard]: Sending key code 5");
      keyboard.release('5');

      break;
    case TOFSensors_e::tof6:
      keyboard.press('6');
      log_i("[USB KeyBoard]: Sending key code 6");
      keyboard.release('6');

      break;
    case TOFSensors_e::tof7:
      keyboard.press('7');
      log_i("[USB KeyBoard]: Sending key code 7");
      keyboard.release('7');

      break;
    case TOFSensors_e::tof8:
      keyboard.press('8');
      log_i("[USB KeyBoard]: Sending key code 8");
      keyboard.release('8');

      break;
    default:
      log_e("Unknown event: %d", event);
      break;
  }
}

std::string LocalUSBkeyboard::getName(void) {
  return std::string();
}
