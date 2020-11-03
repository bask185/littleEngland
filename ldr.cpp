#include "ldr.h"
#include "variables.h"
#include "turnouts.h"

/************************************************
 * Reads in all LDR values and filters the output N.B. this function should be moved to ldr.cpp and it propable needs alot of adjustments but ldr's need to be tested first
************************************************/


#ifndef debug
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
		uint8_t b = Serial.read() ;

		uint8_t  leftTurnout = section[currentSection].leftTurnout ;
		uint8_t rightTurnout = section[currentSection].rightTurnout ;
		#ifdef debug

		#endif
		
		if( b >= '0' && b <= '9' ) {
			ldr[0].state = 0 ;
			ldr[1].state = 0 ;
			ldr[2].state = 0 ;
			ldr[3].state = 0 ;
			ldr[4].state = 0 ;
			ldr[5].state = 0 ;
			ldr[6].state = 0 ;
			b -= '0' ; 					// read in ascii '0' - '9' and convert them to decimal 0 - 9
			ldr[b].state = 1 ;			// in debug mode a sensor can be toggled by serial command
			#ifdef debug
			Serial.print( "sensor " );Serial.print( b ) ; Serial.println( " ON" ) ;
			#endif
			return 1 ;
		}
		else if( b == 'w' ) {
			if( direction == RIGHT ) { setTurnout( rightTurnout, UP ) ; }
			if( direction ==  LEFT ) { setTurnout(  leftTurnout, UP ) ; }
		}
		else if( b == 's' ) {
			if( direction == RIGHT ) { setTurnout( rightTurnout, DOWN ) ; }
			if( direction ==  LEFT ) { setTurnout(  leftTurnout, DOWN ) ; }
		}
		else if( b == 'a' ) {
			direction = LEFT ;
			Serial.println("direction = LEFT ");
		}
		else if( b == 'd' ) {
			direction = RIGHT ;
			Serial.println("direction = RIGHT ");
		}
	}
	return  0 ;
}



	

#endif
