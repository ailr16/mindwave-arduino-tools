/*
  Arduino Bluetooth Interface with Mindwave

  Get ALL RAW data through serial monitor
  ** REQUIRES Arduino Mega with HC05 connected in Serial Port 1 (pins 18 and 19) **
  ** After some tests, sending all this data thru serial show some errors like halting,
  so use for logging less data**

  Lozano Ramirez Angel Ivan
  Mexico  01.12.2024
*/

#include <mindwaveTools.h>

#define SERIAL_USB_BAUDRATE 115200
#define DATA_QUALITY_LED 13

MindwaveHeadset headset( Serial1 );

void setup() {
  Serial1.begin(HEADSET_BAUDRATE);     // Serial port 1 (ATMEGA2560)
  Serial.begin(SERIAL_USB_BAUDRATE);   // USB Serial
  pinMode(DATA_QUALITY_LED, OUTPUT);
  headset.setOutputQualityPin( DATA_QUALITY_LED );  // Pin 13 as data quality indicator
}

void loop() {
  headset.readHeadset();

  Serial.print( headset.getQuality() );
  Serial.print( ":" );
  // Delta
  Serial.print( headset.getRawDelta() );
  Serial.print( ":" );
  // Theta
  Serial.print( headset.getRawTheta() );
  Serial.print( ":" );
  // Low alpha
  Serial.print( headset.getRawLowAlpha() );
  Serial.print( ":" );
  // High alpha
  Serial.print( headset.getRawHighAlpha() );
  Serial.print( ":" );
  // Low beta
  Serial.print( headset.getRawLowBeta() );
  Serial.print( ":" );
  // High beta
  Serial.print( headset.getRawHighBeta() );
  Serial.print( ":" );
  // Low gamma
  Serial.print( headset.getRawLowGamma() );
  Serial.print( ":" );
  // Mid gamma
  Serial.print( headset.getRawMidGamma() );
  Serial.print( "\n" );
}
