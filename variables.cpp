#include "variables.h"

const int C = 0b10000000 ;
const int S = 0b00000000 ;
/*
      > leftSensor 
      |   > rightSensor  
      |   |
      |   |       > leftTurnout 									; these must jump by the buttons
      |   |       |   > rightTurnout									
      |   |       |   |
      |   |       |   |     > rightTurnoutBlind       			; 1st bit true = curved, false = straight
      |   |       |   |     |   > leftTurnoutBlind     			; blind turnouts must jump when train enters new section
      |   |       |   |     |   |     
      |   |       |   |     |   |       > leftStraightSection   	; last 4 bytes contain information about which new section the train is heading for 
      |   |       |   |     |   |       |   > leftCurvedSection 
      |   |       |   |     |   |       |   |
      |   |       |   |     |   |       |   |     > rightStraightSection 
      |   |       |   |     |   |       |   |     |   > rightCurvedSection
      |   |       |   |     |   |       |   |     |   |                  */
Section section[ sectionAmount ] = {	// this table is the layout
	( NA, NA,    NA, NA,    NA, NA,    NA, NA,   NA, NA ), //  dummy
	( NA,  1,    NA,  2,   1|C, NA,    NA, NA,    4,  5 ), //  1
	( NA,  1,    NA,  2,   1|S, NA,    NA, NA,    4,  5 ), //  2
	( NA,  2,    NA,  3,    NA, NA,    NA, NA,    7,  6 ), //  3
	(  1,  3,     1,  3,   2|C, NA,     2,  1,    8,  9 ), //  4
	(  1,  4,     1, NA,   2|S, 4|S,    2,  1,   10, 10 ), //  5
	(  2,  4,    NA, NA,   5|C, 4|C,    3,  3,   10, 10 ), //  6
	(  2, NA,    NA, NA,    NA, NA,     3,  3,   NA, NA ), //  7
	(  3, NA,     1, NA,    NA, NA,     4,  4,   NA, NA ), //  8
	(  3,  6,    NA,  6,   3|C, 7|C,    4,  4,   11, 11 ), //  9
	(  4,  5,     4,  6,    NA, NA,     5,  6,   11, 12 ), // 10
	(  6, NA,     7, NA,    NA, NA,    10, 9,    NA, NA ), // 11
	(  5, NA,     4, NA,   6|C, NA,    10, 10,   13, NA ), // 12
} ;



// 	uint8_t ID : 1;
// 	uint8_t state : 1;
// 	uint8_t frog;

unsigned int previousSample[6];