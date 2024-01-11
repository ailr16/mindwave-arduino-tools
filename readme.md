# Tools for Arduino - Neurosky Mindwave

Welcome, here you'll find an API to make easier working with Nerusoky Mindwave and Arduino boards!

**Please refer to this document, is basic to understand the Payload data:**  
https://developer.neurosky.com/docs/lib/exe/fetch.php?media=mindset_communications_protocol.pdf

**WORK IN PROGRESS**

## Interfaces

#### readHeadset()
Run as fast as possible to avoid data loss (in the actual tests is called inside loop())

#### getAttention()
Get the atttention value. This value is updated tipically every second

#### getMeditation()
Get the meditation value. This value is updated tipically every second

#### getRaw()
Get raw wave value. This value is updated tipically 2ms

#### getAllRaw()
Get all posible raw values (Delta, Theta, Beta, and Gamma waves). This values are updated tipically every second
