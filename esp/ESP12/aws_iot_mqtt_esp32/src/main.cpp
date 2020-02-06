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
#define PACKET_SIZE              2048


void connectAWS();
void messageHandler(String &topic, String &payload);
void publishMessage();
String getTime();

acc_struct_xyz accelerometer_data;


WiFiClientSecure network = WiFiClientSecure();
MQTTClient client = MQTTClient(PACKET_SIZE);


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;



void connectAWS(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.println("");
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

  Serial.println("Connecting to AWS IoT");

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
  Serial.printf("Subscribed to topic %s \n", AWS_IOT_SUBSCRIBE_TOPIC);
}

void messageHandler(String &topic, String &payload){
  Serial.println("incoming: " + topic + " - " + payload);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);

  if(doc["command"] == "takeSample"){
    Serial.println("Yes, master!");
    publishMessage();
    
  }
  if(doc["motor"]=="on"){
    Serial.println("Turning on the engines!!");
  }
  else if(doc["motor"]=="off"){
    Serial.println("Turning off the engines!!");
  }
}

void publishMessage(){
  takeSamples(&accelerometer_data);
  
  //Specify JSON capacity based on format {"motor": 1, "timestamp" : "Tue Feb 4 14:11:15", 
  //"x_axis" : [0, ...], "y_axis" : [0, ...], "z_axis" : [0, ...] } 
  const size_t capacity = 3*JSON_ARRAY_SIZE(100) + JSON_OBJECT_SIZE(5);

  //Allocate JSON in the heap
  DynamicJsonDocument doc(capacity);
  
 
  doc["motor"] = 1;
  doc["timestamp"] = getTime();
  JsonArray doc_x_axis = doc.createNestedArray("x_axis");
  JsonArray doc_y_axis = doc.createNestedArray("y_axis");
  JsonArray doc_z_axis = doc.createNestedArray("z_axis");
   
  for(int i=0; i < N_SAMPLE; i++){
   doc_x_axis.add(accelerometer_data.x[i]);
   doc_y_axis.add(accelerometer_data.y[i]);
   doc_z_axis.add(accelerometer_data.z[i]);
  }
  
  char buffer[PACKET_SIZE];
  size_t n = serializeJson(doc, buffer);

  Serial.println();
  Serial.print("Sending: ");
  Serial.println(buffer);
  Serial.print("Payload size: ");
  Serial.println(n);
  
  if(!client.publish(AWS_IOT_PUBLISH_TOPIC, buffer, n))
    Serial.printf("Fail to publish into topic %s \n",AWS_IOT_PUBLISH_TOPIC);
  else 
    Serial.printf("message published!\n\n");
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