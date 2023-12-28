////////////////////////////////////////////////////////////////////////
// Arduino Bluetooth Interface with Mindwave
//
// This is example code provided by NeuroSky, Inc. and is provided
// license free.
//
// This modification allows view data trough serial monitor
// Lozano Ramirez Angel Ivan
// Mexico  2.07.2021
////////////////////////////////////////////////////////////////////////

#define DEBUGOUTPUT 0

#define DATA_QUALITY_LED    13
#define HEADSET_BAUDRATE    57600
#define SERIAL_USB_BAUDRATE 57600

#define SYNC_BYTE_1   0xAA
#define SYNC_BYTE_2   0xAA

#define CODE_SIGNAL_QUALITY 0x02
#define CODE_ATTENTION      0x04
#define CODE_MEDITATION     0x05
#define CODE_RAW_WAVE_VALUE 0x80
#define CODE_ASIC_EEG_POWER 0x83

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
              i++;
              poorQuality = payloadData[i];
              bigPacket = true;
              break;

            case CODE_ATTENTION:
              i++;
              attention = payloadData[i];
              Serial.print("ATT ");
              Serial.println(attention);
              break;

            case CODE_MEDITATION:
              i++;
              meditation = payloadData[i];
              break;

            case CODE_RAW_WAVE_VALUE:
              // Not used yet
              i = i + 3;
              break;

            case CODE_ASIC_EEG_POWER:
              // Not used yet
              i = i + 25;
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
          Serial.println("Attention: ");
          Serial.println(attention);
        }
#endif

        bigPacket = false;
      } else {
        // Checksum Error
      }  // end if else for checksum
      
    }    // end if read 0xAA byte
  }      // end if read 0xAA byte
}
