#include "local_keyboard.hpp"

LocalUSBkeyboard::LocalUSBkeyboard() : keyboard() {}

LocalUSBkeyboard::~LocalUSBkeyboard() {}

/**
 * @brief Begin the USB keyboard device
 * @note Setups the Device Descriptor
 *
 */
void LocalUSBkeyboard::begin(void) {
  keyboard.init();
  keyboard.mfgStringId(keyboard.addString(USB_MANUFACTURER));
  keyboard.productStringId(keyboard.addString(USB_PRODUCT));
  keyboard.serialStringId(keyboard.addString(USB_SERIAL_NUMBER));
  keyboard.setVidPid(USB_VENDOR_ID, USB_PRODUCT_ID);
  keyboard.setVersion(USB_REVISION_BCD, USB_BCD);
  keyboard.begin(8);
}

void LocalUSBkeyboard::update(TOFSensors_e event) {
  switch (event) {
    case TOFSensors_e::tof1:
      keyboard.sendKey('1');
      log_i("[USB KeyBoard]: Sending key code 1");
      //? Do we need to call sendKey() after sendKey('')?
      keyboard.sendKey();
      break;
    case TOFSensors_e::tof2:
      keyboard.sendKey('2');
      log_i("[USB KeyBoard]: Sending key code 2");
      keyboard.sendKey();
      break;
    case TOFSensors_e::tof3:
      keyboard.sendKey('3');
      log_i("[USB KeyBoard]: Sending key code 3");
      keyboard.sendKey();
      break;
    case TOFSensors_e::tof4:
      keyboard.sendKey('4');
      log_i("[USB KeyBoard]: Sending key code 4");
      keyboard.sendKey();
      break;
    case TOFSensors_e::tof5:
      keyboard.sendKey('5');
      log_i("[USB KeyBoard]: Sending key code 5");
      keyboard.sendKey();
      break;
    case TOFSensors_e::tof6:
      keyboard.sendKey('6');
      log_i("[USB KeyBoard]: Sending key code 6");
      keyboard.sendKey();
      break;
    case TOFSensors_e::tof7:
      keyboard.sendKey('7');
      log_i("[USB KeyBoard]: Sending key code 7");
      keyboard.sendKey();
      break;
    case TOFSensors_e::tof8:
      keyboard.sendKey('8');
      log_i("[USB KeyBoard]: Sending key code 8");
      keyboard.sendKey();
      break;
    default:
      log_e("Unknown event: %d", event);
      break;
  }
}

std::string LocalUSBkeyboard::getName(void) {
  return std::string();
}
