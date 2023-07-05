#pragma once

#include "ioexpander.h"
#include "epd_driver.h"
#include "epd_highlevel.h"
#include <JPEGDecoder.h>

#define WAVEFORM EPD_BUILTIN_WAVEFORM
const uint16_t ep_width = EPD_WIDTH;
const uint16_t ep_height = EPD_HEIGHT;

EpdiyHighlevelState hl;

uint8_t updateCounter = 0;

void initDisplay() {
    epd_init(EPD_LUT_1K);
    hl = epd_hl_init(WAVEFORM);

    // Default orientation is EPD_ROT_LANDSCAPE
    epd_set_rotation(EPD_ROT_PORTRAIT);
}

void deinitDisplay() {
  pinMode(EP_D0, INPUT); 
  pinMode(EP_D1, INPUT); 
  pinMode(EP_D2, INPUT); 
  pinMode(EP_D3, INPUT); 
  pinMode(EP_D4, INPUT); 
  pinMode(EP_D5, INPUT); 
  pinMode(EP_D6, INPUT); 
  pinMode(EP_D7, INPUT); 
  pinMode(EP_CKH, INPUT);
  pinMode(EP_CKV, INPUT);
  pinMode(EP_STH, INPUT);
  pinMode(EP_LE, INPUT);
  pinMode(TEMP, INPUT);
}

void jpegInfo() {
  Serial.println(F("==============="));
  Serial.println(F("JPEG image info"));
  Serial.println(F("==============="));
  Serial.print(F(  "Width      :")); Serial.println(JpegDec.width);
  Serial.print(F(  "Height     :")); Serial.println(JpegDec.height);
  Serial.print(F(  "Components :")); Serial.println(JpegDec.comps);
  Serial.print(F(  "MCU / row  :")); Serial.println(JpegDec.MCUSPerRow);
  Serial.print(F(  "MCU / col  :")); Serial.println(JpegDec.MCUSPerCol);
  Serial.print(F(  "Scan type  :")); Serial.println(JpegDec.scanType);
  Serial.print(F(  "MCU width  :")); Serial.println(JpegDec.MCUWidth);
  Serial.print(F(  "MCU height :")); Serial.println(JpegDec.MCUHeight);
  Serial.println(F("==============="));
}

void drawHeader() {

}

void drawJPEGFromMemory(ImageBuffer buffer, int xpos, int ypos) {

  
  JpegDec.decodeArray(buffer.image, buffer.size);
  uint16_t  *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  jpegInfo();
  uint8_t *fb = epd_hl_get_framebuffer(&hl);
  // uint32_t mcu_pixels = mcu_w * mcu_h; // total pixels
  // TODO immplmenet something to track drawtime performance
  // uint32_t drawTime = millis();
  uint32_t lastUpdate = millis();
  uint32_t totalBlocks = JpegDec.MCUSPerRow * JpegDec.MCUSPerCol;
  uint32_t decodedBlocks = 0;
  while( JpegDec.read()){
    
    pImg = JpegDec.pImage;
    int mcu_x = (JpegDec.MCUx * mcu_w) + xpos;
    int mcu_y = (JpegDec.MCUy * mcu_h) + ypos;

      
    for (uint8_t y = 0; y < mcu_h; y++) {
      for (uint8_t x = 0; x < mcu_w; x++) {

          
          int absX = mcu_x + x;
          int absY = mcu_y + y;
          //if (absX >= 0 && absX < ep_height && absY >= 0 && absY < ep_width) {
            // Threshold is at 50% grey to either display black or white pixel


            // note: this is naive, we should actually evaluate the scan type
            // and based on the encoding decode color information in the correct way.
            uint8_t red = ((*pImg & 0xF800)>>11);
            uint8_t green = ((*pImg & 0x07E0)>>5);
            uint8_t blue = (*pImg & 0x001F);
            //uint8_t colorIdx = *pImg < 32767 ? 0 : 255;
            uint8_t gray = (6 * red + 5 * green + 6 * blue) / 16;
            //display.drawPixel(absX, absY, colorIdx);
            epd_draw_pixel(absX, absY, (*pImg)/ 256, fb);
            //gfx.setColor(colorIdx);
            //Serial.println(colorIdx);
            //gfx.setPixel(absX, absY);

          //}
          //Serial.printf("x: %d, y: %d, color: %d\n", absX, absY, pImg);
        *pImg++;
      }
    }
    //USE_SERIAL.print(".");
    yield();
    decodedBlocks++;
    if (millis() - lastUpdate > 1000 || decodedBlocks >= totalBlocks) {
        log_v("%d of %d JPEG blocks decoded\n", decodedBlocks, totalBlocks);
        lastUpdate = millis();
    }

  }

}

void drawHeader(float batteryVoltage, float batteryPercentage) {
    uint8_t *fb = epd_hl_get_framebuffer(&hl);
    EpdRect rect;
    rect.x = 10;
    rect.y = 2;
    rect.width = 24;
    rect.height = 10;
    epd_draw_rect(rect, 0x00, fb);
    rect.x = 12;
    rect.y = 4;
    rect.width = (int) (20.0 * batteryPercentage / 100.0);
    rect.height = 6;
    epd_fill_rect(rect, 0x00, fb);
}

void updateScreen() {
  log_i("Updating screen...");
  uint8_t *fb = epd_hl_get_framebuffer(&hl);
  uint8_t x = updateCounter % 2;
  // dirty 1st or 2nd vertical and horizontal line
  // to ensure the full canvas is considered for repainting
  epd_draw_line(x, 0, x, EPD_HEIGHT, 1, fb);
  epd_draw_line(0, x, EPD_WIDTH, x, 1, fb);
  epd_poweron();
  //epd_clear();
  epd_hl_update_screen(&hl, MODE_GC16, 25);
  epd_poweroff();
  log_i("...done");
  updateCounter++;
}