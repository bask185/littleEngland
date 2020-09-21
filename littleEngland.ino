#include "src/basics/timers.h"
#include "src/basics/io.h"
#include "roundRobinTasks.h"

void setup() {
	initTimers();
	initIO();
	Serial.begin(115200);
}


void loop() {
	processRoundRobinTasks();
}

