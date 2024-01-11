////////////////////////////////////////////////////////////////////////
// Arduino Bluetooth Interface with Mindwave
//
// Sketch to test Mindwave API implementations
// 
// Mexico  01.06.2023
////////////////////////////////////////////////////////////////////////

#define DEBUGOUTPUT 0
#define SERIAL_USB_BAUDRATE 115200

#include "mindwaveTools.h"


void setup() {
  Serial.begin(SERIAL_USB_BAUDRATE);   // USB
  Serial1.begin(HEADSET_BAUDRATE);
  pinMode(13, OUTPUT);
}

// checksum variables
byte  generatedChecksum = 0;
byte  checksum = 0; 
int   payloadLength = 0;
byte  payloadData[64] = {0};
byte  poorQuality = 0;
byte  attention = 0;
byte  meditation = 0;

// system variables
long    lastReceivedPacket = 0;
boolean bigPacket = false;
long rawValue = 0;
long allRawArray[8] = {0,0,0,0,0,0,0,0};

#define DEBUGOUTPUT 0

byte ReadOneByte() {
  int ByteRead;
  while(!Serial1.available());
  ByteRead = Serial1.read();
  
  #if DEBUGOUTPUT  
    Serial.println(ByteRead);   // echo the same byte out the USB serial (for debug purposes)
  #endif
  
  return ByteRead;
}

void loop() {
  if(ReadOneByte() == 170) {
    if(ReadOneByte() == 170) {

      payloadLength = ReadOneByte();
      if(payloadLength > 169)                      //Payload length can not be greater than 169
      return;

      generatedChecksum = 0;        
      for(int i = 0; i < payloadLength; i++) {  
        payloadData[i] = ReadOneByte();            //Read payload into memory
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte();                      //Read checksum byte from stream      
      generatedChecksum = 255 - generatedChecksum;   //Take one's compliment of generated checksum

        if(checksum == generatedChecksum) {    

        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for(int i = 0; i < payloadLength; i++) {    // Parse the payload
          switch (payloadData[i]) {
          case 2:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            if( payloadData[i + 1] == 2 )
            {
              rawValue = (payloadData[i + 2] * 256) + payloadData[i + 3];
              if( (rawValue & 0x8000) == 1 )
              {
                rawValue = rawValue - 0x10000;
              }
            }
            i = i + 3;
            break;
          case 0x83:
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
            i = i + 25;   
            break;
          default:
            break;
          } // switch
        } // for loop

#if !DEBUGOUTPUT
        // *** Add your code here ***
        if(bigPacket) {
          if(poorQuality == 0)  digitalWrite(13, HIGH);
          else  digitalWrite(13, LOW);
          Serial.print("Attention: ");
          Serial.print(attention);
          Serial.print("\n");
          Serial.print("Delta: ");
          Serial.print(allRawArray[ALLRAW_OUTPUT_DELTA_INDEX]);
          Serial.print("\n");
        }
        else {
          Serial.print("Raw: ");
          Serial.print(rawValue);
          Serial.print("\n");
        }
#endif        
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  // end if else for checksum
    } // end if read 0xAA byte
  } // end if read 0xAA byte
}
