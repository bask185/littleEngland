#include <Arduino.h> 
#include "src/modules/Adafruit_PWMServoDriver.h"
#include "src/modules/ServoSweep.h"

#define nTurnouts 7

extern ServoSweep turnout[nTurnouts] ;
extern void setTurnout( uint8_t, uint8_t ) ;
extern void initTurnouts() ;
extern void controlTurnouts() ;