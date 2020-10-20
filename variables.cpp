#include "variables.h"


//leftSensor rightSensor  leftTurnout rightTurnout leftStraight leftCurved rightStraight rightCurved
Section section[ sectionAmount ] = {	// this table is the layout
	( NA, NA,    NA | pos3,  NA | pos2,    NA, NA,   NA, NA ), //  dummy
	( NA,  1,    NA | pos3,   2 | pos2,    NA, NA,    4,  5 ), //  1
	( NA,  1,    NA | pos2,   2 | pos2,    NA, NA,    4,  5 ), //  2
	( NA,  3,    NA | pos2,   3 | pos2,    NA, NA,    6,  7 ), //  3
	(  1,  5,     1 | pos1,   5 | pos2,     1,  2,    9,  8 ), //  4
	(  1,  4,     1 | pos1,  NA | pos2,     1,  2,   10, NA ), //  5
	(  3,  4,    NA | pos2,  NA | pos2,     3, NA,   10, NA ), //  6
	(  3, NA,    NA | pos2,  NA | pos2,     3, NA,   NA, NA ), //  7
	(  5, NA,    NA | pos3,  NA | pos2,     4, NA,   NA, NA ), //  8
	(  5,  6,    NA | pos3,   6 | pos2,     4, NA,   11, 12 ), //  9
	(  4,  2,     4 | pos1,  NA | pos2,     5,  6,   13, NA ), // 10
	(  6, NA,    NA | pos3,  NA | pos2,     9, NA,   NA, NA ), // 11
	(  6,  2,    NA | pos2,  NA | pos2,     9, NA,   13, NA ), // 12
	(  2, NA,     7 | pos1,  NA | pos2,    10, 12,   NA, NA )  // 13
}; 



// 	uint8_t ID : 1;
// 	uint8_t state : 1;
// 	uint8_t frog;

unsigned int previousSample[6];