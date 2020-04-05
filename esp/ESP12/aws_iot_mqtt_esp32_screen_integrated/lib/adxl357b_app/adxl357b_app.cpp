

/*
 * ADXL357 Accelerometer 
 * Application to read 3 axis of ADXL357
 * using Arduino Framework in ESP32 Microcontroller
 * 
 * Reference Seed ADLX357 Example
 */

#include <Arduino.h>
#include <adxl357b.h>
#include <adxl357b_app.h>


int32_t cali_buf[N_AXIS ][CALI_BUF_LEN];
int32_t cali_data[N_AXIS ];



float factory;
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
	
	Serial.println("Please place the module horizontally!");
	delay(1000);
	Serial.println("Start calibration........");
	
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
	
	for(int i=0;i<3;i++)
	{
		cali_data[i] =  deal_cali_buf(cali_buf[i]);
		
		#ifdef DEBUG
			Serial.println(cali_data[i]);
		#endif

	}
	x = (((cali_data[2] - cali_data[0]) + (cali_data[2] - cali_data[1]))/2);
	factory = 1.0 / (float)x;
	
	#ifdef DEBUG
		Serial.println(x);
	#endif
	Serial.println("Calibration OK!!");
	
}

int32_t startAccelerometer(void)
{
	float t;
	
	if(adxl357b.begin())
	{
		Serial.println("Can't detect ADXL357B device .");
		return -1;
		//while(1);
	}
	
	/*Set  scale range to ±10g*/
	adxl357b.setAdxlRange(TEN_G);
	/*Switch standby mode to measurement mode.*/
	adxl357b.setPowerCtr(0);
	delay(100);
	/*Read Uncalibration temperature.*/
	adxl357b.readTemperature(t);

	Serial.println("Accelerometer Init OK!");
	
	#ifdef DEBUG
		Serial.print("Uncalibration  temp = ");
		Serial.println(t);
	#endif
	
	/**/
	calibration();
	return 0;
}


void takeSamples(struct acc_struct_xyz *acc_data)
{
	int32_t x,y,z;
	
	//uint8_t entry = 0;
	startTime = millis();
	for(int i=0; i<N_SAMPLE; i++)
	{
		
		if(adxl357b.checkDataReady())
		{
			
			if(!adxl357b.readXYZAxisResultData(x,y,z))
			{
				acc_data->x[i] = x * factory * MG_UNIT;
				acc_data->y[i] = y * factory * MG_UNIT;
				acc_data->z[i] = z * factory * MG_UNIT;
			}
						
		}
		delay(1);
	}
	endTime = millis();
	
	//Send accelerometer array serial interface
	//Serial.write((uint8_t *)&acc_data, acc_data_len);
	#ifdef DEBUG
		float cycleTime;
		cycleTime = 1.0*(endTime-startTime)/SAMPLE;

		Serial.print("Sampling rate =");
	 	Serial.print(cycleTime);
		Serial.println("ms");
	 	Serial.print("Sampling time:");
	 	Serial.print(1.0* (endTime-startTime));
		Serial.println("ms");
	#endif
	
}
