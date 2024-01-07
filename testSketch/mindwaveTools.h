#ifndef MINDWAVE_TOOLS_H
#define MINDWAVE_TOOLS_H

#define SYNC_BYTE_1   0xAA
#define SYNC_BYTE_2   0xAA

#define HEADSET_BAUDRATE    57600u

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

#include <HardwareSerial.h>

class MindwaveHeadset
{
  private:
    unsigned int qualityValue;
    unsigned int attentionValue;
    unsigned int meditationValue;
    int rawValue;
    long allRawArray[8];

  public:
    MindwaveHeadset( void );
    unsigned int getAttention( void );
    unsigned int getMeditation( void );
    int getRaw( void );
    void getAllRaw( long *allRawArray );
    HardwareSerial serialPort;
};

#endif
