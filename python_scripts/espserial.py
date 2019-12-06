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
import dft



SIZE_STUCT = 6144
acc_buffer = []



def plot():
    
    style.use('dark_background')
    
    f, subplot = plt.subplots(3, sharex = True)
    f.suptitle('Vibration Time domain')
    subplot[0].plot(acc_buffer[0:511], color = 'yellow')
    subplot[0].set_title('X axis')
    
    subplot[1].plot(acc_buffer[512:1023])
    subplot[1].set_title('Y axis')
    
    subplot[2].plot(acc_buffer[1024:1535], color = 'red')
    subplot[2].set_title('Z axis')

try:
    esp32Serial = serial.Serial('COM6',115200)   
  
    
    while(True):
        #wait till data in port COM6 is available
        while (esp32Serial.inWaiting()== 0):
            pass
        
        print("New package received")
        
        startByte = esp32Serial.read(1) 
        #print(startByte)
        if (startByte.decode('utf-8') == 'S'):
            data = esp32Serial.read(SIZE_STUCT)
            stopByte = esp32Serial.read(1)
            if (stopByte.decode('utf-8') == 'E'):
                 #Struct holds float arrays var (6KBytes / 4 Bytes) = 1500
                 new_values = struct.unpack('1536f',data)
                 #print(new_values)
                 acc_buffer = np.array(new_values)
                 #drawnow(plot)
                 #dft.calc_dft(acc_buffer[0:499], "X axis")
                 #dft.calc_dft(acc_buffer[500:999], "Y axis")
                 dft.calc_dft(acc_buffer[1024:1535], "Z axis")
                 
except serial.SerialException:
    serial.Serial('COM6',115200).close()
            
             
             