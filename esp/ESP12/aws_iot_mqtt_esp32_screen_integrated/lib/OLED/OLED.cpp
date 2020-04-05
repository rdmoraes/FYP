#include <Arduino.h>
#include "OLED.h"



void OLEDisplay::OLEDinit(int milli){
    Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, false /*Serial Enable*/);
    drawGMITlogo();
    delay(milli);
}

void OLEDisplay::drawGMITlogo(){
    // clear the display
    Heltec.display->clear();
    Heltec.display->flipScreenVertically();
    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(35, 5, "GMIT");
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->drawString(0, 32, "BEng Software & Elec Eng");
    Heltec.display->drawString(15, 48, "FYP - Renan Moraes");
    Heltec.display->drawXbm(0, 0, GMIT_Logo_width,GMIT_Logo_height, GMIT_Logo_bits);
    
    // write the buffer to the display
    Heltec.display->display();
}

void OLEDisplay::drawProgressBar(int counter, String txt) {
  int progress = (5*counter);
  Heltec.display->clear();

  if(progress>95)
    progress = 98;
  // draw the progress bar
  Heltec.display->drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->drawString(64, 15, txt + "   "+ String(progress) + "%");
  Heltec.display->display();
}

void OLEDisplay::drawAWSicon() {
    Heltec.display->clear();
    Heltec.display->drawXbm(40, 0,aws_icon_width,aws_icon_height, aws_icon_bits);
    Heltec.display->display();
}

void OLEDisplay::clearScreen(){
    Heltec.display->clear();
}

void OLEDisplay::print(String msg, int y){
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->drawString(0, y * 10, msg);
    // write the buffer to the display
    Heltec.display->display();
}

void OLEDisplay::screenSleep(){
    Heltec.display->sleep();
}

void OLEDisplay::screenWakeup(){
    Heltec.display->wakeup();
}