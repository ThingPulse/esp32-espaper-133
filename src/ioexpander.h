

#pragma once

#include <pca9555.h>
#include "settings.h"

//PCA9555 ioport(0x20);

#define CS_BIT 0x00
#define RST_BIT 0x01

void pca9555_setCSBit(uint8_t state) {
    uint8_t highPort = 0;
    uint8_t value = 0x00;
    value |= 1UL << RST_BIT;
    if (state) value |= 1UL << CS_BIT;
    pca9555_set_value(EPDIY_I2C_PORT, value, highPort);

}

void initIoExpander() {
    // Start Wire again, because the PCA9555 uses different pins
    /*Wire.begin(I2C_SDA,I2C_SCL); 
    ioport.begin();
    ioport.setClock(400000);

    ioport.pinMode(PHY_CS, OUTPUT);
    ioport.pinMode(PHY_RST, OUTPUT);
    ioport.digitalWrite(PHY_RST, LOW);
    delay(500);
    ioport.digitalWrite(PHY_RST, HIGH);*/
    uint8_t highPort = 0;
    uint8_t config = 0xff;
    config &= ~(1UL << CS_BIT);
    config &= ~(1UL << RST_BIT);
    pca9555_set_config(EPDIY_I2C_PORT, config, highPort);

    uint8_t value = 0x00;
    // set all ports to low, including RST_BIT
    pca9555_set_value(EPDIY_I2C_PORT, value, highPort);
    delay(500);
    // set RST_BIT to high
    value |= 1UL << RST_BIT;
    pca9555_set_value(EPDIY_I2C_PORT, value, highPort);

}

void deinitIoExpander() {
    uint8_t value = 0x00;
    ESP_ERROR_CHECK(pca9555_set_value(EPDIY_I2C_PORT, value, 0));
    ESP_ERROR_CHECK(pca9555_set_value(EPDIY_I2C_PORT, value, 1));
    ESP_ERROR_CHECK(pca9555_set_config(EPDIY_I2C_PORT, value, 0));
    ESP_ERROR_CHECK(pca9555_set_config(EPDIY_I2C_PORT, value, 1));
}