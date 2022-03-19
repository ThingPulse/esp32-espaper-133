

#pragma once

#include "clsPCA9555.h"
#include "Wire.h"
#include "settings.h"

PCA9555 ioport(0x20);

void initIoExpander() {
    // Start Wire again, because the PCA9555 uses different pins
    Wire.begin(I2C_SDA,I2C_SCL); 
    ioport.begin();
    ioport.setClock(400000);

    ioport.pinMode(PHY_CS, OUTPUT);
    ioport.pinMode(PHY_RST, OUTPUT);
    ioport.digitalWrite(PHY_RST, LOW);
    delay(500);
    ioport.digitalWrite(PHY_RST, HIGH);
}