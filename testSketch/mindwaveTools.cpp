#include "HardwareSerial.h"
#include <Arduino.h>
#include "mindwaveTools.h"


byte MindwaveHeadset::ReadOneByte() {
  int ByteRead;

  ByteRead = serialPort.read();
  //Serial.println(ByteRead);

  return ByteRead;
}

void MindwaveHeadset::readHeadset()
{
  if( serialPort.available() )
  {
    if (ReadOneByte() == SYNC_BYTE_1) {
      if (ReadOneByte() == SYNC_BYTE_2) {
        payloadLength = ReadOneByte();

        if( payloadLength > 169 )
        {
          return;
        }

        generatedChecksum = 0;
        for( int i = 0; i < payloadLength; i++ )
        {
          payloadData[i] = ReadOneByte();
          generatedChecksum += payloadData[i];
        }

        checksum = ReadOneByte();
        generatedChecksum = 0xFF - generatedChecksum;

        //Serial.print("CHECKSUM");
        //Serial.println(checksum);
        //Serial.print("GENERATED");
        //Serial.println(generatedChecksum);

        if( generatedChecksum == checksum )
        {
          poorQuality = 200;
          attention = 0;
          meditation = 0;

          for( int i = 0; i < payloadLength; i++ )
          {
            switch( payloadData[i] )
            {
              case CODE_SIGNAL_QUALITY:
                poorQuality = payloadData[i + 1];
                break;

              case CODE_RAW_WAVE_VALUE:
                if( payloadData[i + 1] == 2 )
                {
                  rawValue = (payloadData[i + 2] * 256) + payloadData[i + 3];
                }
                break;
            }
          }
   
        }

      }
    }
  }
}

MindwaveHeadset::MindwaveHeadset( Stream& serialPort ) : serialPort( serialPort )
{
  qualityValue = 0;
  attentionValue = 0;
  meditationValue = 0;
  rawValue = 0;
  for( int i = 0; i < 8; i++ )
  {
    allRawArray[i] = 0;
  }

  generatedChecksum = 0;
  checksum = 0;
  payloadLength = 0;
  poorQuality = 0;
  attention = 0;
  meditation = 0;
  for( int i = 0; i < 64; i++ )
  {
    payloadData[i] = 0;
  }
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
