#include "turnouts.h"
#include "src/basics/io.h"
#include "src/basics/timers.h"
#include "variables.h"


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
	servoDriver.setOscillatorFrequency(27000000);
	servoDriver.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

	
	turnout[0].lowPos =  70 ; turnout[0].highPos = 110 ; turnout[0].state = 1 ;
	turnout[1].lowPos =  70 ; turnout[1].highPos = 110 ; turnout[1].state = 1 ;
	turnout[2].lowPos = 110 ; turnout[2].highPos =  70 ; turnout[2].state = 1 ;
	turnout[3].lowPos =  70 ; turnout[3].highPos = 110 ; turnout[3].state = 1 ;
	turnout[4].lowPos =  70 ; turnout[4].highPos = 110 ; turnout[4].state = 1 ;
	turnout[5].lowPos = 110 ; turnout[5].highPos =  70 ; turnout[5].state = 1 ;
	turnout[6].lowPos = 110 ; turnout[6].highPos =  70 ; turnout[6].state = 1 ;

	for(byte j = 0 ; j < 7 ; j ++ ) {
		//turnout[j].begin();
		//Serial.print(turnout[j].lowPos);Serial.print(' ');Serial.print(turnout[j].highPos);Serial.print(' ');Serial.print(turnout[j].state);
		uint16_t us = map( 90, 0, 180, 120, 490 ); 			// map degrees to pulse lengths, numbers don't make sense but it works
		servoDriver.setPWM( j, 0, us ); 					// 90 degrees
		setTurnout( j, 1 ) ;
		delay( 500 );
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
	ldrDelay = 100 ;

	if( ID > 0 ) ID -- ; // 

	if( ID <= nTurnouts ) {

		turnout[ID].state = state ;

		uint8_t degrees ;

		//Serial.println(turnout[ID].lowPos);
		//Serial.println(turnout[ID].highPos);
		
		if( state ) degrees = turnout[ID].highPos;
		else 		degrees = turnout[ID].lowPos;

		uint16_t us = map( degrees, 0, 180, 120, 490 ); 			// map degrees to pulse lengths, numbers don't make sense but it works
		servoDriver.setPWM( ID, 0, us );

		#ifdef debug
		Serial.print(F("turnout "));Serial.print(ID+1);Serial.print(F(" set at ")); Serial.println( degrees );
		#endif
		delay(100);
		mcpWrite( ID, state ); // sets frog alike
	}
}


	