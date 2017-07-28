#include <Arduino.h>
#include <Keyboard.h>
#include <Mouse.h>

#include "automat.h"

// Accepts remote procedure calls via the serial communication line and
// delegates the request to the appropriate method.
AutomatController controller(&Serial);

void setup()
{
    // Initialize the baud rate for serial communication.
    Serial.begin(115200);

    // Initialize mouse and keyboard emulation.
    Keyboard.begin();
    Mouse.begin();

    // Initialize the JSON-RPC server.
    controller.begin(JSON_PROC_COUNT);
}

void loop()
{
    // Process incoming JSON-RPC requests.
    controller.process();
}
