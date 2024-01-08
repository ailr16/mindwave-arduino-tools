////////////////////////////////////////////////////////////////////////
// Arduino Bluetooth Interface with Mindwave
//
// Sketch to test Mindwave API implementations
// 
// Mexico  01.06.2023
////////////////////////////////////////////////////////////////////////

#define DEBUGOUTPUT 0
#define SERIAL_USB_BAUDRATE 57600

#include "mindwaveTools.h"

MindwaveHeadset headset(Serial1);

void setup() {
  Serial.begin(SERIAL_USB_BAUDRATE);   // USB
  Serial1.begin(57600);
  headset.init();
  pinMode(13, OUTPUT);
}

void loop() {
  headset.readHeadset();
  //Serial.println(headset.getRaw());
}
