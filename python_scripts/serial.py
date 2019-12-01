# -*- coding: utf-8 -*-
"""
Created on Sun Dec  1 19:27:21 2019

@author: Renan
"""

import serial 
import struct
from matplotlib import pyplot as plt
from matplotlib import style
import numpy as np
from drawnow import *

esp32Serial = serial.Serial('COM6',115200)

SIZE_STUCT = 6000
acc_buffer = []

def plot():
    
    style.use('dark_background')
    
    f, subplot = plt.subplots(3, sharex = True)
    f.suptitle('Vibration Time domain')
    subplot[0].plot(acc_buffer[0:499], color = 'yellow')
    subplot[0].set_title('X axis')
    
    subplot[1].plot(acc_buffer[500:999])
    subplot[1].set_title('Y axis')
    
    subplot[2].plot(acc_buffer[1000:1499], color = 'red')
    subplot[2].set_title('Z axis')
    
    
while(True):
    #wait the data in port COM6
    while (esp32Serial.inWaiting()== 0):
        pass
    
    startByte = esp32Serial.read(1) 
    #print(startByte)
    if (startByte.decode('utf-8') == 'S'):
        data = esp32Serial.read(SIZE_STUCT)
        stopByte = esp32Serial.read(1)
        if (stopByte.decode('utf-8') == 'E'):
             #Struct holds float arrays var (6KBytes / 4 Bytes) = 1500
             new_values = struct.unpack('1500f',data)
             print(new_values)
             acc_buffer = np.array(new_values)
             drawnow(plot)
            
             
             