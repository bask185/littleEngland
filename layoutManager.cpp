// HEADER FILES
#include "layoutManager.h"
#include "serial.h"
#include "src/basics/timers.h"
#include "src/basics/io.h"
#include "variables.h"
#include "src/modules/DCC.h"
#include "turnouts.h"

uint8_t readSections(Section *currentSection ) {

	uint8_t sensorNumber = currentSection->leftSensor ;
	uint8_t sensorState = ldr[ sensorNumber ].state ;

	if( direction == LEFT  && sensorState == true ) {   // if LEFT sensor is made and train is moving LEFT
		ldr[ sensorNumber ].state = false ;
		uint8_t adjacentTurnout = currentSection->leftTurnout - 1;

		#ifdef debug
		Serial.println("transitioning");
		Serial.print("adjacent turnout = ");Serial.println(adjacentTurnout);
		#endif

		if( adjacentTurnout != NA ) {										// if there is a turnout

			uint8_t adjacentTurnoutState = turnout[adjacentTurnout].state ;

			if( adjacentTurnoutState ==   UP ) {
				#ifdef debug
				Serial.print("new section = " ) ; Serial.println(currentSection->leftUp);
				#endif
				return currentSection->leftUp ;
			}
			if( adjacentTurnoutState == DOWN ) {
				#ifdef debug
				Serial.print("new section = " ) ; Serial.println(currentSection->leftDown);
				#endif
				return currentSection->leftDown ;
			}
		}
		else {		
			#ifdef debug
				Serial.print("new section = " ) ; Serial.println(currentSection->leftUp);
				#endif													// if there is no turnout
			return currentSection->leftUp ;								// return this value instead
		}
	}

	sensorNumber = currentSection->rightSensor ;
	sensorState = ldr[ sensorNumber].state ;

	if( direction == RIGHT && sensorState == true ) {   // same code for driving towards right side
		ldr[ sensorNumber ].state = false ;
		uint8_t adjacentTurnout = currentSection->rightTurnout - 1 ;

		//#ifdef debug
		Serial.print(F(" transitioning... "));
		Serial.print("adjacent turnout = ");Serial.println(adjacentTurnout);
		//#endif



		if( adjacentTurnout != NA ) {

			uint8_t adjacentTurnoutState ;
			adjacentTurnoutState = turnout[adjacentTurnout].state ;
			Serial.println( adjacentTurnoutState ) ;

			if( adjacentTurnoutState ==   UP ){
				#ifdef debug
				Serial.print("new section = " ) ; Serial.println(currentSection->rightUp);
				#endif
				return currentSection->rightUp ;
			}
			if( adjacentTurnoutState == DOWN ) {
				#ifdef debug
				Serial.print("new section = " ) ; Serial.println(currentSection->rightDown);
				#endif
				return currentSection->rightDown ;
			}
		}
		else {
			#ifdef debug
			Serial.print("new section = " ) ; Serial.println(currentSection->rightUp);
			#endif
			return currentSection->rightUp ;
		}
	}

	return 0; // if neither sensor is made, return 0
}

/*************************************
 * Keeps track of a train's wareabout and controls all blind turnout if a train transitions to a new section.
*************************************/
void layoutManager(void) {

	uint8_t newSection = readSections( &section[ currentSection ] ); // poll sensors of every known occupied sector

	if( newSection > 0 ) {
		#ifdef debug
		Serial.println("new section entered");
		#endif

		// train[i].section = newSection ; // if a sensor is true, the train may transition through sectors.
		//uint8_t leftTurnout  = section[ currentSection ].leftTurnout ; // does not do anything
		//uint8_t rightTurnout = section[ currentSection ].rightTurnout ;

		currentSection = newSection ;

		uint8_t blindTurnout =   section[ newSection ].leftTurnoutBlind & 0x7F ; // last 7 bits holds the turnout number
		uint8_t state 		 = ( section[ newSection ].leftTurnoutBlind & 0x80 ) >> 7 ; // first bit holds the state
		if( blindTurnout != NA ) setTurnout( blindTurnout, state ) ; 

		blindTurnout =   section[ newSection ].rightTurnoutBlind & 0x7F ;
		state 		 = ( section[ newSection ].rightTurnoutBlind & 0x80 ) >> 7 ;
		if( blindTurnout != NA ) setTurnout( blindTurnout, state ) ;
		//section[ currentSection ].rightTurnout = DOWN ;
	}
}
	



