/*
 * ADXL357 Accelerometer 
 * Application to read 3 axis of ADXL357
 * using Arduino Framework in ESP32 Microcontroller
 */

#include <Arduino.h>
#include <adxl357b.h>




#define SERIAL Serial

#define CALI_BUF_LEN           15
#define CALI_INTERVAL_TIME     250
int32_t cali_buf[3][CALI_BUF_LEN];
int32_t cali_data[3];

float factory;

#define SAMPLE 				500


//Accelerometer array data
struct acc_struct_xyz{
	float x[SAMPLE];
	float y[SAMPLE];
	float z[SAMPLE];
}acc_data;

int acc_data_len = sizeof(acc_data);


long startTime, endTime;




Adxl357b  adxl357b;


int32_t deal_cali_buf(int32_t *buf)
{
	int32_t cali_val = 0;
	
	for(int i = 0;i < CALI_BUF_LEN;i++)
	{
		cali_val += buf[i];
	}
	cali_val = cali_val/CALI_BUF_LEN;
	return (int32_t)cali_val;
}


void calibration(void)
{
	int32_t x;
	//SERIAL.println("Please Place the module horizontally!");
	delay(1000);
	//SERIAL.println("Start calibration........");
	
	for(int i=0;i<CALI_BUF_LEN;i++)
	{
		if(adxl357b.checkDataReady())
		{
			if(adxl357b.readXYZAxisResultData(cali_buf[0][i],cali_buf[1][i],cali_buf[2][i]))
			{
			}
		}
		delay(CALI_INTERVAL_TIME);
		// SERIAL.print('.');
	}
	// SERIAL.println('.');
	for(int i=0;i<3;i++)
	{
		cali_data[i] =  deal_cali_buf(cali_buf[i]);
		//SERIAL.println(cali_data[i]);
	}
	x = (((cali_data[2] - cali_data[0]) + (cali_data[2] - cali_data[1]))/2);
	factory = 1.0 / (float)x;
	// SERIAL.println(x);
	//SERIAL.println("Calibration OK!!");
}






void setup(void)
{
	//uint8_t value = 0;
	//float t;
	
	SERIAL.begin(115200);
	Serial.println(acc_data_len);
	
	if(adxl357b.begin())
	{
		SERIAL.println("Can't detect ADXL357B device .");
		while(1);
	}
	//SERIAL.println("Init OK!");
	/*Set full scale range to ±40g*/
	adxl357b.setAdxlRange(TEN_G);
	/*Switch standby mode to measurement mode.*/
	adxl357b.setPowerCtr(0);
	delay(100);
	/*Read Uncalibration temperature.*/
	//adxl357b.readTemperature(t);
	
	//SERIAL.print("Uncalibration  temp = ");
	//SERIAL.println(t);
	/**/
	calibration();

	
	

}


void loop(void)
{
	int32_t x,y,z;
	float cycleTime;
	//uint8_t entry = 0;
	startTime = millis();
	for(int i=0; i<SAMPLE; i++)
	{
		
		if(adxl357b.checkDataReady())
		{
			
			if(!adxl357b.readXYZAxisResultData(x,y,z))
			{
				acc_data.x[i] = (x * factory);
				acc_data.y[i] = y * factory;
				acc_data.z[i] = z * factory;
			}
						
		}
		delay(10);
	}
	endTime = millis();
	cycleTime = 1.0*(endTime-startTime)/SAMPLE;

	//Send accelerometer array to serial interface
	Serial.write('S'); //send start byte
	Serial.write((uint8_t *)&acc_data, acc_data_len);
	Serial.write('E');//send end byte

	// Serial.print("Sampling period =");
	// Serial.println(cycleTime);
	// Serial.print("Total time:");
	// Serial.println(endTime-startTime);
	
}
