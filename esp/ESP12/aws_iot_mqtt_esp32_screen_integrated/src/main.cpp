#include <Arduino.h>
#include <SPI.h>

#include <aws_iot.h>
#include "adxl357b_app.h"
#include <OLED.h>


void setup() {
  Serial.begin(115200);
  Display.OLEDinit(2000);  
  connectAWS();
  
  if(startAccelerometer()){
    Display.print("Accelerometer init FAIL!",3);
    while(1);
  }
  else
    Display.print("Accelerometer init OK!", 3);
}

void loop() {
  //publishMessage();
  clientLoop();
  delay(1000);
}