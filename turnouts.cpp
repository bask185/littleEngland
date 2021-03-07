#include "turnouts.h"
#include "src/basics/io.h"
#include "src/basics/timers.h"
#include "variables.h"

const int frogDelay = 100 ; 

Adafruit_PWMServoDriver servoDriver ;// = Adafruit_PWMServoDriver();


//ServoSweep::ServoSweep( uint8_t _servoPin, uint8_t _min, uint8_t _max, uint8_t _turnOff,  uint8_t _speed ) { 

				//    UP DOWN
// ServoSweep turnout[nTurnouts] {					// this sweep class merely calculates the positions and handles the frog relay
// 	ServoSweep( 255,  45, 135, 10, 0 ), 		// the pca9685 device will handle the servo control signals
// 	ServoSweep( 255,   0,  70, 10, 0 ),
// 	ServoSweep( 255,  90,  45, 10, 0 ),
// 	ServoSweep( 255,  95,  30, 10, 0 ),
// 	ServoSweep( 255,  70, 105, 10, 0 ),
// 	ServoSweep( 255,  60, 105, 10, 0 ),
// 	ServoSweep( 255, 150,  90, 10, 0 )
// } ;


Turnouts turnout[ nTurnouts ];
// Turnouts turnout[ nTurnouts ] = {
// 	(  45, 135, 1 ),
// 	(   0,  70, 1 ),
// 	(  90,  45, 1 ),
// 	(  95,  30, 1 ),
// 	(  70, 105, 1 ),
// 	(  60, 105, 1 ),
// 	( 150,  90, 1 )
// } ;



void initTurnouts() {
	servoDriver.begin();
	servoDriver.reset() ;
	servoDriver.setOscillatorFrequency(27000000);
	servoDriver.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

	
	turnout[0].lowPos = 87 ; turnout[0].highPos = 93 ; turnout[0].state = 1 ;
	turnout[1].lowPos = 87 ; turnout[1].highPos = 93 ; turnout[1].state = 1 ;
	turnout[2].lowPos = 87 ; turnout[2].highPos = 93 ; turnout[2].state = 1 ; // 3
	turnout[3].lowPos = 93 ; turnout[3].highPos = 87 ; turnout[3].state = 1 ; // 4
	turnout[4].lowPos = 87 ; turnout[4].highPos = 82 ; turnout[4].state = 1 ;
	turnout[5].lowPos = 93 ; turnout[5].highPos = 87 ; turnout[5].state = 1 ;
	turnout[6].lowPos = 87 ; turnout[6].highPos = 93 ; turnout[6].state = 1 ; // 7

	for(byte j = 1 ; j < 8 ; j ++ ) {
		//turnout[j].begin();
		//Serial.print(turnout[j].lowPos);Serial.print(' ');Serial.print(turnout[j].highPos);Serial.print(' ');Serial.print(turnout[j].state);
		
		uint16_t us = map( 90, 0, 180, 204, 409 ); 			// map degrees to pulse lengths, numbers don't make sense but it works
		// servoDriver.setPWM( j, 0, us ); 					// 90 degrees
		setTurnout( j, 0 ) ;
		frogT[j-1] = 255 ;
		while( frogT[j-1] != 0 ) { updateFrog(); } // wait before frog is flipped before setting next turnout
		Serial.println(F("next servo"));
		delay(300);
	}
}


/*********************************************
 * Handles the controll of all turnout servo's and frog juicer relays
 * The function makes use of the servoSweep library to slowly move all servo motors
*********************************************/
void controlTurnouts() {
	// for( uint8_t pin = 0 ; pin < nTurnouts ; pin ++ ) {
		
	// 	uint8_t newPos = turnout[pin].sweep() ;						// calculate new servo position
	// 	if( newPos ) {												// if a new position is received..
			
	// 		uint8_t middle = turnout[pin].getMiddle() ;
	// 		if( newPos > middle ) mcpWrite( pin , HIGH ) ;			// if servo is halfway, flip the frog
	// 		else				  mcpWrite( pin ,  LOW ) ;
			
	// 		uint16_t us = map( newPos, 0, 180, 120, 490 ); 			// map degrees to pulse lengths, numbers don't make sense but it works
	// 		servoDriver.setPWM( pin, 0, us );						// send I2C instruction for the servo
	// 	}
	// }
}

// uint8_t getTurnoutState( uint8_t ID ){
// 	if( ID > 0 ) ID -- ;
// 	Serial.print("ID of turnout = "); Serial.println( ID ) ;
// 	uint8_t retVal = turnout[ ID ].getState() ;
// 	Serial.println(retVal) ;
// 	return retVal ;
// }
	

void setTurnout( uint8_t ID, uint8_t state ) {	

	if( ID > 0 ) ID -- ; 	// I screwed up with 0 index system...

	if( ID <= nTurnouts ) {	// ignores non existing turnouts

		turnout[ID].state = state ;

		uint8_t degrees ;
		
		if( state ) degrees = turnout[ID].highPos;
		else 		degrees = turnout[ID].lowPos;

		uint16_t us = map( degrees, 0, 180, 120, 490 ); 			// map degrees to pulse lengths, numbers don't make sense but it works
		servoDriver.setPWM( ID, 0, us );

		#ifdef debug
		Serial.print(F("turnout "));Serial.print(ID+1);Serial.print(F(" set at ")); Serial.println( degrees );
		#endif
		//delay(100);
		
		frogT[ID] = frogDelay ;
	}
}


void updateFrog() {
	for ( uint8_t i = 0 ; i < 8 ; i ++ ) {
		if( frogT[i] == 1 ) {					// if frog timer reaches 1
			frogT[i] = 0 ;						// set timer to 0, to prevent repetetive action
			
			uint8_t state = turnout[i].state;	// fetch the state
			mcpWrite( i , state ) ;				// flip the frog
			Serial.print("flipping frog #");Serial.println(i+1);
		}
	}
}