#include "aws_iot.h"

WiFiClientSecure network = WiFiClientSecure();
MQTTClient client = MQTTClient(PACKET_SIZE);


const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

acc_struct_xyz accelerometer_data;


void connectAWS(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.println("");
  Serial.println("Connecting to Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED){
    static int counter = 0;
    delay(500);
    Serial.print(".");
    Display.drawProgressBar(++counter, "Connecting to Wi-Fi");
  }
  Display.drawProgressBar(10, "Connecting to Wi-Fi");
    
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
  Display.drawAWSicon();
  delay(200);

  while(!client.connect(THINGNAME)){
    static int counter = 0;
    Serial.print(".");
    delay(100);
    Display.drawProgressBar(++counter, "Connecting to AWS");
  }
  Serial.println("");
  Display.clearScreen();

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    Display.print("AWS IoT Timeout!", 0);
    return;
  }

  //Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.printf("Subscribed to topic %s \n", AWS_IOT_SUBSCRIBE_TOPIC);
  Display.drawbatt();
  Display.print("Subscribed to topic", 0);
  Display.print(AWS_IOT_SUBSCRIBE_TOPIC, 1);
}


void messageHandler(String &topic, String &payload){
  Serial.println("incoming: " + topic + " - " + payload);
  Display.clearScreen();
  Display.drawbatt();
  Display.print("incoming: ", 0);
  
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);

  if(doc["command"] == "takeSample"){
    Display.print("take sample!", 1);
    publishMessage();
  }
  else if(doc["command"]=="MotorOn"){
    Serial.println("Turn on motor!!");
    Display.print("Turn on motor!!", 1);
  }
  else if(doc["command"]=="MotorOff"){
    Serial.println("Turn off motor!!");
    Display.print("Turn off motor!!", 1);
  }
  else{
    Display.print("unknown cmd!", 1); 
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
  
  Display.print("Publishing data into topic", 3);
  Display.print(AWS_IOT_PUBLISH_TOPIC, 4);
  
  
  if(!client.publish(AWS_IOT_PUBLISH_TOPIC, buffer, n)){
    Serial.printf("Fail to publish into topic %s \n",AWS_IOT_PUBLISH_TOPIC);
    Display.print("publish message fail", 5);
  }
  else{
    Serial.printf("message published!\n\n");
    Display.print("message published!", 5);
  } 
}


String getTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "Failed to obtain time";
  }
  return asctime(&timeinfo);
}

void clientLoop(){
    client.loop();
}