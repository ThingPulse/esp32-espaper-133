
#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "settings.h"
#include <esp_wifi.h>
#include <esp_bt.h>

boolean initWifi() {

  //WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoConnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  uint32_t startMillis = millis();
  while(!WiFi.isConnected()) {
    Serial.print(".");
    delay(50);
    if (millis() - startMillis > 15000) {
        return false;
    }
  }
  return true;
}

void deinitWifi() {
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);
  WiFi.setSleep(true);
  btStop();
  esp_wifi_stop();
  esp_bt_controller_disable();
}

ImageBuffer getImageFromWifi(String url) {
    ImageBuffer imageBuffer;
    imageBuffer.size = 0;
    uint32_t startTime = millis();
    Serial.println("Downloading " + url + " and storing to buffer");

    // wait for WiFi connection
    // TODO - decide if there's a different action for first call or subsequent calls
	// boolean isFirstCall = true;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");

    // configure server and url
    http.begin(url);

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if(httpCode > 0) {

        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if(httpCode == HTTP_CODE_OK) {

            // get lenght of document (is -1 when Server sends no Content-Length header)
            int total = http.getSize();
            Serial.printf("Free Heap: %d\n", ESP.getFreeHeap());
            Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());
            uint8_t* jpeg = (uint8_t*)ps_malloc(total);
            imageBuffer.image = jpeg;
            imageBuffer.size = total;
            Serial.printf("Expected total length of file: %d\n", total);
            int len = total;
            //progressCallback(filename, 0,total, true);
            // create buffer for read
            

            // get tcp stream
            WiFiClient * stream = http.getStreamPtr();
            uint16_t counter = 0;
            uint32_t totalDownloaded = 0;

            // read all data from server
            while(http.connected() && (len > 0 || len == -1)) {
                // get available data size
                size_t size = stream->available();
                
                if(size) {
                    // read up to 128 byte
                    int c = stream->readBytes(jpeg + totalDownloaded, total);
                    totalDownloaded += c;
                    // write it to Serial
   

                    if(len > 0) {
                        len -= c;
                    }
                    //progressCallback(filename, total - len,total, false);
                    // isFirstCall = false;
                    //executeCallback();
                }

                Serial.print("#");

            }

            WiFi.disconnect(true);
            WiFi.mode(WIFI_OFF);

            Serial.printf("Total bytes downloaded: %d\n", totalDownloaded);
            Serial.println();
            Serial.print("[HTTP] connection closed or file end.\n");
            Serial.printf("Time to download: %dms\n", millis() - startTime);
            startTime = millis();


        }

    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();
    return imageBuffer;
    
}