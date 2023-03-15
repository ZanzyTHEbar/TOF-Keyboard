#include <Arduino.h>
//* Data
#include "data/events/event_manager.hpp"

//* IO
#include "io/usb/local_keyboard.hpp"

EventManager eventManager;
LocalUSBkeyboard localUSBkeyboard;

void setup() {
  Serial.begin(115200);
  eventManager.begin();
  eventManager.attach(&localUSBkeyboard);
}

void loop() {
  eventManager.loop();
}
