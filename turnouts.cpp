#include "turnouts.h"

Adafruit_PWMServoDriver servoDriver = Adafruit_PWMServoDriver();


//ServoSweep::ServoSweep( uint8_t _servoPin, uint8_t _min, uint8_t _max, uint8_t _turnOff,  uint8_t _speed ) { 
ServoSweep turnouts = {			// this sweep class merely calculates the positions and handles the frog relay
	(255,  45, 135, 0, 20 ), 	// the pca9685 device will handle the servo control signals
	(255,  45, 135, 0, 20 ),
	(255,  45, 135, 0, 20 ),
	(255,  45, 135, 0, 20 ),
	(255,  45, 135, 0, 20 ),
	(255,  45, 135, 0, 20 ),
	(255,  45, 135, 0, 20 ),
	(255,  45, 135, 0, 20 )
} ;


void initTurnouts() {
	
	for(byte j = 0 ; j < 7 ; j ++ ) {
		turnout[j].begin();
	}
	
	servoDriver.begin();
	servoDriver.setOscillatorFrequency(27000000);
	servoDriver.setPWMFreq(50);  // Analog servos run at ~50 Hz updates
}

/*********************************************
 * Handles the controll of all turnout servo's and frog juicer relays
 * The function makes use of the servoSweep library to slowly move all servo motors
*********************************************/
void controlTurnouts() {
	for( byte j = 0 ; j < nTurnouts ; j ++ ) {
		
		uint8_t newPos = turnout[j].sweep() ;						// calculate new servo position
		if( newPos ) {	// if a new position is received..
			
			uint8_t middle = turnout[j].getMiddle() ;
			if( newPos > middle ) mcpWrite( j , HIGH ) ;			// if servo is halfway, flip the frog
			else				  mcpWrite( j ,  LOW ) ;
			
			uint16_t us = map(newPos, 0, 180, 120, 490); 			// map degrees to pulse lengths
			servoDriver.setPWM(pin, 0, us);							// send I2C instruction for the servo
		}
	}
}
	

void setTurnout( uint8_t ID, uint8_t state ) {
	turnout[ ID ].setState( state ) ;
}
	