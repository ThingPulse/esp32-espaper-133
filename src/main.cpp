#include <Arduino.h>
#include "settings.h"
#include "ioexpander.h"
#include "display.h"
#include "ethernet.h"


void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial);

    initDisplay();
    initIoExpander();
    initEthernet();

    getImage();
}

void loop() {
    //processData();
 
}