#include "ldr.h"
#include "variables.h"

/************************************************
 * Reads in all LDR values and filters the output N.B. this function should be moved to ldr.cpp and it propable needs alot of adjustments but ldr's need to be tested first
************************************************/

//#define debug
#ifdef debug
uint8_t readLDR( uint8_t index ) {
	int16_t sample;
	int16_t difference;

	if ( !updateT ) { 
		updateT = 10; // !every 10 ms

		selectSensor( index );

		sample = analogRead( occupanceDetector );
		
		difference = sample - previousSample[ index ] ;
		if( difference < 0 ) difference = - difference ;

		previousSample[ index ] = sample;

		if( difference > 100 ) {
			return 1 ;
		}		
	}
	return  0 ;
}
#else
uint8_t readLDR( uint8_t index ) {
	if( Serial.available() > 0 ) {
		uint8_t b = Serial.read() - '0' ; // read in ascii '0' - '9' and convert them to decimal 0 - 9

		ldr[b].state ^= 1 ; // in debug mode a sensor can be toggled by serial command
	}
}



	

#endif
