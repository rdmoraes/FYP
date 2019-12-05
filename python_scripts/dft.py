# -*- coding: utf-8 -*-
"""
Script to perform a discrete fourier transform

Created on Wed Dec  4 18:15:30 2019

@author: Renan
"""

from matplotlib import pyplot as plt
from matplotlib import style
import math

def calc_dft(sig_src, axies):
    #Resize complex array to N/2 where N =number of samples
    sig_imx = [None]*int((len(sig_src)/2))
    sig_rex = [None]*int((len(sig_src)/2))
    #List to store magnitude
    sig_mag = [None]*int((len(sig_src)/2))
    
    #fill the complex array with zero
    for r in range(int(len(sig_src)/2)):
        sig_rex[r] = 0
        sig_imx[r] = 0
    
    #Compute complex real and imaginary part of the signal    
    for j in range(int(len(sig_src)/2)):
        for i in range(len(sig_src)):
            sig_rex[j] = sig_rex[j] + sig_src[i]*math.cos(2*math.pi*j*i/len(sig_src))
            sig_imx[j] = sig_imx[j] - sig_src[i]*math.sin(2*math.pi*j*i/len(sig_src))
    
    for x in range(int(len(sig_src)/2)):
        sig_mag[x] = math.sqrt(math.pow(sig_rex[x], 2) + math.pow(sig_imx[x], 2))
            
    style.use('ggplot')
    
    f,subplot = plt.subplots(4, sharex=True)
    f.suptitle("Discrete Fourier Transform")
    
    subplot[0].plot(sig_src, color='blue')
    subplot[0].set_title(axies +"Input Signal")
    #subplot.ylim(0,2)
    
    subplot[1].plot(sig_rex, color='green')
    subplot[1].set_title(axies +"Frequency Domain (Real)")

    subplot[2].plot(sig_imx, color='green')
    subplot[2].set_title(axies +"Frequency Domain (Imaginary)") 
    
    subplot[3].plot(sig_mag, color='red')
    subplot[3].set_title(axies +"Frequency Domaing Magnitude")  
    
    plt.show()

