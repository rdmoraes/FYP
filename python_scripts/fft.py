# -*- coding: utf-8 -*-
"""
Created on Fri Dec  6 22:14:12 2019

@author: Renan
"""
from matplotlib import pyplot as plt
from matplotlib import style
from scipy.fftpack import fft, ifft
import numpy as np



def perform_fft(sig_src, axles):
    
    freq_sig = fft(sig_src)
    
    magnitude = np.abs(freq_sig)
    
    style.use('fivethirtyeight')
    
    f, subplot = plt.subplots(3, sharex=True)
    f.suptitle("Fast Fourier Transform")
    
    plt.cla()
    subplot[0].plot()
    subplot[0].set_title('Input Signal ' + axles + ' (Time Domain)')
    
    subplot[1].plot(freq_sig)
    subplot[1].set_title("Input Signal Frequency Domain (FFT)")
    
    subplot[2].plot(magnitude)
    subplot[2].set_title("Magnitude")
    
    


