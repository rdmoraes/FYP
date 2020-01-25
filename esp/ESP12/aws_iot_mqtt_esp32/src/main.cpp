#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#include "secrets.h"

//MQTT topics that this device will publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32_1/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32_1/sub"


void connectAWS();
void messageHandler(String &topic, String &payload);
void publishMessage();



WiFiClientSecure network = WiFiClientSecure();
MQTTClient client = MQTTClient(256);



void connectAWS(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  network.setCACert(AWS_CERT_CA);
  network.setCertificate(AWS_CERT_CRT);
  network.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint
  client.begin(AWS_IOT_ENDPOINT, 8883, network);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.println("Connecting to AWS IOT");

  while(!client.connect(THINGNAME)){
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  //Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
}

void messageHandler(String &topic, String &payload){
  Serial.println("incoming: " + topic + " - " + payload);
}

void publishMessage(){
  StaticJsonDocument<200> doc;
  doc["time"] = millis();
  doc["sensor_a0"] = analogRead(0);

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  Serial.println("publish message");
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}


void setup() {
  Serial.begin(115200);
  connectAWS();
}

void loop() {
  publishMessage();
  client.loop();
  delay(10000);
}