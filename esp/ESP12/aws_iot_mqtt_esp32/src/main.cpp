#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include<time.h>

#include "adxl357b_app.h"
#include "secrets.h"

//MQTT topics that this device will publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32_1/samples"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32_1/commands"


void connectAWS();
void messageHandler(String &topic, String &payload);
void publishMessage();
String getTime();



WiFiClientSecure network = WiFiClientSecure();
MQTTClient client = MQTTClient(256);


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;



void connectAWS(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  // Init time connecting to ntpServer
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

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
  Serial.println("");

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  //Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("Connected AWS IoT!");
}

void messageHandler(String &topic, String &payload){
  Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);

  if(doc["command"] == "takeSample"){
    Serial.println("Yes, master!");
    takeSamples();
  }
}

void publishMessage(){
  StaticJsonDocument<200> doc;
  doc["time"] = getTime();
  doc["x"] = analogRead(0);
  doc["y"] = analogRead(1);
  doc["z"] = analogRead(2);

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  Serial.println("publish message");
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}


String getTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "Failed to obtain time";
  }
  return asctime(&timeinfo);
}


void setup() {
  Serial.begin(115200);
  connectAWS();
  startAccelerometer();
}

void loop() {
  //publishMessage();
  client.loop();
  delay(1000);
}