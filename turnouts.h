#include <Arduino.h> 
#include "src/modules/Adafruit_PWMServoDriver.h"
//#include "src/modules/ServoSweep.h"

#define nTurnouts 8

typedef struct Turnouts {
	uint8_t lowPos ; 
	uint8_t highPos ;
	uint8_t state ;
} ;
extern Turnouts turnout[nTurnouts] ;

//extern ServoSweep turnout[nTurnouts] ;
extern void setTurnout( uint8_t, uint8_t ) ;
extern void initTurnouts( ) ;
extern void updateFrog( ) ;
//extern void controlTurnouts() ;
//extern uint8_t getTurnoutState( uint8_t ) ;