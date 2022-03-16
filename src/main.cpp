
#include "Arduino.h"
#include "epd_driver.h"
#include "epd_highlevel.h"

#define WAVEFORM EPD_BUILTIN_WAVEFORM

EpdiyHighlevelState hl;
int radius = 20;
int x = radius;
int y = radius;
int speed = 10;
int xd = 10;
int yd = 10;



void setup() { 
    Serial.begin(115200);
    log_d("Total heap: %d", ESP.getHeapSize());
    log_d("Free heap: %d", ESP.getFreeHeap());
    log_d("Total PSRAM: %d", ESP.getPsramSize());
    log_d("Free PSRAM: %d", ESP.getFreePsram());
    epd_init(EPD_LUT_1K);
    hl = epd_hl_init(WAVEFORM);

    // Default orientation is EPD_ROT_LANDSCAPE
    epd_set_rotation(EPD_ROT_LANDSCAPE);

    


    epd_poweron();
    epd_clear();

    //epd_poweroff();

    //epd_fill_circle(30,30,15,0,fb);

 }

EpdRect area;

void loop() {
  Serial.println(millis());
  uint8_t* fb = epd_hl_get_framebuffer(&hl);
  epd_fill_circle(x,y,radius,255,fb);
  x += xd;
  y += yd;
  if (x > 1600 - radius) {
    xd = - speed;
  }
  if (x <= radius) {
    xd = speed;
  }
  if (y > 1200 - radius) {
    yd = - speed;
  }
  if (y <= radius) {
    yd = speed;
  }
  area.x = x - (radius + speed);
  area.y = y - (radius + speed);
  area.width = 2* (radius + speed) + 2;
  area.height = 2 * (radius + speed) + 2;
  epd_fill_circle(x,y,radius,0,fb);
  //epd_hl_update_screen(&hl, MODE_GL16, 25);
  epd_hl_update_area(&hl, MODE_DU, 25, area);
  //delay(100);
}

