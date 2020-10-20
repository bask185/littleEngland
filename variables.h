#include <Arduino.h>

//#define nTrains 3

const int pos0 = 0b00000000 ; // not connected to a turnout
const int pos1 = 0b01000000 ; // connected to the entry side of a turnout
const int pos2 = 0b10000000 ; // connected to the straight side
const int pos3 = 0b11000000 ; // connected to the curved side

#define RIGHT 1 
#define LEFT 2 

#define STRAIGHT 1
#define CURVED  2
#define NA 0

#define sectionAmount 14
#define turnoutAmount 16

typedef struct Section {
	uint8_t leftSensor    ;
	uint8_t rightSensor   ; 
	uint8_t leftTurnout   ;
	uint8_t rightTurnout  ;
	uint8_t leftStraight  ;
	uint8_t leftCurved    ;
	uint8_t rightStraight ;
	uint8_t rightCurved   ;
} Section;
extern Section section[ sectionAmount ];

typedef struct Turnout {
	uint8_t ID : 1;
	uint8_t state : 1;
	uint8_t curvedPostition;
	uint8_t straightPosition;
	uint8_t frog;
} Turnout;

// typedef struct Train {
// 	uint8_t direction : 1; 
// 	uint8_t speed : 7;
// 	uint8_t section : 4;
// } Train;


// extern Train train[nTrains];
extern Turnout turnout[ turnoutAmount ];

extern unsigned int previousSample[6];
