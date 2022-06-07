#include <Arduino.h>
#include "settings.h"
#include "ioexpander.h"
#include "display.h"
#include "ethernet.h"
#include "mqtt.h"

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial);

    initDisplay();
    initIoExpander();
    initEthernet();
    initMqtt();

 
    ImageBuffer buffer = getImage();
    drawJPEGFromMemory(buffer, 0, 0);
    
}

void loop() {
    processMqtt();
    
    // Renew DHCP-lease if required.
    // https://www.arduino.cc/reference/en/libraries/ethernet/ethernet.maintain/
    // https://github.com/JAndrassy/EthernetENC/blob/master/src/Ethernet.h#L91
    Ethernet.maintain();
}