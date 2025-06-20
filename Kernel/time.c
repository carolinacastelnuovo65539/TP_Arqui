#include <time.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void wait(int delta){
	int actualTicks = ticks;
	while(ticks - actualTicks < delta){
		_hlt();
	}
}
