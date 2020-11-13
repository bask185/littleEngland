#include <Arduino.h>

#define debug // ENABLE THIS FOR ENABLING DBUG TXTs

//const int nTrains = 1 ;
const int nSensors = 6 ;

const int RIGHT = 1 ; 
const int LEFT  = 2 ; 
const int UP = 0 ;
const int DOWN = 1 ;
const int NA = 255 ; 

const int MAXIMUM_CURRENT = 100 ; // 0.1A -> 10.8 ADC
const int UNDETERMENED = 0 ;
const int INIT_TASK = 255 ;
const int upperVal = 1006 ;
const int lowerVal = 18 ;
const int sectionAmount = 14 ;
const int turnoutAmount = 16 ;

typedef struct Section {
	uint8_t leftSensor  ;
	uint8_t rightSensor ; 
	uint8_t leftTurnout ;
	uint8_t rightTurnout ;
	uint8_t leftTurnoutBlind ;
	uint8_t rightTurnoutBlind ;
	uint8_t leftUp  ;
	uint8_t leftDown  ;
	uint8_t rightUp ;
	uint8_t rightDown ;
} Section;
extern Section section[ sectionAmount ];

typedef struct LDRs {
	uint8_t state ;
} LDR;
extern LDR ldr[ nSensors ] ; ;

// typedef struct Turnout {
// 	uint8_t ID : 1 ;
// 	uint8_t state : 1 ;
// 	uint8_t up ;
// 	uint8_t down ;
// 	uint8_t frog ;
// } Turnout;

// typedef struct Train {
// 	uint8_t direction : 1; 
// 	uint8_t speed : 7;
// 	uint8_t section : 4;
// } Train;


// extern Train train[nTrains];
//extern Turnout turnout[ turnoutAmount ];

//extern unsigned int previousSample[6];
extern uint8_t currentSection ;
extern uint8_t direction ;