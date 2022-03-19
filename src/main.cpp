
#include "settings.h"
#include "display.h"
#include "ethernet.h"
#include "ioexpander.h"

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial);

    //initDisplay();
    initIoExpander();
    initEthernet();
    getImage();

}

void loop() {
 
}