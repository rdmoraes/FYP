'''
  This funciton is triggered by DynamoDB Stream when new data is added to table 
  raw data. Performs filter and fft and stores new results in table analisedData
  
  Renan Moraes
'''

import json

import scipy
from scipy.fftpack import fft
from scipy import signal
import numpy as np

import os       #enviroment variable
import boto3    #AWS sdk for lambda



dynamodb = boto3.resource('dynamodb')
table_name =  os.environ['TABLE_ANALISED_DATA']


table = dynamodb.Table(table_name)

#Triggered when new data is insert into serverless table
def lambda_handler(event, context):
    
    n_sample =160
    sample_rate = 2000
    #extract payload from raw data as string and remove newline characters
    print(event['Records'])
    payload = event['Records'][0]['dynamodb']['NewImage']['payload']['S'].replace('\n', '')
    
    data = json.loads(payload)
    
    #Axis data
    x_axis = data['x_axis']
    y_axis = data['y_axis']
    z_axis = data['z_axis']
    
    #remove DC offset at 0Hz
    x = signal.detrend(x_axis)
    y = signal.detrend(y_axis)
    z = signal.detrend(z_axis)
    
    # Filter signal to reduce noise
    filtered_x = FIR_filter(x, 10)
    filtered_y = FIR_filter(y, 10)
    filtered_z = FIR_filter(z, 10)
    
    #Take FFT, total n. sample 160, and assign np array to a list  
    fft_mag_x =np.abs(fft(x, n=n_sample)).tolist()
    fft_mag_y =np.abs(fft(y, n=n_sample)).tolist()
    fft_mag_z =np.abs(fft(z, n=n_sample)).tolist()
    
    #Returns FFT sample frequencies
    freqs=np.fft.fftfreq(n_sample, 1/sample_rate).tolist()
    
    
    #insert new data into dynamo
    table.put_item(
        Item={
            'id': str(int(event['Records'][0]['dynamodb']['ApproximateCreationDateTime'])),
            'x_axis': json.dumps(fft_mag_x),
            'y_axis': json.dumps(fft_mag_y),
            'z_axis': json.dumps(fft_mag_z),
            'freqs': json.dumps(freqs)
            
        }
    )
    
    #Updates last reading in this partition key 
    table.put_item(
        Item={
            'id': 'lastResult',
            'x_axis': json.dumps(fft_mag_x),
            'y_axis': json.dumps(fft_mag_y),
            'z_axis': json.dumps(fft_mag_z),
            'freqs': json.dumps(freqs),
            'x_axis_raw': json.dumps(x_axis),
            'y_axis_raw': json.dumps(y_axis),
            'z_axis_raw': json.dumps(z_axis)
    
        }
    )
    
    return{
        'statusCode' : 200,
        'body': 'FFT successfully performed, populating database'
    }

def FIR_filter(data, windowing_size):
   
    
    #Hanning window to smooth data signal.
    taps = signal.hann(windowing_size)
    
    # Use lfilter to filter x with the FIR filter.
    data_filtered = np.convolve(data, taps)
    
    return data_filtered
    