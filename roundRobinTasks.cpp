
#include "roundRobinTasks.h"
#include "src/basics/io.h"
#include "src/basics/timers.h"
#include "layoutManager.h"
#include "variables.h"
#include "ldr.h"
#include "src/modules/weistra.h"
#include "src/modules/DCC.h"
#include "turnouts.h"


enum modes { analog, DCC1, DCC2, DCC3 };
uint8_t mode, newTrainSelected = 0;;


uint8_t selectedAddres ;

const int addresses[] = { 3, 4, 5 } ;

Weistra regelaar( trackPower );

#define debounceInterval 50
#define accelerationInterval 20
#define decelerationInterval 10
#define accelerating 1
#define decelerating 2
#define potmeterOffset 29
#define middlePos 550
#define potentioRange 400
#define maxSpeed 50
#define potentioThreshold 5
#define speedSteps 3
#define nSamples 5

void followSpeed(uint8_t speedTmp)
{
	static uint8_t speed ;

	if( !speedT ) {
		speedT = 200 ;

		static uint8_t speedPrev ;
		if( speedTmp < speed ) speed -- ;
		if( speedTmp > speed ) speed ++ ;

		if( speedPrev != speed )
		{	speedPrev  = speed ;
			regelaar.setSpeed( abs( speed ) );
		}
	}
}

/********************************
 * reads in the potentiometer and the 2 turnout buttons of the handcontroller
 * The pot value is recalculated to a speed  setpoint
 * A seperate acceleration speed and deceleration speed is than used to accel and break
 * The speed is than send to the train being analog or digital
********************************/

void handController() {
	//#ifndef debug
	static uint8_t speedState = accelerating, previousSpeed, sampleCount = 0, buttonPressed = 0;
	static uint16_t sample[nSamples] ;
	struct {
		uint8_t up : 1 ;
		uint8_t down : 1 ;
	 } turnoutButton = {0,0} ;

	if( !sampleT ) { sampleT = 50 ;		// take ADC sample x amount of time

		static int8_t speed = 0, speedSetPoint = 0, speedPrev = 50 ;
		uint8_t difference ;

		uint16_t sample1= analogRead( throttle ) ;
		
		uint8_t rightSwitch = section[ currentSection ].rightTurnout ;
		uint8_t  leftSwitch = section[ currentSection ].leftTurnout ;
		
		if( sample1 <= lowerVal ) {		// if turnout up is pressed	
			if( buttonPressed == 0 ) { buttonPressed = 1;
				Serial.println(F("Up button is pressed")) ;
				if( direction == RIGHT ) setTurnout( rightSwitch, UP ) ;
				if( direction ==  LEFT ) setTurnout(  leftSwitch, UP ) ;
			}
		} 
		else if( sample1 >= upperVal  ) {	// if turnout down is pressed
			if( buttonPressed == 0 ) { buttonPressed = 1;
				Serial.println(F("Down button is pressed")) ;
				if( direction == RIGHT ) setTurnout( rightSwitch, DOWN ) ;
				if( direction ==  LEFT ) setTurnout(  leftSwitch, DOWN ) ;
			}
		}
		else if( (sample1 > lowerVal + 50) && (sample1 < upperVal - 50) ) { // so: 300 < ADC sample <  800
			buttonPressed = 0 ;

			if( ++sampleCount == nSamples ) sampleCount = 0 ;	
			sample[ sampleCount ] = sample1 ;				// use sample1 to create an average to prevent visible adc spikes in train movement

			uint16_t average = 0 ;
			for( uint8_t i = 0 ; i < nSamples ; i ++ ) {
				average += sample[ i ] ;
			}
			average /= nSamples ;

			if( average < middlePos ) {				// set direction left or right
				digitalWrite( directionPin1,  LOW );
				digitalWrite( directionPin2, HIGH );
				direction =  LEFT ; 
			}
			else {	
				digitalWrite( directionPin1, HIGH );
				digitalWrite( directionPin2,  LOW );
				direction = RIGHT ; 
			}

			uint8_t speedTmp ;
			if(      average < middlePos - 50 ) { speedTmp = map( average, middlePos - 50, lowerVal + 50, 0, maxSpeed ) ; }
			else if( average > middlePos + 50 ) { speedTmp = map( average, middlePos + 50, upperVal - 50, 0, maxSpeed ) ; }
			else speed = 0 ;

			//speed /= ( speedSteps ) ; 	// reduces sensitivity  MIGHT NOT BE NEEDED
			//speed *= ( speedSteps ) ;
			followSpeed(speedTmp);
		}
	}
}
#undef middlePos 

