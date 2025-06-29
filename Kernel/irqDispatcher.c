#include <time.h>
#include <keyboard.h>
#include <stdint.h>

static void int_20();
static void int_21();

extern void get_key();

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21(){
	keyboard_handler();
}

