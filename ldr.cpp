#include "ldr.h"
#include "variables.h"
#include "turnouts.h"
#include "src/basics/timers.h"
#include "src/basics/io.h"

/************************************************
 * Reads in all LDR values and filters the output N.B. this function should be moved to ldr.cpp and it propable needs alot of adjustments but ldr's need to be tested first
************************************************/


//#ifndef debug
const int threshold = 60 ;
const int ldrInterval = 100 ;

/* selects a channel of the multiplexer */
#define setMux(x,j,k,l) case x:digitalWrite( muxPin3,  j ); digitalWrite( muxPin2,  k ); digitalWrite( muxPin1,  l ); break;
void selectSensor(uint8_t nSensor) {
	switch( nSensor ) {
		setMux(0,  LOW, HIGH,  LOW); // 0
		setMux(1,  LOW,  LOW, HIGH); // 1
		setMux(2,  LOW,  LOW,  LOW); // 2
		setMux(3,  LOW, HIGH, HIGH); // 3
		setMux(4, HIGH,  LOW,  LOW); // 4
		setMux(5, HIGH, HIGH,  LOW); // 7 
		setMux(6, HIGH, HIGH, HIGH); // 5 
		setMux(7, HIGH,  LOW, HIGH); // 6 
	}
}

void readLDR( ) {
		if( !ldrDelay ) {
		
		int16_t sample;
		int16_t difference;
		static uint8_t index ; 
		static unsigned int previousSample[8];

		if ( !updateT ) { 
			updateT = ldrInterval ; // !every 20 ms sample time is 8 x 20ms = 160ms per sensor, 8 sensors are polled
			if( ++index == 8 ) index = 0 ;

			selectSensor( index );

			sample = analogRead( occupanceDetector );
			
			difference = sample - previousSample[ index ] ;
			if( difference < 0 ) difference = -difference ;

			previousSample[ index ] = sample;

			if( difference > threshold ) {
				ldr[index].state = 1 ;
				Serial.print(F("sensor ")) ; Serial.print(index );Serial.println(" true ");
			}
			else {
				ldr[index].state = 0 ;	
			}
		}
	}
}
// #else
// void readLDR(  ) {
// 	if( Serial.available() > 0 ) {
// 		uint8_t b = Serial.read() ;

// 		uint8_t  leftTurnout = section[currentSection].leftTurnout ;
// 		uint8_t rightTurnout = section[currentSection].rightTurnout ;
// 		#ifdef debug

// 		#endif
		
// 		if( b >= '0' && b <= '9' ) {
// 			ldr[0].state = 0 ;
// 			ldr[1].state = 0 ;
// 			ldr[2].state = 0 ;
// 			ldr[3].state = 0 ;
// 			ldr[4].state = 0 ;
// 			ldr[5].state = 0 ;
// 			ldr[6].state = 0 ;
// 			b -= '0' ; 					// read in ascii '0' - '9' and convert them to decimal 0 - 9
// 			ldr[b].state = 1 ;			// in debug mode a sensor can be toggled by serial command
// 			#ifdef debug
// 			Serial.print( "sensor " );Serial.print( b ) ; Serial.println( " ON" ) ;
// 			#endif
// 			return 1 ;
// 		}
// 		else if( b == 'w' ) {
// 			if( direction == RIGHT ) { setTurnout( rightTurnout, UP ) ; }
// 			if( direction ==  LEFT ) { setTurnout(  leftTurnout, UP ) ; }
// 		}
// 		else if( b == 's' ) {
// 			if( direction == RIGHT ) { setTurnout( rightTurnout, DOWN ) ; }
// 			if( direction ==  LEFT ) { setTurnout(  leftTurnout, DOWN ) ; }
// 		}
// 		else if( b == 'a' ) {
// 			direction = LEFT ;
// 			Serial.println("direction = LEFT ");
// 		}
// 		else if( b == 'd' ) {
// 			direction = RIGHT ;
// 			Serial.println("direction = RIGHT ");
// 		}
// 	}
// 	return  0 ;
// }
// #endif

void initLDR() {

	for( int i = 0 ; i < 8 ; i ++ ) {
		updateT = 0;	// cancel debounce time
		readLDR();		// first 8 polls will make all sensor values true
	}

	for( int i = 0 ; i < 8 ; i ++ ) {
		ldr[i].state = 0 ; // clear all sensor states
	}
	Serial.println(F("sensors initialized"));
}
	


