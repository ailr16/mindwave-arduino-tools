#include "HardwareSerial.h"
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

        Serial.print("CHECKSUM");
        Serial.println(checksum);
        Serial.print("GENERATED");
        Serial.println(generatedChecksum);

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
                //Serial.println("QUALITY");
                break;
              
              case CODE_ATTENTION:
                attention = payloadData[i + 1];
                //Serial.println("ATTENTION");
                break;
              
              case CODE_MEDITATION:
                meditation = payloadData[i + 1];
                //Serial.println("MEDITATION");
                break;

              case CODE_RAW_WAVE_VALUE:
                if( payloadData[i + 1] == 2 )
                {
                  rawValue = (payloadData[i + 2] * 256) + payloadData[i + 3];
                  if( (rawValue & 0x8000) == 1 )
                  {
                    rawValue = rawValue - 0x10000;
                  }
                }
                //Serial.println("RAW");
                break;

              case CODE_ASIC_EEG_POWER:
                if( payloadData[i + 1] == 24 )
                {
                  allRawArray[ALLRAW_OUTPUT_DELTA_INDEX]      = (payloadData[i + 2] << 16) + (payloadData[i + 3] << 8) + payloadData[i + 4];
                  allRawArray[ALLRAW_OUTPUT_THETA_INDEX]      = (payloadData[i + 5] << 16) + (payloadData[i + 6] << 8) + payloadData[i + 7];
                  allRawArray[ALLRAW_OUTPUT_LOW_APLHA_INDEX]  = (payloadData[i + 8] << 16) + (payloadData[i + 9] << 8) + payloadData[i + 10];
                  allRawArray[ALLRAW_OUTPUT_HIGH_APLHA_INDEX] = (payloadData[i + 11] << 16) + (payloadData[i + 12] << 8) + payloadData[i + 13];
                  allRawArray[ALLRAW_OUTPUT_LOW_BETA_INDEX]   = (payloadData[i + 14] << 16) + (payloadData[i + 15] << 8) + payloadData[i + 16];
                  allRawArray[ALLRAW_OUTPUT_HIGH_BETA_INDEX]  = (payloadData[i + 17] << 16) + (payloadData[i + 18] << 8) + payloadData[i + 19];
                  allRawArray[ALLRAW_OUTPUT_LOW_GAMMA_INDEX]  = (payloadData[i + 20] << 16) + (payloadData[i + 21] << 8) + payloadData[i + 22];
                  allRawArray[ALLRAW_OUTPUT_MID_GAMMA_INDEX]  = (payloadData[i + 23] << 16) + (payloadData[i + 24] << 8) + payloadData[i + 25];
                }
                //Serial.println("ASIC");
                break;

              default:
                break;
            }
          }
        }

        if( poorQuality == 0 )
        {
          digitalWrite(13, HIGH);
        }
        else {
          digitalWrite(13, LOW);
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
  return attentionValue;
}

unsigned int MindwaveHeadset::getMeditation()
{
  return meditationValue;
}

int MindwaveHeadset::getRaw()
{
  return rawValue;
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
