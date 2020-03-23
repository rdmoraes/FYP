import json
from json import JSONEncoder
import scipy
from scipy.fftpack import fft
from scipy import signal
import numpy as np
import os       #enviroment variable
import boto3    #AWS sdk for lambda
from boto3.dynamodb.conditions import Key, Attr


dynamodb = boto3.resource('dynamodb')
raw_data_table =  os.environ['TABLE_RAW_DATA']
analised_data_table =  os.environ['TABLE_ANALISED_DATA']

table_fetch = dynamodb.Table(raw_data_table)
table_insert = dynamodb.Table(analised_data_table)


def lambda_handler(event, context):
    
    print(event)
    
    #Fetch last insertion from raw data
    response = table_fetch.query(
        KeyConditionExpression = Key('id').eq('2'))
    
    
    #extract payload as string and remove newline characters
    payload = response['Items'][0]['payload'].replace("\n", "")
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
    
    table_insert.put_item(
        Item={
            'id': '3',
            'x_axis': json.dumps(fft_mag_x),
            'y_axis': json.dumps(fft_mag_y),
            'z_axis': json.dumps(fft_mag_z)
            
        }
    )
    
    
    return{
        'statusCode' : 200,
        'body': 'Analysed data added'
    }
