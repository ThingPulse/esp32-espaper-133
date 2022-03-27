#pragma once

#define PHY_CS 0
#define PHY_RST 1
#define I2C_SDA 32
#define I2C_SCL 33

#define EPDIY_I2C_PORT I2C_NUM_0

#define MQTT_USER "MQTT user"
#define MQTT_PASS "MQTT pass"
#define MQTT_SERVER "ssl.mqtt-domain.xx"
#define MQTT_PORT 8883
#define MQTT_DEVICE_ID "133-Test-Device"
#define MQTT_PUBLISH_TOPIC "MQTT_Pub"    
#define MQTT_SUBSCRIBE_TOPIC  "MQTT_Sub" 

#define IOEXPANDER_ADDRESS 0x20

typedef struct {
    uint8_t* image;
    uint32_t size;
} ImageBuffer;