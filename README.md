# Tools for Arduino - Neurosky Mindwave

Welcome, here you'll find an API to make easier working with Nerusoky Mindwave and Arduino boards!

**Please refer to this document, is basic to understand the Payload data:**  
https://developer.neurosky.com/docs/lib/exe/fetch.php?media=mindset_communications_protocol.pdf


## Interfaces

### void readHeadset( void )
Run as fast as possible to avoid data loss (in the current tests is called inside loop())

### void setOutputQualityPin( unsigned char pin )
Set pin as output for indicating the data quality (as a led)

### unsigned int getQuality( void )
Get the current quiality value, a 0 value indicates good quality data, otherwise a higher value means poor quality

### unsigned int getAttention( void )
Get the atttention value. This value is updated tipically every second

### unsigned int getMeditation( void )
Get the meditation value. This value is updated tipically every second

### int getRaw( void )
Get raw wave value. This value is updated tipically 2ms

### long getRawDelta( void )
Get the Delta current value (could be polled as fast as possible, but updates aproximately once a second)

### long getRawTheta( void )
Get the Theta current value (could be polled as fast as possible, but updates aproximately once a second)

### long getRawLowAlpha( void )
Get the Low Alpha current value (could be polled as fast as possible, but updates aproximately once a second)

### long getRawHighAlpha( void )
Get the High Alpha current value (could be polled as fast as possible, but updates aproximately once a second)

### long getRawLowBeta( void )
Get the Low Beta current value (could be polled as fast as possible, but updates aproximately once a second)

### long getRawHighBeta( void )
Get the High Beta current value (could be polled as fast as possible, but updates aproximately once a second)

### long getRawLowGamma( void )
Get the Low Gamma current value (could be polled as fast as possible, but updates aproximately once a second)

### long getRawMidGamma( void )
Get the Mid Gamma current value (could be polled as fast as possible, but updates aproximately once a second)

### void getAllRaw( long *allRawArray )
Get all the raw values (Delta, Theta, Beta and Gamma) in an array (only needs a call to retrieve all the raw values).
This values are updated tipically every second

## Configure the API
Use the file *ConfigAPI.h* to enable/disable features:
- Set DEBUG_USB_SERIAL to 1 for debugging using *Serial*, **This means that headset can't be connected to this port**, or set to 0 for disable debugging.
- Set ENABLE_ATTENTION to 1 when attention values will be read, or set to 0 to disable
- Set ENABLE_MEDITATION to 1 when meditation values will be read, or set to 0 to disable
- Set ENABLE_RAW to 1 when raw values will be read, or set to 0 to disable
- Set ENABLE_ALL_RAW to 1 when all the raw values will be read, or set to 0 to disable
- Set ENABLE_QUALITY_INDICATOR to 1 when a pin will be used as output indicating the data quality, or set to 0 to disable

*Using intellisense will show the enabled features while developing the code*
*Disabling unused features will reduce the memory usage*