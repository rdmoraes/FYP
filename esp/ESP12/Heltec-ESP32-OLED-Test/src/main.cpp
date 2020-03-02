#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
/*
 * HelTec Automation(TM) ESP32 Series Dev boards OLED draw Simple Function test code
 *
 * - Some OLED draw Simple Function function test;
 *
 * by LXYZN from HelTec AutoMation, ChengDu, China
 * 
 * www.heltec.cn
 *
 * this project also realess in GitHub:
 * https://github.com/HelTecAutomation/Heltec_ESP32
*/


// This example just provide basic function test;
// For more informations, please vist www.heltec.cn or mail to support@heltec.cn

#include "Arduino.h"
#include "heltec.h"
#include "images.h"



#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);



  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);

}

void drawFontFaceDemo() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->drawString(0, 0, "BEng Software & Elec Eng");
    Heltec.display->setFont(ArialMT_Plain_16);
    Heltec.display->drawString(0, 10, "Hello world");
    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(0, 26, "Hello world");
}

void drawTextFlowDemo() {
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->drawStringMaxWidth(0, 0, 128,
      "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );
}

void drawTextAlignmentDemo() {
    // Text alignment demo
  Heltec.display->setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
  Heltec.display->drawString(128, 33, "Right aligned (128,33)");
}


void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  Heltec.display->drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  Heltec.display->setTextAlignment(TEXT_ALIGN_CENTER);
  Heltec.display->drawString(64, 15, String(progress) + "%");
}

void drawGMITlogo() {
    
    Heltec.display->setFont(ArialMT_Plain_24);
    Heltec.display->drawString(93, 5, "GMIT");
    Heltec.display->setFont(ArialMT_Plain_10);
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
    Heltec.display->drawString(0, 32, "BEng Software & Elec Eng");
    Heltec.display->drawXbm(0, 0, GMIT_Logo_width,GMIT_Logo_height, GMIT_Logo_bits);
}

void drawAWSicon() {
    
     Heltec.display->drawXbm(0, 0,aws_icon_width,aws_icon_height, aws_icon_bits);
}

Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawAWSicon, drawGMITlogo};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

void loop() {
  // clear the display
  Heltec.display->clear();
  // draw the current demo method
  demos[demoMode]();

  Heltec.display->setTextAlignment(TEXT_ALIGN_RIGHT);
  Heltec.display->drawString(10, 128, String(millis()));
  // write the buffer to the display
  Heltec.display->display();

  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
    demoMode = (demoMode + 1)  % demoLength;
    timeSinceLastModeSwitch = millis();
  }
  counter++;
  delay(10);
}