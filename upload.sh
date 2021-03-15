#!/bin/bash

# python.exe updateTimers.py
# python.exe updateIO.py

# #-p "COM4"
# arduino-cli compile  -b arduino:avr:nano ~/Documents/software/rosedaleAbbey
# arduino-cli upload -b arduino:avr:nano:cpu=atmega328old -p "COM3" -i ~/Documents/software/rosedaleAbbey/rosedaleAbbey.arduino.avr.nano.hex

# rm *.hex *.elf
# putty.exe
# exit


python.exe updateTimers.py
python.exe updateIO.py

# REMOTE UPLOAD
#arduino-cli upload -b arduino:avr:nano:cpu=atmega328old -p /dev/ttyUSB0 -i /home/pi/temp/*.hex
#rm -r /home/pi/temp/*.hex

/C/Users/Gebruiker/Documents/arduino-cli14/arduino-cli compile -b arduino:avr:nano ~/Documents/software/rosedaleAbbey -e
scp build/arduino.avr.nano/rosedaleAbbey.ino.with_bootloader.hex pi@192.168.2.11:/home/pi/temp/build/arduino.avr.nano/temp.ino.hex


ssh pi@192.168.2.11 <<'ENDSSH'
#commands to run on remote host
/home/pi/upload.sh
ENDSSH
exit


# # LOCAL UPLOAD
# echo "COMPILING"
# arduino-cli compile -b arduino:avr:nano ~/Documents/software/DCC-MM2-controller
# #echo "COMPILING DONE"

# echo "UPLOADING"
# arduino-cli upload -b arduino:avr:nano:cpu=atmega328old -p COM4 -i ~/Documents/software/DCC-MM2-controller/DCC-MM2-controller.arduino.avr.nano.hex
# #echo "UPLOADING COMPLETE"
# rm *.hex *.elf
# exit
