/**
 * @file  mindwaveTools.h
 * @brief This file contains macros and declarations used by API
 */

#ifndef MINDWAVE_TOOLS_H
#define MINDWAVE_TOOLS_H

#include <Arduino.h>
#include "ConfigAPI.h"


#define SYNC_BYTE_1   0xAA          /*!< First byte read for indentify package  */
#define SYNC_BYTE_2   0xAA          /*!< Second byte read for indentify package */

#define HEADSET_BAUDRATE    57600   /*!< Headset bluetooth baudrate is 57600 */

#define CODE_SIGNAL_QUALITY 0x02    /*!< Indicates that data quality has been received */
#define CODE_ATTENTION      0x04    /*!< Indicates that attention has been received    */
#define CODE_MEDITATION     0x05    /*!< Indicates that meditation has been received   */
#define CODE_RAW_WAVE_VALUE 0x80    /*!< Indicates that raw value has been received    */
#define CODE_ASIC_EEG_POWER 0x83    /*!< Indicates that all raw data has been received */

#define ALLRAW_OUTPUT_DELTA_INDEX      0  /*!< Array index for raw delta value      */
#define ALLRAW_OUTPUT_THETA_INDEX      1  /*!< Array index for raw theta value      */
#define ALLRAW_OUTPUT_LOW_APLHA_INDEX  2  /*!< Array index for raw low alpha value  */
#define ALLRAW_OUTPUT_HIGH_APLHA_INDEX 3  /*!< Array index for raw high alpha value */
#define ALLRAW_OUTPUT_LOW_BETA_INDEX   4  /*!< Array index for raw low beta value   */
#define ALLRAW_OUTPUT_HIGH_BETA_INDEX  5  /*!< Array index for raw high beta value  */
#define ALLRAW_OUTPUT_LOW_GAMMA_INDEX  6  /*!< Array index for raw low gamma value  */
#define ALLRAW_OUTPUT_MID_GAMMA_INDEX  7  /*!< Array index for raw mida gamme value */


/**
 * @brief MindwaveHeadset class for handling a neurosky mindwave headset
 */
class MindwaveHeadset
{
  private:
    /* General attributes */
    Stream& serialPort;
    unsigned int qualityValue;

    #if ENABLE_ATTENTION
    unsigned int attentionValue;
    #endif

    #if ENABLE_MEDITATION
    unsigned int meditationValue;
    #endif

    #if ENABLE_RAW
    int rawValue;
    #endif

    #if ENABLE_ALL_RAW
    long deltaValue;
    long thetaValue;
    long lowAlphaValue; 
    long highAlphaValue;
    long lowBetaValue; 
    long highBetaValue;
    long lowGammaValue; 
    long midGammaValue;
    long allRawArray[8];
    #endif

    unsigned char qualityIndicatorPin;

    /* Attributes used by reading methods */
    byte generatedChecksum;
    byte checksum;
    int  payloadLength;
    byte payloadData[64];
    byte poorQuality;
    byte attention;
    byte meditation;
    boolean bigPacket;

    byte ReadOneByte(void);

  public:
    MindwaveHeadset( Stream& serialPort );

    #if ENABLE_QUALITY_INDICATOR
    void setOutputQualityPin( unsigned char pin );
    #endif
    
    void readHeadset( void );
    unsigned int getQuality( void );

    #if ENABLE_ATTENTION
    unsigned int getAttention( void );
    #endif

    #if ENABLE_MEDITATION
    unsigned int getMeditation( void );
    #endif

    #if ENABLE_RAW
    int  getRaw( void );
    #endif

    #if ENABLE_ALL_RAW
    long getRawDelta( void );
    long getRawTheta( void );
    long getRawLowAlpha( void );
    long getRawHighAlpha( void );
    long getRawLowBeta( void );
    long getRawHighBeta( void );
    long getRawLowGamma( void );
    long getRawMidGamma( void );
    void getAllRaw( long *allRawArray );
    #endif
};

#endif
