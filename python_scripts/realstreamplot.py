# -*- coding: utf-8 -*-
"""
This program reads the serial port that ESP32 is connected to.
Slipt the string and retrive the values associated with the variation in magnetude
of the acceleration 

"""
import serial
import numpy
from matplotlib import pyplot as plt
from matplotlib import style
from drawnow import *
    
esp32Serial = serial.Serial('COM6',115200)
plt.ion()
 
x_buffer = []
y_buffer = []
z_buffer = []

def plot():
    
    style.use('dark_background')
    
    f, subplot = plt.subplots(3, sharex = True)
    f.suptitle('Vibration Time domain')
    subplot[0].plot(x_buffer, color = 'yellow')
    subplot[0].set_title('X axis')
    
    subplot[1].plot(y_buffer)
    subplot[1].set_title('Y axis')
    
    subplot[2].plot(z_buffer, color = 'red')
    subplot[2].set_title('Z axis')
    
    
        
 
#endless loop to read serial
while True:

#wait the data in port COM6
    while (esp32Serial.inWaiting()== 0):
        pass
        
    serial_in = esp32Serial.readline().decode('utf-8')
    
    buffer_xyz = serial_in.split(',')
     
    
    # cast string into a interger
    x = float(buffer_xyz[0]);
    y = float(buffer_xyz[1]);
    z = float(buffer_xyz[2]);         
    
    #append values to a array
    x_buffer.append(x)
    y_buffer.append(y)
    z_buffer.append(z)
   
    drawnow(plot)
    
    
    
            
 

