/*
  Arduino Bluetooth Interface with Mindwave

  Get RAW data through serial monitor
  ** REQUIRES Arduino Mega with HC05 connected in Serial Port 1 (pins 18 and 19) **

  Lozano Ramirez Angel Ivan
  Mexico  01.12.2024
*/

#include "mindwaveTools.h"

#define SERIAL_USB_BAUDRATE 115200
#define DATA_QUALITY_LED 13

MindwaveHeadset headset( Serial1 );

void setup() {
  Serial1.begin( HEADSET_BAUDRATE );     // Serial port 1 (ATMEGA2560)
  Serial.begin( SERIAL_USB_BAUDRATE );   // USB Serial
  pinMode( DATA_QUALITY_LED, OUTPUT );
  headset.setOutputQualityPin( DATA_QUALITY_LED );  // Pin 13 as data quality indicator
}

void loop() {
  headset.readHeadset();
  Serial.print( headset.getQuality() );
  Serial.print(":");
  Serial.print( headset.getRaw() );
  Serial.print("\n");
}
