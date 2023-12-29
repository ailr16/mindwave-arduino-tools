/*
  Arduino Bluetooth Interface with Mindwave

  Get RAW data through serial monitor
  ** REQUIRES Arduino Mega with HC05 connected in Serial Port 1 (pins 18 and 19) **

  Lozano Ramirez Angel Ivan
  Mexico  12.28.2023
*/

#include "mindwaveTools.h"

#define DEBUGOUTPUT 0

#define HEADSET_BAUDRATE    57600
#define SERIAL_USB_BAUDRATE 115200

// checksum variables
byte generatedChecksum = 0;
byte checksum = 0;
int payloadLength = 0;
byte payloadData[64] = { 0 };
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// system variables
long lastReceivedPacket = 0;
boolean bigPacket = false;

//////////////////////////
// Microprocessor Setup //
//////////////////////////
void setup() {
  pinMode(DATA_QUALITY_LED, OUTPUT);
  Serial1.begin(HEADSET_BAUDRATE);  // Serial port 1 (ATMEGA2560)
  Serial.begin(SERIAL_USB_BAUDRATE);   // USB
}

////////////////////////////////
// Read data from Serial UART //
////////////////////////////////
byte ReadOneByte() {
  int ByteRead;
  while (!Serial1.available()) {
  }
  ByteRead = Serial1.read();

  //Serial.println(ByteRead);  // echo the same byte out the USB serial (for debug purposes)

  return ByteRead;
}

/////////////
//MAIN LOOP//
/////////////
void loop() {
  // Look for sync bytes
  if (ReadOneByte() == SYNC_BYTE_1) {
    if (ReadOneByte() == SYNC_BYTE_2) {
      payloadLength = ReadOneByte();

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
              bigPacket = true;
              break;

            case CODE_ASIC_EEG_POWER:
              // Not used yet
              if( payloadData[i + 1] == 24)
              {
                Serial.print(poorQuality);
                Serial.print(":");
                // Delta
                Serial.print(payloadData[i + 2]);
                Serial.print(",");
                Serial.print(payloadData[i + 3]);
                Serial.print(",");
                Serial.print(payloadData[i + 4]);
                Serial.print(":");
                // Theta
                Serial.print(payloadData[i + 5]);
                Serial.print(",");
                Serial.print(payloadData[i + 6]);
                Serial.print(",");
                Serial.print(payloadData[i + 7]);
                Serial.print(":");
                // Low alpha
                Serial.print(payloadData[i + 8]);
                Serial.print(",");
                Serial.print(payloadData[i + 9]);
                Serial.print(",");
                Serial.print(payloadData[i + 10]);
                Serial.print(":");
                // High alpha
                Serial.print(payloadData[i + 11]);
                Serial.print(",");
                Serial.print(payloadData[i + 12]);
                Serial.print(",");
                Serial.print(payloadData[i + 13]);
                Serial.print(":");
                // Low beta
                Serial.print(payloadData[i + 14]);
                Serial.print(",");
                Serial.print(payloadData[i + 15]);
                Serial.print(",");
                Serial.print(payloadData[i + 16]);
                Serial.print(":");
                // HHigh beta
                Serial.print(payloadData[i + 17]);
                Serial.print(",");
                Serial.print(payloadData[i + 18]);
                Serial.print(",");
                Serial.print(payloadData[i + 19]);
                Serial.print(":");
                // Low gamma
                Serial.print(payloadData[i + 20]);
                Serial.print(",");
                Serial.print(payloadData[i + 21]);
                Serial.print(",");
                Serial.print(payloadData[i + 22]);
                Serial.print(":");
                // Mid gamma
                Serial.print(payloadData[i + 23]);
                Serial.print(",");
                Serial.print(payloadData[i + 24]);
                Serial.print(",");
                Serial.print(payloadData[i + 25]);
                Serial.print("\n");
              }
              break;

            default:
              break;
          }  // switch
        }    // for loop

        
#if DEBUGOUTPUT == 0
        // *** Add your code here ***
        if (bigPacket) {
          if (poorQuality == 0) digitalWrite(DATA_QUALITY_LED, HIGH);
          else digitalWrite(DATA_QUALITY_LED, LOW);
        }
#endif

        bigPacket = false;
      } else {
        // Checksum Error
      }  // end if else for checksum
      
    }    // end if read 0xAA byte
  }      // end if read 0xAA byte
}
