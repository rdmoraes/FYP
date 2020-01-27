#ifndef __ADXL357B_APP_H
#define __ADXL357B_APP_H

#include <Arduino.h>

#define CALI_BUF_LEN           15
#define CALI_INTERVAL_TIME     250
#define N_SAMPLE 			   512
#define N_AXIS                 3
#define MG_UNIT                1000

/*Uncomment this macro to print in the serial emulator */
//#define DEBUG					0

int32_t deal_cali_buf(int32_t *);
void calibration(void);
void startAccelerometer(void);

//Accelerometer array data
struct acc_struct_xyz{
	float x[N_SAMPLE];
	float y[N_SAMPLE];
	float z[N_SAMPLE];
}acc_data;

#endif