/********************************
 * measures the current going to the tracks and keeps track of time
 * if a continous overload of 50ms has occured, the trackpower will be shut off
 * after 5 seconds a new attempt will be made to apply track power
********************************/
void shortCircuit() {
	static byte msCounter = 0;

	if( ( digitalRead( trackPower ) == HIGH )  
	/*&&  ( digitalRead( weistraPin ) == HIGH )*/ ) { // only detect for short circuit when the PWM pulse = HIGH

		if(!overloadT) {
			overloadT = 2; // 2ms interval
			uint16_t sample ;
			sample = analogRead(currentSensePin);
			#ifdef debug
			//Serial.println( sample ) ;
			#endif
			if(sample < MAXIMUM_CURRENT) {
				msCounter = 25 ; // 50ms total
				//Serial.println("DETECTING SHORT CIRCUIT!");
			}

			if( msCounter) { msCounter--; } // decrement every 2ms

			if(!msCounter) { 
				digitalWrite(trackPower, LOW); 
				//Serial.println(F("TRACK POWER DISABLED"));
				restartT = 50; // after 5s trackpower will be attemt to reenable power
			}
		}
	}

	if( restartT == 1 ) { 
		restartT = 0;
		digitalWrite( trackPower, HIGH ); 
		//Serial.println(F("TRACK POWER ENABLED"));
	}
}


const int Ntrains = 4 ;
/************************************************
 * reads in the train select button and pick the correct train to controll
*************************************************/
void selectTrain() {
	uint16_t sample;
	uint8_t difference;
	uint8_t newTrain;
	static uint16_t previousSample = 0;

	if( !selectTrainT ) { selectTrainT = 25; // 250ms interval
		//#ifndef debug
		sample = analogRead( trainSelector );
		//#endif

		if ( sample >= previousSample ) difference = sample - previousSample;
		if ( sample <  previousSample ) difference = previousSample - sample;
		previousSample = sample;

		if( difference > ( 1024 / Ntrains / 4 ) ) { // ADC range / Ntrains steps / 2 -> mininum step needed to accept new value
			
			newTrain = map ( sample, 0 ,1023, 0, Ntrains );	// we re-use "sample" to select the new train
			newTrain = constrain(newTrain , 0 , Ntrains - 1);

			//Serial.print("new selected train is "); //Serial.println( newTrain );
			//Serial.print("sample is: "); //Serial.println( sample );
			//Serial.print("difference is: "); //Serial.println( difference );

			if( newTrain != analog ) {
				selectedAddres = addresses[ newTrain - 1 ];
				setSpeed( selectedAddres, 28 ) ;
				Serial.print(F(" DIGITAL MODE ENABLED, ADDRESS: "));Serial.println( selectedAddres );
			}

			else {
				bitClear(TIMSK1,OCIE1B);	// turn of ISR when analog mode is enabled
				Serial.println(F(" PWM MODE ENABLED "));
			}

			
			#define newMode(x) case x: mode = x; break; 
			switch ( newTrain ) {
				newMode( analog );
				newMode( DCC1 );
				newMode( DCC2 );
				newMode( DCC3 );
			}
			newTrainSelected = 1;
		}
	}
}
/*******************************************
 * handles the Weistra controll method or the DCC method to controll all trains
*******************************************/
void trackSignals(){
	//if( mode == analog ) {
		if( restartT == 0 ) regelaar.update(); 	// analog mode
	//} 
	//else {
	//	DCCsignals( ); 		// digital mode (mode termines if this function should run)
	//}
}
/***************
 * flashed led13 as a sign of live.
 * blinks 1-4 times after a new train is picked for visual conformation
*************/
void flash13() {
	static uint8_t ticks = 0;

	if( newTrainSelected ) {
		if( !flashT ) { flashT = 5; // 0.5s
			PORTB ^= ( 1 << 5 );
			ticks++;

			if( ticks > mode * 2 + 2 ) {
				newTrainSelected = 0;
				PORTB &= ~ ( 1 << 5 );
				ticks = 0;
			}
		}
	} else {
		flashT = 10 ;
		PORTB ^= ( 1 << 5 );
	}

}


extern void processRoundRobinTasks(void) {
	static unsigned char taskCounter = INIT_TASK - 1;

	

	// HIGH PRIORITY TASKS
	trackSignals();

	switch(++taskCounter) {
	// INITIALIZE TASKS (runs once uppon booting)
		case INIT_TASK:
		//cli();
		initTurnouts();		// handle all servo motors and frog juicer
		regelaar.begin();	// weistra pwm control
		//initDCC();			// DCC signal control
		initLDR();
		delay(200);

		restartT = 50;
		//sei(); 
		//PORTD = ( PORTD & 0b1011 ) | 0b1000; // differentiate both direction pins
		break;

	// LOW PRIORITY TASKS
		default: taskCounter = 0;
		case 0: handController();	break;
		case 1: shortCircuit();		break;
		case 2:	readLDR();			break;
		case 3: layoutManager();	break;
		case 4: selectTrain();		break;
		case 5: flash13();			break;
		case 6: updateFrog();		break;
	}

	//Serial.println(taskCounter);
}