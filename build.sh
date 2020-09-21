#!/bin/bash

python.exe updateTimers.py
python.exe updateIO.py

# -p "COM4"
arduino-cli compile -b arduino:avr:nano ~/Documents/software/littleEngland
#arduino-cli upload -b arduino:avr:nano:oldbootloader -p COM4 -i ~/Documents/software/piTrain-decoder/piTrain-decoder.arduino.avr.nano.hex
#rm *.hex *.elf
exit

