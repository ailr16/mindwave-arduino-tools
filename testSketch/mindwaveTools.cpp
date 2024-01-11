#include "HardwareSerial.h"
#include <Arduino.h>
#include "mindwaveTools.h"

byte MindwaveHeadset::ReadOneByte() {
  int ByteRead;

  while( !Serial1.available() );
  ByteRead = Serial1.read();

  #if DEBUG_USB_SERIAL == true
  Serial.println(ByteRead);
  #endif 
  
  return ByteRead;
}

void MindwaveHeadset::readHeadset()
{
  if( ReadOneByte() == SYNC_BYTE_1 ) {
    if(ReadOneByte() == SYNC_BYTE_2) {

      payloadLength = ReadOneByte();
      if( payloadLength > 169 )
      {
        // Invalid length
        return;
      }

      generatedChecksum = 0;
      for( int i = 0; i < payloadLength; i++ ) {  
        payloadData[ i ] = ReadOneByte();
        generatedChecksum += payloadData[ i ];
      }   

      checksum = ReadOneByte();
      generatedChecksum = 255 - generatedChecksum;

      if(checksum == generatedChecksum) {

        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for( int i = 0; i < payloadLength; i++ ){
          switch (payloadData[i]) {
            case CODE_SIGNAL_QUALITY:
              i++;            
              poorQuality = payloadData[i];
              qualityValue = poorQuality;
              bigPacket = true;            
              break;

            case CODE_ATTENTION:
              i++;
              attention = payloadData[i];                        
              break;

            case CODE_MEDITATION:
              i++;
              meditation = payloadData[i];
              break;

            case CODE_RAW_WAVE_VALUE:
              #if ENABLE_RAW == true
              if( payloadData[i + 1] == 2 )
              {
                rawValue = (payloadData[ i + 2 ] << 8) | payloadData[i + 3];
              }
              #endif
              i = i + 3;
              break;

            case CODE_ASIC_EEG_POWER:
              #if ENABLE_ALL_RAW == true
              if( payloadData[i + 1] == 24 )
              {
                allRawArray[ ALLRAW_OUTPUT_DELTA_INDEX ]      = (payloadData[i + 2] << 16) + (payloadData[i + 3] << 8) + payloadData[i + 4];
                allRawArray[ ALLRAW_OUTPUT_THETA_INDEX ]      = (payloadData[i + 5] << 16) + (payloadData[i + 6] << 8) + payloadData[i + 7];
                allRawArray[ ALLRAW_OUTPUT_LOW_APLHA_INDEX ]  = (payloadData[i + 8] << 16) + (payloadData[i + 9] << 8) + payloadData[i + 10];
                allRawArray[ ALLRAW_OUTPUT_HIGH_APLHA_INDEX ] = (payloadData[i + 11] << 16) + (payloadData[i + 12] << 8) + payloadData[i + 13];
                allRawArray[ ALLRAW_OUTPUT_LOW_BETA_INDEX ]   = (payloadData[i + 14] << 16) + (payloadData[i + 15] << 8) + payloadData[i + 16];
                allRawArray[ ALLRAW_OUTPUT_HIGH_BETA_INDEX ]  = (payloadData[i + 17] << 16) + (payloadData[i + 18] << 8) + payloadData[i + 19];
                allRawArray[ ALLRAW_OUTPUT_LOW_GAMMA_INDEX ]  = (payloadData[i + 20] << 16) + (payloadData[i + 21] << 8) + payloadData[i + 22];
                allRawArray[ ALLRAW_OUTPUT_MID_GAMMA_INDEX ]  = (payloadData[i + 23] << 16) + (payloadData[i + 24] << 8) + payloadData[i + 25];
              }
              #endif
              i = i + 25;   
              break;

            default:
              break;
          }
        }

        if(bigPacket) {
          if(poorQuality == 0)  digitalWrite(13, HIGH);
          else  digitalWrite(13, LOW);
          attentionValue = attention;
          meditationValue = meditation;
          
          #if ENABLE_ALL_RAW == true
          deltaValue =     allRawArray[ ALLRAW_OUTPUT_DELTA_INDEX ];
          thetaValue =     allRawArray[ ALLRAW_OUTPUT_THETA_INDEX ];
          lowAlphaValue =  allRawArray[ ALLRAW_OUTPUT_LOW_APLHA_INDEX ];
          highAlphaValue = allRawArray[ ALLRAW_OUTPUT_HIGH_APLHA_INDEX ];
          lowBetaValue =   allRawArray[ ALLRAW_OUTPUT_LOW_BETA_INDEX ];
          highBetaValue =  allRawArray[ ALLRAW_OUTPUT_HIGH_BETA_INDEX ];
          lowGammaValue =  allRawArray[ ALLRAW_OUTPUT_LOW_GAMMA_INDEX ];
          midGammaValue =  allRawArray[ ALLRAW_OUTPUT_MID_GAMMA_INDEX ];
          #endif
        }

        bigPacket = false;
      }
      else {
        //Handle a checksum error here
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

unsigned int MindwaveHeadset::getQuality()
{
  return qualityValue;
}

unsigned int MindwaveHeadset::getAttention()
{
  return attentionValue;
}

unsigned int MindwaveHeadset::getMeditation()
{
  return meditationValue;
}

#if ENABLE_RAW == true
int MindwaveHeadset::getRaw()
{
  return rawValue;
}
#endif

#if ENABLE_ALL_RAW == true
long MindwaveHeadset::getRawDelta( void )
{
  return deltaValue;
}

long MindwaveHeadset::getRawTheta( void )
{
  return thetaValue;
}

long MindwaveHeadset::getRawLowAlpha( void )
{
  return lowAlphaValue;
}

long MindwaveHeadset::getRawHighAlpha( void )
{
  return highAlphaValue;
}

long MindwaveHeadset::getRawLowBeta( void )
{
  return lowBetaValue;
}

long MindwaveHeadset::getRawHighBeta( void )
{
  return highBetaValue;
}

long MindwaveHeadset::getRawLowGamma( void )
{
  return lowGammaValue;
}

long MindwaveHeadset::getRawMidGamma( void )
{
  return midGammaValue;
}

void MindwaveHeadset::getAllRaw(long *allRawArrayOutput)
{
  allRawArrayOutput[ ALLRAW_OUTPUT_DELTA_INDEX ]      = this->allRawArray[ ALLRAW_OUTPUT_DELTA_INDEX ];
  allRawArrayOutput[ ALLRAW_OUTPUT_THETA_INDEX ]      = this->allRawArray[ ALLRAW_OUTPUT_THETA_INDEX ];
  allRawArrayOutput[ ALLRAW_OUTPUT_LOW_APLHA_INDEX ]  = this->allRawArray[ ALLRAW_OUTPUT_LOW_APLHA_INDEX ];
  allRawArrayOutput[ ALLRAW_OUTPUT_HIGH_APLHA_INDEX ] = this->allRawArray[ ALLRAW_OUTPUT_HIGH_APLHA_INDEX ];
  allRawArrayOutput[ ALLRAW_OUTPUT_LOW_BETA_INDEX ]   = this->allRawArray[ ALLRAW_OUTPUT_LOW_BETA_INDEX ];
  allRawArrayOutput[ ALLRAW_OUTPUT_HIGH_BETA_INDEX ]  = this->allRawArray[ ALLRAW_OUTPUT_HIGH_BETA_INDEX ];
  allRawArrayOutput[ ALLRAW_OUTPUT_LOW_GAMMA_INDEX ]  = this->allRawArray[ ALLRAW_OUTPUT_LOW_GAMMA_INDEX ];
  allRawArrayOutput[ ALLRAW_OUTPUT_MID_GAMMA_INDEX ]  = this->allRawArray[ ALLRAW_OUTPUT_MID_GAMMA_INDEX ];
}
#endif
