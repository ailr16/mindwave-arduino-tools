/**
 * @file  mindwaveTools.cpp
 * @brief This file contains the API source code (methods definition)
 */

#include "HardwareSerial.h"
#include <Arduino.h>
#include "mindwaveTools.h"


/**
 * @brief Read a byte from serial port
 */
byte MindwaveHeadset::ReadOneByte() {
  int ByteRead;

  while( !serialPort.available() );
  ByteRead = serialPort.read();

  #if DEBUG_USB_SERIAL
  Serial.println(ByteRead);
  #endif 
  
  return ByteRead;
}


/**
 * @brief Constructor with serial port used by headset and pin used as quality indicator
 * @param serialPort Serial port which communicates with headset
 */
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


#if ENABLE_QUALITY_INDICATOR
/**
 * @brief Set the pin used as output indicator for data quality
 * @param pin Pin used as output
 */
void MindwaveHeadset::setOutputQualityPin(unsigned char pin)
{
  this->qualityIndicatorPin = pin;
}
#endif


/**
 * @brief Receive data from headset and parse it
 * NOTE: Run as fast as possible for avoid data loss
 */
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

              #if ENABLE_ATTENTION == true
              attention = payloadData[i];
              #endif

              break;

            case CODE_MEDITATION:
              i++;

              #if ENABLE_MEDITATION == true
              meditation = payloadData[i];
              #endif

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
          #if ENABLE_QUALITY_INDICATOR
            if(poorQuality == 0)
            {
              digitalWrite( qualityIndicatorPin , HIGH);
            }
            else
            {
              digitalWrite( qualityIndicatorPin, LOW);
            }
          #endif

          #if ENABLE_ATTENTION
          attentionValue = attention;
          #endif

          #if ENABLE_MEDITATION
          meditationValue = meditation;
          #endif
          
          #if ENABLE_ALL_RAW
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


/**
 * @brief Get the last data quality received value
 * @retval Data quality value as unsigned int (possible values 0 to 200)
 */
unsigned int MindwaveHeadset::getQuality()
{
  return qualityValue;
}


#if ENABLE_ATTENTION
/**
 * @brief Get the last attention received value
 * @retval Attention value as unsigned int (possible values 0 to 100)
 */
unsigned int MindwaveHeadset::getAttention()
{
  return attentionValue;
}
#endif


#if ENABLE_MEDITATION
/**
 * \@brief Get the last meditation received value
 * \@retval Meditation value as unsigned int (possible values 0 to 100)
 */
unsigned int MindwaveHeadset::getMeditation()
{
  return meditationValue;
}
#endif


#if ENABLE_RAW
/**
 * @brief Get the last raw received value
 * @retval Raw wave value as int (possible values in the int range (-32768 to 32767))
 */
int MindwaveHeadset::getRaw()
{
  return rawValue;
}
#endif


#if ENABLE_ALL_RAW
/**
 * @brief Get the last raw delta received value
 * @retval Raw delta value as long
 */
long MindwaveHeadset::getRawDelta( void )
{
  return deltaValue;
}


/**
 * @brief Get the last raw theta received value
 * @retval Raw tetha value as long
 */
long MindwaveHeadset::getRawTheta( void )
{
  return thetaValue;
}


/**
 * @brief Get the last raw low alpha received value
 * @retval Raw low alpha value as long
 */
long MindwaveHeadset::getRawLowAlpha( void )
{
  return lowAlphaValue;
}


/**
 * @brief Get the last raw high alpha received value
 * @retval Raw high alpha value as long
 */
long MindwaveHeadset::getRawHighAlpha( void )
{
  return highAlphaValue;
}


/**
 * @brief Get the last raw low beta received value
 * @retval Raw low beta value as long
 */
long MindwaveHeadset::getRawLowBeta( void )
{
  return lowBetaValue;
}


/**
 * @brief Get the last raw high beta received value
 * @retval Raw high beta value as long
 */
long MindwaveHeadset::getRawHighBeta( void )
{
  return highBetaValue;
}


/**
 * @brief Get the last raw low gamma received value
 * @retval Raw low gamma value as long
 */
long MindwaveHeadset::getRawLowGamma( void )
{
  return lowGammaValue;
}


/**
 * @brief Get the last raw low gamma received value
 * @retval Raw mid gamma value as long
 */
long MindwaveHeadset::getRawMidGamma( void )
{
  return midGammaValue;
}


/**
 * @brief Get all the raw values into an array
 * @retval All raw values as long
 */
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
