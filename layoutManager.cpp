// HEADER FILES
#include "layoutManager.h"
#include "serial.h"
#include "src/basics/timers.h"
#include "src/basics/io.h"
#include "variables.h"
#include "src/modules/DCC.h"
#include "turnouts.h"


uint8_t readSections(Section *currentSection, uint8_t direction) {

	if( direction == LEFT  && currentSection->leftSensor  == true ) {   // if LEFT sensor is made and train is moving LEFT
		uint8_t adjacentTurnout = currentSection->leftTurnout ;

		if( adjacentTurnout != NA ) {										// if there is a turnout
			if( turnout[ adjacentTurnout ].state ==   UP ) return currentSection->leftUp ;
			if( turnout[ adjacentTurnout ].state == DOWN ) return currentSection->leftDown ;
		}
		else {															// if there is no turnout
			return currentSection->leftUp ;										// return this value instead
		}
	}

	if( direction == RIGHT && currentSection->rightSensor == true ) {   // same code for driving towards right side
		uint8_t adjacentTurnout = currentSection->rightTurnout ;

		if( adjacentTurnout != NA ) {
			if( turnout[ adjacentTurnout ].state ==   UP ) return currentSection->rightUp ;
			if( turnout[ adjacentTurnout ].state == DOWN ) return currentSection->rightDown ;
		}
		else {
			return currentSection->rightUp ;
		}
	}

	return 0; // if neither sensor is made, return 0
}

/*************************************
 * Keeps track of a train's wareabout and controls all blind turnout if a train transitions to a new section.
*************************************/
void layoutManager(void) {
	for( int i = 0 ; i < nTrains ; i++ ) {

		// uint8_t currentSection = train[i].section;

		uint8_t newSection = readSections( &section[ currentSection ] , /*train[i].*/direction ); // poll sensors of every known occupied sector

		if( newSection > 0 ) {

			// train[i].section = newSection ; // if a sensor is true, the train may transition through sectors.
			uint8_t leftTurnout  = section[ currentSection ].leftTurnout ;
			uint8_t rightTurnout = section[ currentSection ].rightTurnout ;

			currentSection = newSection ;

			uint8_t blindTurnout = section[ newSection ].leftTurnoutBlind & 0x7F ; // last 7 bits holds the turnout number
			uint8_t state 		 = section[ newSection ].leftTurnoutBlind & 0x80 ; // first bit holds the state
			if( blindTurnout != NA ) setTurnout( blindTurnout, state ) ; 

			blindTurnout = section[ newSection ].rightTurnoutBlind & 0x7F ;
			state 		 = section[ newSection ].rightTurnoutBlind & 0x80 ;
			if( blindTurnout != NA ) setTurnout( blindTurnout, state ) ;
			//section[ currentSection ].rightTurnout = DOWN ;
		}
	}
}
	



