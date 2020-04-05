
#ifndef __OLED_H
#define __OLED_H

#include "images.h"         // Bitmap images 
#include "heltec.h"         // OLED library

class OLEDisplay{
    public:
    OLEDisplay(){};
    ~OLEDisplay(){};

    /**OLEDinit(), initalize heltec oled display .
     *@param int milli delay GMIT logo 
     **/
    void OLEDinit(int);
    void drawGMITlogo(void);
    void drawAWSicon(void);

    /**drawProgressBar(), initalize heltec oled display .
     * @param  int counter  - counter values used to increment progress bar 
     * @param  String txt  - text that describes progress bar
     **/
    void drawProgressBar(int, String);

    void clearScreen();

    /**print(), print String on oled display .
     * @param  String msg  - text to write on screen 
     * @param  int y  - position   
     **/
    void print(String, int);

    void screenSleep(void);
    void screenWakeup(void);

};

#endif