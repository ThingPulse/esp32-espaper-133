
#pragma once
#include "defines.h"

#include <PubSubClient.h>
#include "ethernet.h"


const char *ID        = MQTT_DEVICE_ID;  // Name of our device, must be unique
const char *TOPIC     = MQTT_PUBLISH_TOPIC;               // Topic to subcribe to
const char *subTopic  = MQTT_SUBSCRIBE_TOPIC;               // Topic to subcribe to

//IPAddress mqttServer(172, 16, 0, 2);

//
void callback(char* topic, byte* payload, unsigned int length);

//EthernetClient  ethClient;
PubSubClient    mqttClient(MQTT_SERVER, MQTT_PORT, callback, sslClient);

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (unsigned int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }
  
  Serial.println();
}



void reconnect()
{
  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection to ");
    Serial.print(MQTT_SERVER);

    // Attempt to connect
    if (mqttClient.connect(ID, MQTT_USER, MQTT_PASS))
    {
      Serial.println("...connected");
      
      // Once connected, publish an announcement...
      String data = "Hello from MQTTClient_SSL on " + String(BOARD_NAME);

      mqttClient.publish(TOPIC, data.c_str());

      //Serial.println("Published connection message successfully!");
      //Serial.print("Subcribed to: ");
      //Serial.println(subTopic);
      
      // This is a workaround to address https://github.com/OPEnSLab-OSU/SSLClient/issues/9
      //ethClientSSL.flush();
      // ... and resubscribe
      mqttClient.subscribe(subTopic);
      // for loopback testing
      mqttClient.subscribe(TOPIC);
      // This is a workaround to address https://github.com/OPEnSLab-OSU/SSLClient/issues/9
      //ethClientSSL.flush();
    }
    else
    {
      Serial.print("...failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void initMqtt()
{


  Serial.print("\nStart MQTTClient_Basic on " + String(BOARD_NAME));
  Serial.println(" with " + String(SHIELD_TYPE));
  Serial.println(ETHERNET_WEBSERVER_SSL_VERSION);


  mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  mqttClient.setCallback(callback);

  // Allow the hardware to sort itself out
  delay(1500);
}

#define MQTT_PUBLISH_INTERVAL_MS       5000L

String data         = "Hello from MQTTClient_Basic on " + String(BOARD_NAME) + " with " + String(SHIELD_TYPE);
const char *pubData = data.c_str();

unsigned long lastMsg = 0;

void processMqtt() 
{
  static unsigned long now;
  
  if (!client.connected()) 
  {
    reconnect();
  }

  // Sending Data
  now = millis();
  
  if (now - lastMsg > MQTT_PUBLISH_INTERVAL_MS)
  {
    lastMsg = now;

    if (!mqttClient.publish(TOPIC, pubData))
    {
      Serial.println("Message failed to send.");
    }

    Serial.print("Message Send : " + String(TOPIC) + " => ");
    Serial.println(data);
  }
  
  mqttClient.loop();
}