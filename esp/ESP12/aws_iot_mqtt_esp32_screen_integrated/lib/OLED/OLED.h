
#ifndef __OLED_H
#define __OLED_H

#include "images.h"         // Bitmap images 
#include "heltec.h"         // OLED library
#include "battery.h"        // Battery Macros



class OLEDisplay{
    public:
    OLEDisplay(){};
    ~OLEDisplay(){};

    /**OLEDinit(), initalize heltec oled display .
     *@param int milli delay GMIT logo 
     **/
    void OLEDinit(uint16_t);
    void drawGMITlogo(void);
    void drawAWSicon(void);

    /**drawProgressBar(), initalize heltec oled display .
     * @param  int counter  - counter values used to increment progress bar 
     * @param  String txt  - text that describes progress bar
     **/
    void drawProgressBar(uint8_t, String);

    void clearScreen();

    /**print(), print String on oled display .
     * @param  String msg  - text to write on screen 
     * @param  int y  - position   
     **/
    void print(String, uint8_t);

    void screenSleep(void);
    void screenWakeup(void);

    void drawbatt(void);

};

extern OLEDisplay Display; 

#endif