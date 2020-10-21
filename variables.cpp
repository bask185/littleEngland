#include "variables.h"


/*
        > leftSensor 
        |  > rightSensor  
        |  |
        |  |       > leftTurnout 									; these must jump by the buttons
        |  |       |  > rightTurnout									
        |  |       |  |
        |  |       |  |       > leftTurnoutBlind      			; 1st bit true = curved, false = straight
        |  |       |  |       |  > rightTurnoutBlind     			; blind turnouts must jump when train enters new section
        |  |       |  |       |  |     
        |  |       |  |       |  |      > leftUpSection   	; last 4 bytes contain information about which new section the train is heading for 
        |  |       |  |       |  |      |   > leftDownSection 
        |  |       |  |       |  |      |   |
        |  |       |  |       |  |      |   |     > rightUpSection 
        |  |       |  |       |  |      |   |     |    > rightDownSection
        |  |       |  |       |  |      |   |     |    |                  */
Section section[ sectionAmount ] = {	// this table is the layout
	( NA, NA,    NA, NA,    NA, NA,        NA, NA,   NA, NA ), //  dummy
	( NA,  1,    NA,  2,    NA,  1 | 0,    NA, NA,    4,  5 ), //  1
	( NA,  1,    NA,  2,    NA, NA,        NA, NA,    4,  5 ), //  2
	( NA,  3,    NA,  3,    NA, NA,        NA, NA,    6,  7 ), //  3
	(  1,  5,     1,  5,    NA, NA,         1,  2,    9,  8 ), //  4
	(  1,  4,     1, NA,    NA, NA,         1,  2,   10, NA ), //  5
	(  3,  4,    NA, NA,    NA, NA,         3, NA,   10, NA ), //  6
	(  3, NA,    NA, NA,    NA, NA,         3, NA,   NA, NA ), //  7
	(  5, NA,    NA, NA,    NA, NA,         4, NA,   NA, NA ), //  8
	(  5,  6,    NA,  6,    NA, NA,         4, NA,   11, 12 ), //  9
	(  4,  2,     4, NA,    NA, NA,         5,  6,   13, NA ), // 10
	(  6, NA,    NA, NA,    NA, NA,         9, NA,   NA, NA ), // 11
	(  6,  2,    NA, NA,    NA, NA,         9, NA,   13, NA ), // 12
	(  2, NA,     7, NA,    NA, NA,        10, 12,   NA, NA )  // 13
}; 

uint8_t currentSection ;
uint8_t direction ;

// 	uint8_t ID : 1;
// 	uint8_t state : 1;
// 	uint8_t frog;

unsigned int previousSample[6];