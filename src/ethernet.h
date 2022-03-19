#pragma once

#include "defines.h"

// You must have SSL Certificates here
#include "trust_anchors.h"
#include "settings.h"
#include "ioexpander.h"


EthernetClient    client;
EthernetSSLClient sslClient(client, TAs, (size_t)TAs_NUM);

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// Raw IP address not accepted in SSL
//IPAddress server_host(104, 22, 48, 75);

// https://loremflickr.com/cache/resized/385_17889439303_57be1c692a_k_960_1280_nofilter.jpg

const char      server_host[]   = "loremflickr.com"; // leave this alone, change only above two
const uint16_t  server_port     = 443;

// Variables to measure the speed
unsigned long beginMicros, endMicros;
unsigned long byteCount = 0;

bool printWebData = false;  // set to false for better speed measurement

void setCSPin(uint8_t state) {
  ioport.digitalWrite(PHY_CS, state);
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

void getImage() {
    // if you get a connection, report back via serial:
    auto start = millis();
        // specify the server and port, 443 is the standard port for HTTPS
    if (sslClient.connect(server_host, server_port)) {
        auto time = millis() - start;
        Serial.print("Connected to ");

        
        Serial.print("Took: ");
        Serial.println(time);

        // Make a HTTP request:
        sslClient.println("GET /cache/resized/65535_51753983634_758f6dbf65_k_1280_960_nofilter.jpg HTTP/1.1");
        sslClient.println("User-Agent: SSLClientOverEthernet");
        sslClient.print("Host: ");
        sslClient.println(server_host);
        sslClient.println("Connection: close");
        sslClient.println();
    }
    else
    {
        // if you didn't get a connection to the server:
        Serial.println("Connection failed");
    }

    beginMicros = micros();
}

void processData() {
     // if there are incoming bytes available
  // from the server, read them and print them:
  int len = sslClient.available();

  if (len > 0)
  {
    byte buffer[80];

    if (len > 80)
      len = 80;

    sslClient.read(buffer, len);

    if (printWebData)
    {
      Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }

    byteCount = byteCount + len;
  }

  // if the server's disconnected, stop the sslClient:
  if (!sslClient.connected())
  {
    endMicros = micros();

    Serial.println();
    Serial.println("Disconnecting.");
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

    // do nothing forevermore:
    while (true)
    {
      delay(1);
    }
  }
}