/*!
 *****************************************************************************
 * @file:    ADXL357.h
 * @brief:   ADXL357 accelerometer IC
 * @version: $Revision$
 * @date:    $Date$
 *-----------------------------------------------------------------------------
 *
 *****************************************************************************/

#ifndef ADXL357_H
#define ADXL357_H


/* ADXL357 Register Map */
#define DEVID_AD                 0x00
#define DEVID_MST                0x01
#define PARTID                   0x02
#define REVID                    0x03
#define STATUS                   0x04
#define FIFO_ENTRIES             0x05
#define TEMP2                    0x06
#define TEMP1                    0x07
#define XDATA3                   0x08
#define XDATA2                   0x09
#define XDATA1                   0x0A
#define YDATA3                   0x0B
#define YDATA2                   0x0C
#define YDATA1                   0x0D
#define ZDATA3                   0x0E
#define ZDATA2                   0x0F
#define ZDATA1                   0x10
#define FIFO_DATA                0x11
#define OFFSET_X_H               0x1E
#define OFFSET_X_L               0x1F
#define OFFSET_Y_H               0x20
#define OFFSET_Y_L               0x21
#define OFFSET_Z_H               0x22
#define OFFSET_Z_L               0x23
#define ACT_EN                   0x24
#define ACT_THRESH_H             0x25
#define ACT_THRESH_L             0x26
#define ACT_COUNT                0x27
#define FILTER                   0x28
#define FIFO_SAMPLES             0x29
#define INT_MAP                  0x2A
#define SYNC                     0x2B
#define RANGE                    0x2C
#define POWER_CTL                0x2D
#define SELF_TEST                0x2E
#define RESET                    0x2F

/************* Configuration paramenters ****************/
#define ADXL357_TEMP_BIAS		(float) 1852.0  /*Acceleromenter nominal intercept at 25C*/
#define ADXL357_TEMP_SLOPE		(float)	-9.05	/*Acceleromenter nominal slope*/

/* Accelerometer parameters */
#define ADXL_RANGE     2     /* ADXL362 sensitivity: 2, 4, 8 [g] */

#define ACT_VALUE          50     /* Activity threshold value */

#define INACT_VALUE        50     /* Inactivity threshold value */

#define ACT_TIMER          100    /* Activity timer value in ms */

#define INACT_TIMER        10     /* Inactivity timer value in seconds */


/****************************** Global Data ***********************************/

extern volatile int32_t i32SensorX;
extern volatile int32_t i32SensorY;
extern volatile int32_t i32SensorZ;

extern volatile uint32_t ui32SensorX;
extern volatile uint32_t ui32SensorY;
extern volatile uint32_t ui32SensorZ;
extern volatile uint32_t ui32SensorT;

/****************************** Functions prototypes ***********************************/

void ADXL357_Init(void);
void ADXL357_Start_Sensor(void);
void ADXL357_Stop_Sensor(void);
void ADXL357_Data_Scan(void);
int32_t ADXL357_Acceleration_Data_Conversion (uint32_t ui32SensorData);



#endif /* ADXL357_H*/