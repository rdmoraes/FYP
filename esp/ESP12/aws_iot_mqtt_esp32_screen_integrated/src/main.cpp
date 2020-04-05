#include <Arduino.h>
#include <SPI.h>
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include<time.h>

#include "adxl357b_app.h"
#include "secrets.h"        // AWS secrets
#include <OLED.h>



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
OLEDisplay display = OLEDisplay();


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;



void connectAWS(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.println("");
  Serial.println("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED){
    static int counter = 0;
    delay(500);
    Serial.print(".");
    display.drawProgressBar(++counter, "Connecting to Wi-Fi");
  }
  display.drawProgressBar(10, "Connecting to Wi-Fi");
    
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
  display.drawAWSicon();
  delay(200);

  while(!client.connect(THINGNAME)){
    static int counter = 0;
    Serial.print(".");
    delay(100);
    display.drawProgressBar(++counter, "Connecting to AWS");
  }
  Serial.println("");
  display.clearScreen();

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    display.print("AWS IoT Timeout!", 0);
    return;
  }

  //Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.printf("Subscribed to topic %s \n", AWS_IOT_SUBSCRIBE_TOPIC);
  display.print("Subscribed to topic", 0);
  display.print(AWS_IOT_SUBSCRIBE_TOPIC, 1);
}

void messageHandler(String &topic, String &payload){
  Serial.println("incoming: " + topic + " - " + payload);
  display.clearScreen();
  display.print("incoming: ", 0);
  
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);

  if(doc["command"] == "takeSample"){
    display.print("take sample!", 1);
    publishMessage();
  }
  if(doc["motor"]=="on"){
    Serial.println("Turn on motor!!");
    display.print("Turn on motor!!", 1);
  }
  else if(doc["motor"]=="off"){
    Serial.println("Turn off motor!!");
    display.print("Turn off motor!!", 1);
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
  
  display.print("Publishing data into topic", 3);
  display.print(AWS_IOT_PUBLISH_TOPIC, 4);
  
  
  if(!client.publish(AWS_IOT_PUBLISH_TOPIC, buffer, n)){
    Serial.printf("Fail to publish into topic %s \n",AWS_IOT_PUBLISH_TOPIC);
    display.print("publish message fail", 5);
  }
  else{
    Serial.printf("message published!\n\n");
    display.print("message published!", 5);
  } 
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
  display.OLEDinit(2000);  
  connectAWS();
  
  if(startAccelerometer()){
    display.print("Accelerometer init FAIL!",3);
    while(1);
  }
  else
    display.print("Accelerometer init OK!", 3);
}

void loop() {
  //publishMessage();
  client.loop();
  delay(1000);
}