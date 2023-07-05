#include <Arduino.h>
#include "settings.h"
#include "ioexpander.h"
#include "display.h"
//#include "ethernet.h"
#include "wireless.h"
#include "mqtt.h"
#include "battery.h"
#include <Wire.h>

//RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int bootCount = 0;

void pca955_init() {
  i2c_config_t conf;
  conf.mode = I2C_MODE_MASTER;
  conf.sda_io_num = GPIO_NUM_32;
  conf.scl_io_num = GPIO_NUM_33;
  conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
  conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
  conf.master.clk_speed = 400000;
  conf.clk_flags = 0;

  //conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;
  ESP_ERROR_CHECK(i2c_param_config(EPDIY_I2C_PORT, &conf));

  ESP_ERROR_CHECK(i2c_driver_install(EPDIY_I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));
}

void gotoSleep() {
    deinitEthernet();
    //pca955_init();
    Wire.begin(I2C_SDA,I2C_SCL, 400000); 
    deinitIoExpander();
    deinitDisplay();


    deinitBatteryMonitor();


    // turn off generic periphery pins
    pinMode(I2C_SDA, INPUT);
    pinMode(I2C_SCL, INPUT);
    pinMode(GPIO_INTR, INPUT);
    pinMode(MOSI, INPUT);
    pinMode(MISO, INPUT);
    pinMode(SCLK, INPUT);
    pinMode(GPIO_NUM_0, INPUT);

    /*esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);*/
    //rtc_gpio_isolate(GPIO_NUM_12);
    //rtc_gpio_isolate(GPIO_NUM_15);
    esp_bt_controller_disable();
    esp_wifi_stop();
    delay(500);
    Serial.println("Going to sleep...");
    esp_sleep_enable_timer_wakeup(DEEP_SLEEP_MICROS);
    esp_deep_sleep_start();
}

void setup()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial);


    //initEthernet();
    //initMqtt();
    bool successful = initWifi();
    if (!successful) {
        gotoSleep();
    }
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());

    initBatteryMonitor();

    float batteryVoltage = getBatteryVoltage();
    float batteryPercentage = getBatteryPercentage();
    String url = String(IMAGE_URL) + String("?vBat=") + String(batteryVoltage) 
    +  String("&percBat=") + String(batteryPercentage)
    + String("&bootCount=") + String(bootCount);
    ImageBuffer buffer = getImageFromWifi(url);
    deinitWifi();
    log_d("Free PSRAM: %d", ESP.getFreePsram());
    initDisplay();
    initIoExpander();
    drawJPEGFromMemory(buffer, 0, 0);


    drawHeader(batteryVoltage, batteryPercentage);
    
    updateScreen();
    //bootCount++;
    gotoSleep();

    
    
}

void loop() {
    //processMqtt();
}