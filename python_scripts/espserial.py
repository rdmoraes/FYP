# -*- coding: utf-8 -*-
"""
Created on Sun Dec  1 19:27:21 2019

This script reads the output of serial COM and perform the FFT 
from the data of the ADLX357 Accelerometer 

@author: Renan
"""

import serial  
import struct
from matplotlib import pyplot as plt
from matplotlib import style
import numpy as np
from scipy.fftpack import fft
from scipy import signal
from scipy.signal import kaiserord, lfilter, firwin, butter
#from matplotlib.animation import FuncAnimation
from drawnow import drawnow 
#import dft
#import fft

plt.ion()
#fig = plt.figure()

#SIZE_STUCT = 6144
#SIZE_STUCT = 600
SIZE_STUCT = 3072
acc_buffer = []
x_axis = []
y_axis = []
z_axis = []

#Kaizer wind
def FIR_filter(data):
    sample_rate = 2000
    nyq_rate = sample_rate/2.0
    
    # The desired width of the transition from pass to stop,
    # relative to the Nyquist rate.  We'll design the filter
    # with a 5 Hz transition width.
    width = 500.0/nyq_rate
    
    # The desired attenuation in the stop band, in dB.
    ripple_db = 60.0
    
    # Compute the order and Kaiser parameter for the FIR filter.
    N, beta = kaiserord(ripple_db, width)
    
    # The cutoff frequency of the filter.
    cutoff_hz = 200.0
    
    # Use firwin with a Kaiser window to create a lowpass FIR filter.
    taps = firwin(N, cutoff_hz/nyq_rate, window=('kaiser', beta))

    # Use lfilter to filter x with the FIR filter.
    data_filtered = lfilter(taps, 1.0, data)
    
    return data_filtered

def butter_bandpass(lowcut, highcut, fs, order=5):
    nyq = 0.5 * fs
    low = lowcut / nyq
    high = highcut / nyq
    b, a = butter(order, [low, high], btype='band')
    return b, a

def butter_bandpass_filter(data, lowcut, highcut, fs, order=5):
    b, a = butter_bandpass(lowcut, highcut, fs, order=order)
    y = lfilter(b, a, data)
    return y

def moving_average(data,size):
    #Define mask and store as an array
    #mask=np.ones((1,window))/window
    #mask=mask[0,:]
    
    #Hann window
    window = signal.hann(size)
    
    #Hamming window
    #window = signal.hamming(size)
    
    #Convolve the mask with the raw data
    convolved_data = np.convolve(data,window,'same')

    return convolved_data
   
def perform_fft():
    
    
    
    NUM_SAMPLES = 256       
    NUM_FFT= 256          #crop desirable n. fft
    LOW_CUTOFF = 10
    HIGH_CUTOFF = 600
    FS = 2000
    
    # subtracts the DC offset 0Hz 
    # by removing the mean value
    x = signal.detrend(x_axis)
    y = signal.detrend(y_axis)
    z = signal.detrend(z_axis)
    
    #filtered_x = FIR_filter(x)
    #filtered_y = FIR_filter(y)
    #filtered_z = FIR_filter(z)
    
    filtered_x = moving_average(x, 10)
    filtered_y = moving_average(y, 10)
    filtered_z = moving_average(z, 10)
    
    #filtered_x = butter_bandpass_filter(x, LOW_CUTOFF,  HIGH_CUTOFF, FS, order=5)
    #filtered_y = butter_bandpass_filter(y, LOW_CUTOFF,  HIGH_CUTOFF, FS, order=5)
    #filtered_z = butter_bandpass_filter(z, LOW_CUTOFF,  HIGH_CUTOFF, FS, order=5)
    
    fft_abs_x = np.abs(fft(filtered_x, n=NUM_FFT))
    fft_abs_y = np.abs(fft(filtered_y, n=NUM_FFT))
    fft_abs_z = np.abs(fft(filtered_z, n=NUM_FFT))
    
    fft_db_x = 20*np.log10(fft_abs_x/NUM_SAMPLES)
    fft_db_y = 20*np.log10(fft_abs_y/NUM_SAMPLES)
    fft_db_z = 20*np.log10(fft_abs_z/NUM_SAMPLES)
    
    freqs=np.fft.fftfreq(NUM_FFT, 1/FS)
    freqs_abs = np.abs(freqs)
    
        
    style.use('fivethirtyeight')
    
    fig,(ax1, ax2) = plt.subplots(2, 1, figsize=(10, 10))
    
    #ax1 = fig.add_subplot(1,2,2)
    ax1.set_title('Spectrum Analyser')
    ax1.set_ylabel("FFT magnitude (mg/Hz)")
    ax1.set_xlabel("Frequency (Hz)")
    #ax1.set_ylim(0,40000)
    ax1.plot(freqs_abs, fft_abs_x, label='x-axis')
    ax1.plot(freqs_abs, fft_abs_y, label='y-axis')
    ax1.plot(freqs_abs, fft_abs_z, label='z-axis')
    ax1.legend()
    
    #ax2 = fig.add_subplot(1,2,1)
    ax2.set_title('Vibration - Time Domain')
    ax2.set_ylabel("Vibration (mg)")
    ax2.set_xlabel("t [ms]")
    ax2.set_xlim(0,NUM_SAMPLES)
    ax2.set_ylim(-300,300)
    ax2.plot(filtered_x, label='x-axis')
    ax2.plot(filtered_y, label='y-axis')
    ax2.plot(filtered_z, label='z-axis')
    ax2.legend()
    
    plt.subplots_adjust(hspace=0.75)
    plt.show()
    
 
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
                 
                #TEST for 512 samples
                #Struct holds float arrays var (6.1KBytes / 4 Bytes) = 1536
                 #new_values = struct.unpack('1536f',data)
                 
                 #acc_buffer = np.array(new_values)
                 #x_axis = acc_buffer[0:511]
                # y_axis = acc_buffer[512:1023]
                 #z_axis = acc_buffer[1024:1535]
                 
                 #TEST for 50 samples
                 #Struct holds float arrays var (600Bytes / 4 Bytes) = 300
                 #new_values = struct.unpack('150f',data)
                 
                 #acc_buffer = np.array(new_values)
                 #x_axis = acc_buffer[0:49]
                 #y_axis = acc_buffer[50:99]
                 #z_axis = acc_buffer[100:149]
                 
                  #TEST for 100 samples
                 #Struct holds float arrays var (3072Bytes / 4 Bytes) = 768
                 new_values = struct.unpack('768f',data)
                 
                 acc_buffer = np.array(new_values)
                 x_axis = acc_buffer[0:255]
                 y_axis = acc_buffer[256:511]
                 z_axis = acc_buffer[512:767]
                 
                 #ani = FuncAnimation(plt.gcf(), perform_fft, interval=500)
                 
                 print("heeyyy")
                 
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
            
             
             