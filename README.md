# LibLanc 

This Arudino library allows to control a [Sony Lanc](http://www.boehmel.de/lanc) enabled camera.

Since the Protocol has very strict timing requirements, the application should normally block during transmission (see `LancBlocking` class). This class is designed so that it loops until a complete communication exchange is completed. This is triggered by calling the `loop` function.

There is a non blocking variant of the Library available (see `LancNonBlocking` class). This variant simply checks whether there is anything to do and immediately returns. Be aware that you must call `loop` very quickly. It is recommended to call `loop` at least every 25us.

## Example usage

The following example shows how `LancBlocking` could be used:

```c++
#include <LibLanc.h>

#define LANC_INPUT_PIN  2
#define LANC_OUTPUT_PIN  3
LancBlocking lanc(LANC_INPUT_PIN, LANC_OUTPUT_PIN);

void setup() {
    lanc.begin();
}

void loop() {
    // get next command to execute
    // call lanc.Zoom(value);
    if(!lanc.loop()){
        // Run the lanc.loop(),  if the loop fails to run a false state is returned and the lanc library stops working until the lanc.begin(); is called again
        // reasons for failure might be that the camera was turned off, or cable is disconnected.   The library has a default blocking timeout of 100ms. 
    }
}
```

For more examples see: [examples](examples)
