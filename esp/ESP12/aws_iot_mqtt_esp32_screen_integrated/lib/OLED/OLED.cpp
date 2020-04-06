#include <Arduino.h>
#include "OLED.h"



void OLEDisplay::OLEDinit(uint16_t milli){
    Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, false /*Serial Enable*/);
    
    /** Enable Analog Reading Batt **/
    Heltec.VextON();
    analogSetClockDiv(255);
    
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

void OLEDisplay::drawProgressBar(uint8_t counter, String txt) {
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

void OLEDisplay::print(String msg, uint8_t y){
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

void OLEDisplay::drawbatt()  
{
  //Battery voltage read pin changed from GPIO37
  uint16_t battery_charge  =  analogRead(BATT_PIN)*ADC_RATIO_CORRECTION*MV;

  if (battery_charge > FULL_BATT)
    battery_charge = FULL_BATT;
    
  int percent = (battery_charge - FLAT_BATT)/PERCENT_FACTOR; 

  Heltec.display->drawRect(110, 0,12,6);
  Heltec.display->fillRect(123, 1, 1, 4);
  Heltec.display->fillRect(111, 1, percent + 1, 4);
  
}

OLEDisplay Display = OLEDisplay();