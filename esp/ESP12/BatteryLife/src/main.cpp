/*
 Heltec WIFI-KIT-32.V2  Battery Monitoring 
 Reads batery voltage using GPIO37 and display it in an batt icon in percentage

 References
 https://github.com/3KUdelta/heltec_wifi_kit_32_batt_monitor
*/

#include <Arduino.h>
#include "heltec.h"

/*Macros and constants*/               
#define FULL_BATT 4200
#define FLAT_BATT 3200
#define PERCENT_FACTOR 100
#define BATT_PIN 37

//Manually batt measurement voltage at full charge  divided by analog reading
const float ADC_RATIO_CORRECTION = 0.002109; 
const uint16_t MV = 1000;

char buf[128];

/*Draw batt icon and fill with percentage */
void drawbatt(uint16_t);

void setup() {
  
  Serial.begin(115200);
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.VextON();

  analogSetClockDiv(255); 

}

void loop() {
  // clear the display
  Heltec.display->clear();

  //Battery voltage read pin changed from GPIO37
  uint16_t battery_charge  =  analogRead(BATT_PIN)*ADC_RATIO_CORRECTION*MV;
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);

  sprintf(buf, "Analog read: %d", analogRead(BATT_PIN));
  Heltec.display->drawString(0, 0, buf);
  sprintf (buf, "VBat: %d", battery_charge);
  
  Heltec.display->drawString(0, 12, buf);
  Heltec.display->drawString(70, 12, "(mV)");

  drawbatt(battery_charge);

  // write the buffer to the display
  Heltec.display->display();

  delay(2000);
}

void drawbatt(uint16_t battery_charge)  
{
  if (battery_charge > FULL_BATT)
    battery_charge = FULL_BATT;
    
  int percent = (battery_charge - FLAT_BATT)/PERCENT_FACTOR; 

  Heltec.display->drawRect(110, 0,12,6);
  Heltec.display->fillRect(123, 1, 1, 4);
  Heltec.display->fillRect(111, 1, percent + 1, 4);
  
}