#include <Arduino.h>

//#define nTrains 3


#define RIGHT 1 
#define LEFT 2 

#define STRAIGHT 1
#define CURVED  2
#define NA 0

#define sectionAmount 14
#define turnoutAmount 16

typedef struct Section {
	uint8_t leftSensor    :  4; // 16 max
	uint8_t rightSensor   :  4; // 16 max 
	uint8_t leftTurnout   :  4; // 16 max
	uint8_t rightTurnout  :  4; // 16 max
	uint8_t leftStraight  :  4; // 16 max
	uint8_t leftCurved    :  4; // 16 max
	uint8_t rightStraight :  4; // 16 max
	uint8_t rightCurved   :  4; // 16 max
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
