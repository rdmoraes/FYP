#!/bin/bash

dmesg | grep 'cp210x ttyUSB0'

ls -l /dev/ttyUSB0

echo 'Change device serial access permittion'
sudo chmod 777 /dev/ttyUSB0

ls -l /dev/ttyUSB0

echo 'Execution espserial_linux.py ...'
python espserial_linux.py
