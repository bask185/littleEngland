#include "variables.h"

LDR ldr[ nSensors ] ;

const int B = 0b10000000 ;
const int T = 0b00000000 ;
/*
      > leftSensor 
      |   > rightSensor  
      |   |
      |   |       > leftTurnout 									; these must jump by the buttons
      |   |       |   > rightTurnout									
      |   |       |   |
      |   |       |   |     > leftTurnoutBlind       			; 1st bit true = curved, false = straight
      |   |       |   |     |   > rightTurnoutBlind     			; blind turnouts must jump when train enters new section
      |   |       |   |     |   |     
      |   |       |   |     |   |       > leftUpSection        	      ; last 4 bytes contain information about which new section the train is heading for 
      |   |       |   |     |   |       |   > leftDownSection 
      |   |       |   |     |   |       |   |
      |   |       |   |     |   |       |   |     > rightUpSection 
      |   |       |   |     |   |       |   |     |   > rightDownSection
      |   |       |   |     |   |       |   |     |   |                  */
Section section[ sectionAmount ] = {	// this table is the layout
	NA, NA,    NA, NA,    NA,  NA,   NA, NA,   NA, NA , //  Dummy
	NA,  1,    NA,  2,   1|T,  NA,   NA, NA,    4,  5 , //  1
	NA,  1,    NA,  2,   1|B,  NA,   NA, NA,    4,  5 , //  2
	NA,  2,    NA,  5,    NA,  NA,   NA, NA,    6,  7 , //  3
	 1,  3,     1,  3,   2|T,  NA,    1,  2,    8,  9 , //  4
	 1,  4,     1, NA,   2|B, 4|T,    1,  2,   10, 10 , //  5
	 2,  4,    NA, NA,   5|T, 4|B,    3,  3,   10, 10 , //  6
	 2, NA,    NA, NA,    NA,  NA,    3,  3,   NA, NA , //  7
	 3, NA,     1, NA,    NA,  NA,    4,  4,   NA, NA , //  8
	 3,  6,    NA, NA,   3|B, 7|T,    4,  4,   11, 11 , //  9
	 4,  5,     4,  6,    NA, 7|B,    5,  6,   11, 12 , // 10
	 6, NA,     7, NA,   6|T,  NA,    9, 10,   NA, NA , // 11
	 5, NA,     4, NA,   6|B,  NA,   10, 10,   NA, NA , // 12
} ;



uint8_t currentSection = 2 ;
uint8_t direction ;

// 	uint8_t ID : 1;
// 	uint8_t state : 1;
// 	uint8_t frog;

//unsigned int previousSample[6];