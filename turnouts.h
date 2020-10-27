#include <Arduino.h> 
#include "src/modules/Adafruit_PWMServoDriver.h"

extern void setTurnout( uint8_t, uint8_t ) ;
extern void initTurnouts() ;
extern void controlTurnouts() ;