#include "src/basics/timers.h"
#include "src/basics/io.h"
#include "roundRobinTasks.h"

void setup() {
	cli();
	initTimers();
	sei();
	initIO();
	Serial.begin(115200);
	Serial.println("hi bas");
}


void loop() {
	processRoundRobinTasks();
}

