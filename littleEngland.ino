#include "src/basics/timers.h"
#include "src/basics/io.h"
#include "roundRobinTasks.h"

void setup() {
	cli();
	initTimers();
	sei();
	
	Serial.begin(115200);
	initIO();
	Serial.println("hi bas");


}


void loop() {
	processRoundRobinTasks();
}

