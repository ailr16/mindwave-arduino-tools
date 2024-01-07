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
  headset.init();
  pinMode(13, OUTPUT);
}

void loop() {

}

int flag = 0;
int readHeadsetFlag = 0;

ISR(TIMER1_COMPA_vect)
{
  flag ^= 1;
  digitalWrite(13, flag);
  headset.readHeadset();
}