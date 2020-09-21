#!/bin/bash

python.exe updateTimers.py
python.exe updateIO.py

#-p "COM4"
arduino-cli compile  -b arduino:avr:nano ~/Documents/software/littleEngland
arduino-cli upload -b arduino:avr:nano:cpu=atmega328old -p "COM3" -i ~/Documents/software/littleEngland/littleEngland.arduino.avr.nano.hex

rm *.hex *.elf
putty.exe
exit

