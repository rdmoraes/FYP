#ifndef __BATTERY_H
#define __BATTERY_H

/*Macros and constants for battery icon*/               
#define FULL_BATT 4200
#define FLAT_BATT 3200
#define PERCENT_FACTOR 100
#define BATT_PIN 37

//Manually batt measurement voltage at full charge  divided by analog reading
const float ADC_RATIO_CORRECTION = 0.002109; 
const int MV = 1000;

#endif