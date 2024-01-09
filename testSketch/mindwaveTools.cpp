#include <Arduino.h>
#include "mindwaveTools.h"


byte MindwaveHeadset::ReadOneByte() {
  int ByteRead;

  ByteRead = serialPort.read();
  Serial.println(ByteRead);

  return ByteRead;
}

void MindwaveHeadset::readHeadset()
{
  ReadOneByte();
}

MindwaveHeadset::MindwaveHeadset( Stream& serialPort ) : serialPort( serialPort )
{
  
}

void MindwaveHeadset::init()
{
  
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
