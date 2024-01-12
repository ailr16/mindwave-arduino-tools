////////////////////////////////////////////////////////////////////////
// Arduino Bluetooth Interface with Mindwave
//
// Sketch to test Mindwave API implementations
// Tested on Arduino UNO using HC05
// Control Led instensity with PWM and attention level
//
// https://github.com/ailr16/mindwave-arduino-tools
// 
// Mexico  01.12.2024
////////////////////////////////////////////////////////////////////////

#define PIN_QUALITY_INDICATOR 13
#define LED_PWM_PIN 11

#include "mindwaveTools.h"

MindwaveHeadset headset( Serial );

void setup() {
  Serial.begin(HEADSET_BAUDRATE);
  pinMode(PIN_QUALITY_INDICATOR, OUTPUT);
  pinMode(LED_PWM_PIN, OUTPUT);
  headset.setOutputQualityPin( PIN_QUALITY_INDICATOR );
}


void loop() {
  headset.readHeadset();
  analogWrite(LED_PWM_PIN, headset.getAttention());
}
