#include "turnouts.h"
#include "src/basics/io.h"
#include "variables.h"

Adafruit_PWMServoDriver servoDriver ;// = Adafruit_PWMServoDriver();


//ServoSweep::ServoSweep( uint8_t _servoPin, uint8_t _min, uint8_t _max, uint8_t _turnOff,  uint8_t _speed ) { 
ServoSweep turnout[nTurnouts] {					// this sweep class merely calculates the positions and handles the frog relay
	ServoSweep( 255,  45, 135, 0, 20 ), 		// the pca9685 device will handle the servo control signals
	ServoSweep( 255,  45, 135, 0, 20 ),
	ServoSweep( 255,  45, 135, 0, 20 ),
	ServoSweep( 255,  45, 135, 0, 20 ),
	ServoSweep( 255,  45, 135, 0, 20 ),
	ServoSweep( 255,  45, 135, 0, 20 ),
	ServoSweep( 255,  45, 135, 0, 20 )
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
	for( uint8_t pin = 0 ; pin < nTurnouts ; pin ++ ) {
		
		uint8_t newPos = turnout[pin].sweep() ;						// calculate new servo position
		if( newPos ) {												// if a new position is received..
			
			uint8_t middle = turnout[pin].getMiddle() ;
			if( newPos > middle ) mcpWrite( pin , HIGH ) ;			// if servo is halfway, flip the frog
			else				  mcpWrite( pin ,  LOW ) ;
			
			uint16_t us = map( newPos, 0, 180, 120, 490 ); 			// map degrees to pulse lengths
			servoDriver.setPWM( pin, 0, us );						// send I2C instruction for the servo
		}
	}
}
	

void setTurnout( uint8_t ID, uint8_t state ) {
	if(ID > 0 && ID <= nTurnouts ) {
		#ifdef debug
		Serial.print("turnout ");Serial.print(ID);Serial.print(" is "); Serial.println(state);
		#endif
		turnout[ ID ].setState( state ) ;
	}
}
	