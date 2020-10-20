// HEADER FILES
#include "layoutManager.h"
#include "serial.h"
#include "src/basics/timers.h"
#include "src/basics/io.h"
#include "variables.h"
#include "src/modules/DCC.h"

// MACROS



// VARIABLES

/* een sectie heeft de volgende attributen:
	* een mogelijke linker sensor
	* een mogelijke rechter sensor
	* een mogelijke wissel links
	* een mogelijke wissel rechts
	* 2 mogelijke secties links
	* 2 mogelijke secties rechts

voorbeeld:
*/

uint8_t readSections(Section *currentSection, uint8_t direction) {

	if( direction == LEFT  && currentSection->leftSensor  == true ) {   // if LEFT sensor is made and train is moving LEFT
		uint8_t adjacentTurnout = currentSection->leftTurnout ;

		if( adjacentTurnout > 0 ) {										// if there is a turnout
			if( turnout[ adjacentTurnout ].state == STRAIGHT ) return currentSection->leftStraight ;
			if( turnout[ adjacentTurnout ].state == CURVED   ) return currentSection->leftCurved ;
		}
		else {															// if there is no turnout
			return currentSection->leftStraight ;										// return this value instead
		}
	}

	if( direction == RIGHT && currentSection->rightSensor == true ) {   // same code for driving towards right side
		uint8_t adjacentTurnout = currentSection->rightTurnout ;

		if( adjacentTurnout ) {
			if( turnout[ adjacentTurnout ].state == STRAIGHT ) return currentSection->rightStraight ;
			if( turnout[ adjacentTurnout ].state == CURVED   ) return currentSection->rightCurved ;
		}
		else {
			return currentSection->rightStraight ;
		}
	}

	return 0; // if neither sensor is made, return 0
}

#define nTrains 3
/*************************************
 * Keeps track of a train's wareabout and controlls all turnouts correctly.
*************************************/
void layoutManager(void) {
	for( int i = 0 ; i < nTrains ; i++ ) {

		uint8_t currentSection = train[i].section;

		uint8_t newSection = readSections( &section[ currentSection ] , train[i].direction ); // poll sensors of every known occupied sector
		currentSection = newSection ;

		if( newSection ) {
			train[i].section = newSection ; // if a sensor is true, the train may transition through sectors.
			uint8_t leftTurnout  = section[ currentSection ].leftTurnout ;
			uint8_t rightTurnout = section[ currentSection ].rightTurnout ;
			
			if( leftTurnout  & pos2 )  setTurnout( leftTurnout  & 0x7F, STRAIGHT );
			if( leftTurnout  & pos3 )  setTurnout( leftTurnout  & 0x7F,   CURVED );
			if( rightTurnout & pos2 )  setTurnout( rightTurnout & 0x7F, STRAIGHT );
			if( rightTurnout & pos3 )  setTurnout( rightTurnout & 0x7F,   CURVED );
		}
	}
}
	



