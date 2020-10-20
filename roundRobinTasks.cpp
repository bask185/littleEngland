
#include "roundRobinTasks.h"
#include "src/basics/io.h"
#include "src/basics/timers.h"
#include "layoutManager.h"
#include "variables.h"
#include "src/modules/weistra.h"
#include "src/modules/DCC.h"


// HIGH PRIORITY ROUND ROBIN TASKS
	//readSerialBus();
	//readLDR();

#define MAXIMUM_CURRENT 200

#define UNDETERMENED 0
#define STRAIGHT 1
#define CURVED 2

#define INIT_TASK 255


#define upperVal 1013
#define lowerVal 11

struct sensors {
	uint16_t occupied : 1;
	uint16_t occupanceValue : 10;
};
uint8_t nextTurnout;

enum modes { analog, DCC1, DCC2, DCC3 };
uint8_t mode, newTrainSelected = 0;;

sensors sensor[6];

Weistra regelaar( trackPower );

#define setMux(x,j,k,l) case x:digitalWrite( muxPin1,  j ); digitalWrite( muxPin2,  k ); digitalWrite( muxPin3,  l ); break;
/* selects a channel of the multiplexer */
void selectSensor(uint8_t nSensor) {
	switch( nSensor ) {
		setMux(0,  LOW,  LOW,  LOW);
		setMux(1,  LOW,  LOW, HIGH);
		setMux(2,  LOW, HIGH,  LOW);
		setMux(3,  LOW, HIGH, HIGH);
		setMux(4, HIGH,  LOW,  LOW);
		setMux(5, HIGH,  LOW, HIGH);
		setMux(6, HIGH, HIGH,  LOW);
	}
}

#define accelerationInterval 15
#define decelerationInterval 7
#define accelerating 1
#define decelerating 2

/********************************
 * reads in the potentiometer and the 2 turnout buttons of the handcontroller
 * The pot value is recalculated to a speed  setpoint
 * A seperate acceleration speed and deceleration speed is than used to accel and break
 * The speed is than send to the train being analog or digital
********************************/
void handController() {
	static uint8_t speedState = accelerating, previousSpeed;

	if( !controllerT ) {
		if( speedState == accelerating ) controllerT = accelerationInterval ;
		if( speedState == decelerating ) controllerT = decelerationInterval ;

		static int8_t speed = 0, speedSetPoint = 0, speedPrev = 50 ;
		uint8_t difference;

		static uint16_t previousSample;
		uint16_t sample = analogRead( throttle );

		if ( sample >= previousSample ) difference = sample - previousSample;
		if ( sample <  previousSample ) difference = previousSample - sample;

		if( difference > 10 ) { 
			previousSample = sample;

				 if( sample > 1000 ) { nextTurnout = STRAIGHT ; /*Serial.println("  up button pressed");*/ }  
			else if( sample <   10 ) { nextTurnout = CURVED   ; /*Serial.println("down button pressed");*/ } 
			else {
				nextTurnout = UNDETERMENED;
				////Serial.print( "sample speed " );  //Serial.println(sample);
				speedSetPoint = map( sample, lowerVal, upperVal, -100, 100 ); 
				speedSetPoint /= 10; // creates 10 speedsteps
				speedSetPoint *= 10;
			}
		}

		if( nextTurnout ) {		// if turnout buttons are pressed...
			
		}
		else {					// control speed
			if( speed < speedSetPoint ) speed ++;
			if( speed > speedSetPoint ) speed --;	

			if( abs(speed) > previousSpeed ) {
				speedState = accelerating;
			} else {
				speedState = decelerating;
			}
			previousSpeed = abs( speed ) ;

			if( speed != speedPrev ) {
				speedPrev = speed;

				// ANALOG MODE
				if( mode == analog ) {
					if( speed < 0 ) {	digitalWrite( directionPin1,  LOW );
										digitalWrite( directionPin2, HIGH ); }
					if( speed > 0 ) {	digitalWrite( directionPin1, HIGH );
										digitalWrite( directionPin2,  LOW ); }
					//Serial.print("PWM speed = "); //Serial.println(speed);
					regelaar.setSpeed( abs( speed ) );
				}

				// DCC MOCDE
				else {
					uint8_t DCCspeed = map( speed, -100, 100, 0, 56 ); // remap speed value for DCC signals
					
					setSpeed( selectedAddres, DCCspeed );
				}
			}
		}
	}
}
/********************************
 * measures the current going to the tracks and keeps track of time
 * if a continous overload of 50ms has occured, the trackpower will be shut off
 * after 5 seconds a new attempt will be made to apply track power
********************************/
void shortCircuit() {
	static byte msCounter = 0;

	if( ( digitalRead( trackPower ) == HIGH )  
	&&  ( digitalRead( weistraPin ) == HIGH ) ) { // only detect for short circuit when the PWM pulse = HIGH

		if(!overloadT) {
			overloadT = 2; // 2ms interval
		
			unsigned int sample = analogRead(currentSensePin);
			if(sample < MAXIMUM_CURRENT) {
				msCounter = 25; // 50ms total
				//Serial.println("DETECTING SHORT CIRCUIT!");
			}

			if( msCounter) { msCounter--; } // decrement every 2ms

			if(!msCounter) { 
				digitalWrite(trackPower, LOW); 
				//Serial.println("TRACK POWER DISABLED");
				restartT = 51; // after 5s trackpower will be attemt to reenable power
			}
		}
	}

	if( restartT == 1 ) { 
		restartT = 0;
		digitalWrite( trackPower, HIGH ); 
		//Serial.println("TRACK POWER ENABLED");
	}
}

