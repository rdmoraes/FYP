#ifndef __AWS_IOT_H
#define __AWS_IOT_H

#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include<time.h>
#include "secrets.h"        // AWS secrets
#include "adxl357b_app.h"
#include <OLED.h>

//MQTT topics that this device will publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32_1/samples"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32_1/commands"
#define PACKET_SIZE              2048


void connectAWS(void);
void messageHandler(String &topic, String &payload);
void publishMessage(void);
String getTime(void);
void clientLoop(void);


#endif