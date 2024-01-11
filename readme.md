# Tools for Arduino - Neurosky Mindwave

Welcome, here you'll find an API to make easier working with Nerusoky Mindwave and Arduino boards!

**Please refer to this document, is basic to understand the Payload data:**  
https://developer.neurosky.com/docs/lib/exe/fetch.php?media=mindset_communications_protocol.pdf

**WORK IN PROGRESS**

## Interfaces

### void readHeadset( void )
Run as fast as possible to avoid data loss (in the current tests is called inside loop())

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