/************************************************
 * Reads in all LDR values and filters the output
************************************************/
void readLDR() {
	static byte i = 0;
	uint16_t sample;
	uint16_t difference;

	if ( !updateT ) { 
		updateT = 10; // !every 3 ms

		i++;
		if( i == 6) i = 0;

		selectSensor(i);

		sample = analogRead( occupanceDetector );
		
		if ( sample >= previousSample[i] ) difference = sample - previousSample[i];
		if ( sample <  previousSample[i] ) difference = previousSample[i] - sample;

		previousSample[i] = sample;

		if( sensor[i].occupied == false && difference > 100 ) {

			sensor[i].occupied = true;
			sensor[i].occupanceValue = sample;
			 // //Serial.print("sensor ");//Serial.print(i); //Serial.print(" is true with value: "); //Serial.println(sample);
		}		
	}
}

#define nTrains 4
/************************************************
 * reads in the train select button and pick the correct train to controll
*************************************************/
void selectTrain() {
	uint16_t sample;
	uint8_t difference;
	uint8_t newTrain;
	static uint16_t previousSample = 0;

	if( !selectTrainT ) { selectTrainT = 25; // 250ms interval

		sample = analogRead( trainSelector );

		if ( sample >= previousSample ) difference = sample - previousSample;
		if ( sample <  previousSample ) difference = previousSample - sample;
		previousSample = sample;

		if( difference > ( 1024 / nTrains / 4 ) ) { // ADC range / nTrains steps / 2 -> mininum step needed to accept new value
			
			newTrain = map ( sample, 0 ,1023, 0, nTrains );	// we re-use "sample" to select the new train
			newTrain = constrain(newTrain , 0 , nTrains - 1);

			//Serial.print("new selected train is "); //Serial.println( newTrain );
			//Serial.print("sample is: "); //Serial.println( sample );
			//Serial.print("difference is: "); //Serial.println( difference );

			if( newTrain != analog ) {
				selectedAddres = addresses[ newTrain - 1 ];
				setSpeed( selectedAddres, 28 ) ;
			}

			else {
				bitClear(TIMSK1,OCIE1B);	// turn of ISR when analog mode is enabled
				Serial.println(" PWM MODE ENABLED ");
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
	if( digitalRead( trackPower ) ) { // if track power is not on, there is really no need to handle the track signals

		if( mode == analog ) {
			regelaar.update(); 	// analog mode
		} 
		else {
			DCCsignals( ); 		// digital mode (mode termines if this function should run)
		}
	}
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
	}
}


extern void processRoundRobinTasks(void) {
	static unsigned char taskCounter = INIT_TASK - 1;

	// HIGH PRIORITY TASKS
	trackSignals();

	switch(++taskCounter) {
	// INITIALIZE TASKS (runs once uppon booting)
		case INIT_TASK:
		initTurnouts();		// handle all servo motors and frog juicer
		regelaar.begin();	// weistra pwm control
		initDCC();			// DCC signal control
		PORTD = ( PORTD & 0b1011 ) | 0b1000; // differentiate both direction pins
		break;

	// LOW PRIORITY TASKS
		default: taskCounter = 0;
		case 0: handController();	break;
		case 1: shortCircuit();		break;
		case 2: layoutManager();	break;
		case 3: readLDR();			break;
		case 4: selectTrain();		break;
		case 5: flash13();			break;
		case 6: controlTurnouts();	break;
	}
}