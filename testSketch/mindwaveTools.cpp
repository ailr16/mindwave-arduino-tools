#include <Arduino.h>
#include "HardwareSerial.h"
#include "mindwaveTools.h"


byte MindwaveHeadset::ReadOneByte() {
  int ByteRead;

  ByteRead = Serial1.read();

  return ByteRead;
}

void MindwaveHeadset::readHeadset()
{
  // Look for sync bytes
  if (ReadOneByte() == SYNC_BYTE_1) {
    if (ReadOneByte() == SYNC_BYTE_2) {
      payloadLength = this->ReadOneByte();

      if (payloadLength > 169)  //Payload length can not be greater than 169
        return;

      generatedChecksum = 0;
      for (int i = 0; i < payloadLength; i++) {
        payloadData[i] = ReadOneByte();  //Read payload into memory
        generatedChecksum += payloadData[i];
      }

      checksum = ReadOneByte();                      //Read checksum byte from stream
      generatedChecksum = 0xFF - generatedChecksum;  //Take one's compliment of generated checksum

      if (checksum == generatedChecksum) {
        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for (int i = 0; i < payloadLength; i++) {  // Parse the payload
          switch (payloadData[i]) {
            case CODE_SIGNAL_QUALITY:
              poorQuality = payloadData[i + 1];
              break;

            case CODE_RAW_WAVE_VALUE:
              if( payloadData[i + 1] == 2)
              {
                Serial.print(poorQuality);
                Serial.print(":");
                Serial.print( payloadData[i + 2] );
                Serial.print( ":" );
                Serial.print( payloadData[i + 3] );
                Serial.print("\n");
              }
              break;

            default:
              break;
          }
        }
      }
    }    // end if read 0xAA byte
  }      // end if read 0xAA byte
}

MindwaveHeadset::MindwaveHeadset( HardwareSerial& serialPort ) : serialPort( serialPort )
{
  this->serialPort.begin( HEADSET_BAUDRATE );
}

void MindwaveHeadset::init()
{
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