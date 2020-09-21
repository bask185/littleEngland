#include "variables.h"

Section section[ sectionAmount ] = {	// this table is the layout
	( NA, NA,    NA, NA,    NA, NA,   NA, NA ), //  dummy
	( NA,  1,    NA,  2,    NA, NA,    4,  5 ), //  1
	( NA,  1,    NA,  2,    NA, NA,    4,  5 ), //  2
	( NA,  3,    NA,  3,    NA, NA,    6,  7 ), //  3
	(  1,  5,     1,  5,     1,  2,    9,  8 ), //  4
	(  1,  4,     1, NA,     1,  2,   10, NA ), //  5
	(  3,  4,    NA, NA,     3, NA,   10, NA ), //  6
	(  3, NA,    NA, NA,     3, NA,   NA, NA ), //  7
	(  5, NA,    NA, NA,     4, NA,   NA, NA ), //  8
	(  5,  6,    NA,  6,     4, NA,   11, 12 ), //  9
	(  4,  2,     4, NA,     5,  6,   13, NA ), // 10
	(  6, NA,    NA, NA,     9, NA,   NA, NA ), // 11
	(  6,  2,    NA, NA,     9, NA,   13, NA ), // 12
	(  2, NA,     7, NA,    10, 12,   NA, NA )  // 13
}; 

// Train train[nTrains];
Turnout turnout[ turnoutAmount ] = {
	( .ID = 1 , .frog = 0),
	( .ID = 2 , .frog = 1),
	( .ID = 3 , .frog = 2),
	( .ID = 4 , .frog = 3),
	( .ID = 5 , .frog = 4),
	( .ID = 6 , .frog = 5),
	( .ID = 7 , .frog = 6)
};



// 	uint8_t ID : 1;
// 	uint8_t state : 1;
// 	uint8_t frog;

unsigned int previousSample[6];