#ifndef MINDWAVE_TOOLS_H
#define MINDWAVE_TOOLS_H

#include <Arduino.h>
#include "ConfigAPI.h"

#define SYNC_BYTE_1   0xAA
#define SYNC_BYTE_2   0xAA

#define HEADSET_BAUDRATE    57600

#define CODE_SIGNAL_QUALITY 0x02
#define CODE_ATTENTION      0x04
#define CODE_MEDITATION     0x05
#define CODE_RAW_WAVE_VALUE 0x80
#define CODE_ASIC_EEG_POWER 0x83

#define ALLRAW_OUTPUT_DELTA_INDEX      0
#define ALLRAW_OUTPUT_THETA_INDEX      1
#define ALLRAW_OUTPUT_LOW_APLHA_INDEX  2
#define ALLRAW_OUTPUT_HIGH_APLHA_INDEX 3
#define ALLRAW_OUTPUT_LOW_BETA_INDEX   4
#define ALLRAW_OUTPUT_HIGH_BETA_INDEX  5
#define ALLRAW_OUTPUT_LOW_GAMMA_INDEX  6
#define ALLRAW_OUTPUT_MID_GAMMA_INDEX  7


class MindwaveHeadset
{
  private:
    /* General attributes */
    Stream& serialPort;
    unsigned int qualityValue;
    unsigned int attentionValue;
    unsigned int meditationValue;
    long deltaValue;
    long thetaValue;
    long lowAlphaValue; 
    long highAlphaValue;
    long lowBetaValue; 
    long highBetaValue;
    long lowGammaValue; 
    long midGammaValue;
    int rawValue;
    long allRawArray[8];
    byte ReadOneByte(void);

    /* Attributes used by reading methods */
    byte generatedChecksum;
    byte checksum;
    int  payloadLength;
    byte payloadData[64];
    byte poorQuality;
    byte attention;
    byte meditation;
    boolean bigPacket;

  public:
    MindwaveHeadset( void );
    MindwaveHeadset( Stream& serialPort );
    void readHeadset( void );
    unsigned int getQuality( void );

    #if ENABLE_ATTENTION == true
    unsigned int getAttention( void );
    #endif

    #if ENABLE_MEDITATION == true
    unsigned int getMeditation( void );
    #endif

    #if ENABLE_RAW == true
    int  getRaw( void );
    #endif

    #if ENABLE_ALL_RAW == true
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
