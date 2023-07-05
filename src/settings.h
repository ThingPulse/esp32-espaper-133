#pragma once



#define EPDIY_I2C_PORT I2C_NUM_0

#define MQTT_USER "MQTT user"
#define MQTT_PASS "MQTT pass"
#define MQTT_SERVER "ssl.mqtt-domain.xx"
#define MQTT_PORT 8883
#define MQTT_DEVICE_ID "133-Test-Device"
#define MQTT_PUBLISH_TOPIC "MQTT_Pub"    
#define MQTT_SUBSCRIBE_TOPIC  "MQTT_Sub" 

#define WIFI_SSID "yourssid"
#define WIFI_PASSWORD "yourpassw0rd"

#define IMAGE_URL "http://192.168.0.100:1880/imageBig"

#define IOEXPANDER_ADDRESS 0x20

#define BAT_MON_PIN 34
// voltage divider
#define BAT_R1 2.2f
#define BAT_R2 4.7f

#define PHY_CS 0
#define PHY_RST 1
#define I2C_SDA 32
#define I2C_SCL 33
#define PHY_INT 39
#define GPIO_INTR 36
#define TEMP 35
#define EP_D0 25
#define EP_D1 4
#define EP_D2 5
#define EP_D3 18
#define EP_D4 19
#define EP_D5 21
#define EP_D6 22
#define EP_D7 23
#define EP_CKH 15
#define EP_CKV 26
#define EP_STH 27
#define EP_LE 2
#define MISO 12
#define MOSI 13
#define SCLK 14


#define DEEP_SLEEP_MICROS 1000 * 1000 * 60 * 15

typedef struct {
    uint8_t* image;
    uint32_t size;
} ImageBuffer;