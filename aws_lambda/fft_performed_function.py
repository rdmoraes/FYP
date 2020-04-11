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
    
    
    #extract payload from raw data as string and remove newline characters
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
    
    #Take FFT Magnitude of signal, and convert np array to a list 
    fft_mag_x =np.abs(fft(x, n=128)).tolist()
    fft_mag_y =np.abs(fft(y, n=128)).tolist()
    fft_mag_z =np.abs(fft(z, n=128)).tolist()
    
    
    #insert new data into dynamo
    table.put_item(
        Item={
            'id': str(int(event['Records'][0]['dynamodb']['ApproximateCreationDateTime'])),
            'x_axis': json.dumps(fft_mag_x),
            'y_axis': json.dumps(fft_mag_y),
            'z_axis': json.dumps(fft_mag_z)
            
        }
    )
    
    #Updates last reading in this partition key 
    table.put_item(
        Item={
            'id': 'lastResult',
            'x_axis': json.dumps(fft_mag_x),
            'y_axis': json.dumps(fft_mag_y),
            'z_axis': json.dumps(fft_mag_z),
            'x_axis_raw': json.dumps(x_axis),
            'y_axis_raw': json.dumps(y_axis),
            'z_axis_raw': json.dumps(z_axis)
    
        }
    )
    
    return{
        'statusCode' : 200,
        'body': 'FFT successfully performed, populating database'
    }
