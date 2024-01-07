#include <Arduino.h>
#include "HardwareSerial.h"
#include "mindwaveTools.h"

MindwaveHeadset::MindwaveHeadset( HardwareSerial serialPort ) : serialPort( serialPort )
{
  this->serialPort.begin( HEADSET_BAUDRATE );
  this->initTimer();
}

unsigned int MindwaveHeadset::getAttention()
{
  return this->attentionValue;
}

unsigned int MindwaveHeadset::getMeditation()
{
  return this->meditationValue;
}

int MindwaveHeadset::getRaw()
{
  return this->rawValue;
}

void MindwaveHeadset::getAllRaw(long *allRawArray)
{
  allRawArray[ALLRAW_OUTPUT_DELTA_INDEX]      = this->allRawArray[0];
  allRawArray[ALLRAW_OUTPUT_THETA_INDEX]      = this->allRawArray[1];
  allRawArray[ALLRAW_OUTPUT_LOW_APLHA_INDEX]  = this->allRawArray[2];
  allRawArray[ALLRAW_OUTPUT_HIGH_APLHA_INDEX] = this->allRawArray[3];
  allRawArray[ALLRAW_OUTPUT_LOW_BETA_INDEX]   = this->allRawArray[4];
  allRawArray[ALLRAW_OUTPUT_HIGH_BETA_INDEX]  = this->allRawArray[5];
  allRawArray[ALLRAW_OUTPUT_LOW_GAMMA_INDEX]  = this->allRawArray[6];
  allRawArray[ALLRAW_OUTPUT_MID_GAMMA_INDEX]  = this->allRawArray[7];
}

void MindwaveHeadset::initTimer()
{
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 1999;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12);
  TCCR1B |= (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

uint8_t flag = 0;

ISR(TIMER1_COMPA_vect)
{
  flag ^= 1;
  digitalWrite(13, flag);
}