#include <Arduino.h>
#include "HardwareSerial.h"
#include "mindwaveTools.h"


byte MindwaveHeadset::ReadOneByte() {
  int ByteRead;

  ByteRead = Serial1.read();
  Serial.println(ByteRead);

  return ByteRead;
}

void MindwaveHeadset::readHeadset()
{
  if(this->serialPort.available())
  {
    // Look for sync bytes
    if (this->ReadOneByte() == SYNC_BYTE_1) {
      digitalWrite(13, HIGH);
      if (this->ReadOneByte() == SYNC_BYTE_2) {
        this->payloadLength = this->ReadOneByte();

        if (this->payloadLength > 169)  //Payload length can not be greater than 169
          return;

        this->generatedChecksum = 0;
        for (int i = 0; i < this->payloadLength; i++) {
          this->payloadData[i] = this->ReadOneByte();  //Read payload into memory
          this->generatedChecksum += this->payloadData[i];
        }

        this->checksum = ReadOneByte();                      //Read checksum byte from stream
        this->generatedChecksum = 0xFF - this->generatedChecksum;  //Take one's compliment of generated checksum

        if (this->checksum == this->generatedChecksum) {
          this->poorQuality = 200;
          this->attention = 0;
          this->meditation = 0;

          for (int i = 0; i < this->payloadLength; i++) {  // Parse the payload
            switch (this->payloadData[i]) {
              case CODE_SIGNAL_QUALITY:
                this->poorQuality = this->payloadData[i + 1];
                break;

              case CODE_RAW_WAVE_VALUE:
                if( this->payloadData[i + 1] == 2)
                {
                  this->rawValue = this->payloadData[i + 2] * 256 + this->payloadData[i + 3];
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
}

MindwaveHeadset::MindwaveHeadset( HardwareSerial serialPort ) : serialPort( serialPort )
{
  
}

void MindwaveHeadset::init()
{
  this->serialPort.begin( HEADSET_BAUDRATE );
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
