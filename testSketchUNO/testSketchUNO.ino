////////////////////////////////////////////////////////////////////////
// Arduino Bluetooth Interface with Mindwave
//
// Sketch to test Mindwave API implementations
// Tested on Arduino UNO using HC05
//
// https://github.com/ailr16/mindwave-arduino-tools
// 
// Mexico  01.11.2023
////////////////////////////////////////////////////////////////////////

#define SERIAL_USB_BAUDRATE   115200
#define PIN_QUALITY_INDICATOR 13

#include "mindwaveTools.h"

MindwaveHeadset headset( Serial );

void setup() {
  Serial.begin(HEADSET_BAUDRATE);
  pinMode(PIN_QUALITY_INDICATOR, OUTPUT);
  headset.setOutputQualityPin( PIN_QUALITY_INDICATOR );
}


void loop() {
  headset.readHeadset();  
}
