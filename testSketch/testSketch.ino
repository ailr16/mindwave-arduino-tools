////////////////////////////////////////////////////////////////////////
// Arduino Bluetooth Interface with Mindwave
//
// Sketch to test Mindwave API implementations
// 
// Mexico  01.11.2023
////////////////////////////////////////////////////////////////////////

#define SERIAL_USB_BAUDRATE 115200

#include "mindwaveTools.h"

MindwaveHeadset headset( Serial1 );

void setup() {
  Serial.begin(SERIAL_USB_BAUDRATE);   // USB
  Serial1.begin(HEADSET_BAUDRATE);
  pinMode(13, OUTPUT);
}


void loop() {
  headset.readHeadset();
  
  Serial.print(headset.getQuality());
  Serial.print(",");
  Serial.print(headset.getAttention());
  Serial.print(",");
  Serial.print(headset.getMeditation());
  Serial.print(",");
  Serial.print(headset.getRaw());
  Serial.print("\n");
  
}
