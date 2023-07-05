#pragma once

#include "ioexpander.h"
#include "defines.h"

// You must have SSL Certificates here
#include "trust_anchors.h"
#include "settings.h"



EthernetClient    client;
EthernetSSLClient sslClient(client, TAs, (size_t)TAs_NUM);

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// Raw IP address not accepted in SSL
//IPAddress server_host(104, 22, 48, 75);

// https://www.scantick.com/wp-content/uploads/2021/09/immotick-e-ink-display-2-0.jpg
const char      server_host[]   = "www.scantick.com"; // leave this alone, change only above two
const char      documentPath[] = "/wp-content/uploads/2021/09/immotick-e-ink-display-2-0.jpg";
const uint16_t  server_port     = 443;

// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;

bool printWebData = false;  // set to false for better speed measurement

void setCSPin(uint8_t state) {
  pca9555_setCSBit(state);
}

void initEthernet() {



    Serial.print("\nStart WebClient_SSL on " + String(BOARD_NAME));
    Serial.println(" with " + String(SHIELD_TYPE));
    Serial.println(ETHERNET_WEBSERVER_SSL_VERSION);

    Ethernet.init(-1);

    // start the ethernet connection and the server:
    // Use DHCP dynamic IP and random mac
    uint16_t index = millis() % NUMBER_OF_MAC;

    Serial.println(F("========================="));

    Serial.print(F("Using mac index = "));
    Serial.println(index);
    // Use Static IP
    //Ethernet.begin(mac[index], ip);
    Ethernet.begin(mac[index]);

    Serial.print(F("Connected! IP address: "));
    Serial.println(Ethernet.localIP());
    
    // give the Ethernet shield a second to initialize:
    delay(2000);

    Serial.print("Connecting to : ");
    Serial.print(server_host);
    Serial.print(", port : ");
    Serial.println(server_port);


}

void deinitEthernet() {
      Enc28J60Network::powerOff();
      delay(100);
      pinMode(PHY_INT, INPUT); 

}

ImageBuffer getImage() {
    ImageBuffer imageBuffer;
    imageBuffer.size = 0;
    // if you get a connection, report back via serial:
    auto start = millis();
    // specify the server and port, 443 is the standard port for HTTPS
    if (sslClient.connect(server_host, server_port)) {
        auto time = millis() - start;
        Serial.print("Connected to ");

        
        Serial.print("Took: ");
        Serial.println(time);

        // Make a HTTP request:
        sslClient.print("GET ");
        sslClient.print(documentPath);
        sslClient.println(" HTTP/1.1");
        sslClient.println("User-Agent: SSLClientOverEthernet");
        sslClient.print("Host: ");
        sslClient.println(server_host);
        sslClient.println("Connection: close");
        sslClient.println();
        Serial.println("Request sent");
    } else {
        // if you didn't get a connection to the server:
        Serial.printf("Connection failed: %d\n", sslClient.getWriteError());

        return imageBuffer;
    }

    beginMicros = micros();
    Serial.println("Processing Data");
     // if there are incoming bytes available
    // from the server, read them and print them:
    int len = 0;
    boolean isHeader = true;
    uint32_t lastUpdate = millis();
    uint32_t contentLength = 0;
    uint8_t *currentBuffer;

    while(sslClient.connected()) {
        len = sslClient.available();


        if (len > 0)
        {
            if (isHeader) {
                String headerLine = sslClient.readStringUntil('\n');
                Serial.println(headerLine);
                if (headerLine == "\r") {
                    Serial.println("Body:");
                    isHeader = false;

                }
                if (headerLine.startsWith("Content-Length:")) {
                    String contentLengthText = headerLine.substring(16);
                    Serial.printf("contentLength: %s\n", contentLengthText.c_str());
                    contentLength = contentLengthText.toInt();
                    Serial.printf("as number: %d\n", contentLength);
                    if (ESP.getFreePsram() < contentLength) {
                        Serial.printf("Not enough PSRAM to store image in memory!");
                        return imageBuffer;
                    }
                    imageBuffer.image = (uint8_t*)ps_malloc(contentLength);
                    imageBuffer.size = contentLength;
                    currentBuffer = imageBuffer.image;
                }
            } else {
                int bufferSize = 1024;


                if (len > bufferSize) len = bufferSize;

                sslClient.read(currentBuffer, len);

                if (printWebData) {
                    Serial.write(currentBuffer, len); // show in the serial monitor (slows some boards)
                }
                if (millis() - lastUpdate > 1000) {
                    Serial.printf("Received %d bytes\n", byteCount);
                    lastUpdate = millis();
                }

                byteCount = byteCount + len;
                currentBuffer = imageBuffer.image + byteCount;
            }
        }
    }
    Serial.printf("Total byte count: %d\n", byteCount);
    endMicros = micros();

    Serial.println();
    Serial.printf("Disconnecting. Status code: %d\n", sslClient.getWriteError());
    sslClient.stop();

    Serial.print("Received ");
    Serial.print(byteCount);
    Serial.print(" bytes in ");
    float seconds = (float)(endMicros - beginMicros) / 1000000.0;
    Serial.print(seconds, 4);
    float rate = (float)byteCount / seconds / 1000.0;
    Serial.print(" s, rate = ");
    Serial.print(rate);
    Serial.print(" kbytes/second");
    Serial.println();

    return imageBuffer;
  
}

void processData() {

}