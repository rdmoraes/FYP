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
from scipy.fftpack import fft
from drawnow import drawnow 
#import dft
#import fft

plt.ion()
fig = plt.figure()

SIZE_STUCT = 6144
acc_buffer = []
x_axis = []
y_axis = []
z_axis = []

                 
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
    
    
def perform_fft():
    
    
    SAMPLING_RATE = 500    #sampling frequency rate (HZ)
    NUM_SAMPLES = 512
    NUM_FFT=450             #crop desirable fft
       
    fft_magnitude_x = np.abs(fft(x_axis, n=NUM_FFT))
    fft_magnitude_y = np.abs(fft(y_axis, n=NUM_FFT))
    fft_magnitude_z = np.abs(fft(z_axis, n=NUM_FFT))
    
    style.use('fivethirtyeight')
    
    ax1 = fig.add_subplot(1,2,2)
    ax1.set_title('Spectrum Analyser')
    ax1.set_ylabel("FFT magnitude (mg/Hz)")
    ax1.set_xlabel("Frequency (Hz)")
    ax1.set_xlim(0,SAMPLING_RATE)
    ax1.plot(fft_magnitude_x, label='x-axis')
    ax1.plot(fft_magnitude_y, label='y-axis')
    ax1.plot(fft_magnitude_z, label='z-axis')
    ax1.legend()
    
    ax2 = fig.add_subplot(1,2,1)
    ax2.set_title('Vibration - Time Domain')
    ax2.set_ylabel("Vibration (mg)")
    ax2.set_xlabel("Sample rate")
    ax2.set_xlim(0, NUM_SAMPLES)
    ax2.set_ylim(-2000,2000)
    ax2.plot(x_axis, label='x-axis')
    ax2.plot(y_axis, label='y-axis')
    ax2.plot(z_axis, label='z-axis')
    ax2.legend()
    
    
    
    
    """      
    fig, (ax1, ax2) = plt.subplots(1,2)
    fig.suptitle("Fast Fourier Transform")
    
    style.use('fivethirtyeight')
    ax1.clear()
    ax1.plot(sig_src, color='#e5ae38')
    ax1.set_title(axis + ' (Time Domain)',color='grey')
    ax1.set_ylabel("Vibration (mg)")
    ax1.set_xlabel("Number of samples")
    ax1.set_xlim(0, NUM_SAMPLES)
    ax1.set_ylim(-2000,2000)
    
    
    ax2.clear()
    ax2.plot(fft_magnitude, color='#fc4f30')
    ax2.set_title(axis + ' (Frequency Domain)',color = 'grey')
    ax2.set_ylabel("FFT magnitude (mg/Hz)")
    ax2.set_xlabel("Frequency (Hz)")
    ax2.set_xlim(1,SAMPLING_RATE)
    
    plt.show()
    """                
    
   
try:
    esp32Serial = serial.Serial('COM6',115200) 
    
    figManager = plt.get_current_fig_manager()
    figManager.full_screen_toggle()
  
    
    while(True):
        #wait till data in port COM6 is available
        while (esp32Serial.inWaiting()== 0):
            pass
        
        #print("New package received")
        
        startByte = esp32Serial.read(1) 
        #print(startByte)
        if (startByte.decode('utf-8') == 'S'):
            data = esp32Serial.read(SIZE_STUCT)
            stopByte = esp32Serial.read(1)
            if (stopByte.decode('utf-8') == 'E'):
                 
                #Struct holds float arrays var (6.1KBytes / 4 Bytes) = 1536
                 new_values = struct.unpack('1536f',data)
                 #print(new_values)
                 acc_buffer = np.array(new_values)
                 x_axis = acc_buffer[0:511]
                 y_axis = acc_buffer[512:1023]
                 z_axis = acc_buffer[1024:1535]
                 
                 drawnow(perform_fft)
                 plt.pause(0.001)
                 #perform_fft(acc_buffer[0:511], "X axis")
                 #perform_fft(acc_buffer[512:1023], "Y axis")
                 #perform_fft(acc_buffer[1024:1535], "Z axis")
                
                 
                 #dft.calc_dft(acc_buffer[0:511], )
                 #dft.calc_dft(acc_buffer[512:1023], "Y axis")
                 #dft.calc_dft(acc_buffer[1024:1535], "Z axis")
                 
except serial.SerialException:
    serial.Serial('COM6',115200).close()
            
             
             