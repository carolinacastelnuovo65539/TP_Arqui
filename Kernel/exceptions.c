#include <videoDriver.h>

#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6


extern uint64_t exc_regs[19];
static char * nombreRegistros[19] = {"RAX","RBX","RCX","RDX","RSI","RDI","RBP","R8","R9","R10","R11","R12","R13","R14","R15","RIP","CS","RFLAGS", "RSP"};



static void pasoHexa(uint64_t num1, char buffer[18]){
	buffer[0] = '0';
	buffer[1] = 'x';
	for (int i = 17; i >= 2; i--) {
		int num2 = num1 % 16;
		buffer[i] = (num2 < 10) ? ('0' + num2) : ('A' + num2 - 10);
		num1 /= 16;
	}
	buffer[18] = '\0';
}


void print_regs(){
	char buffer[19];
	for(int i = 0; i < 19; i++){
		vd_prints(nombreRegistros[i], WHITE, BLACK);
		vd_prints(": ", WHITE, BLACK);
		pasoHexa(exc_regs[i], buffer);
		vd_prints(buffer, WHITE, BLACK);
		vd_print('\n', WHITE, BLACK);
	}
}

static void zero_division() {
	vd_prints("\nERROR: ZERO DIVISION EXCEPTION\n", WHITE, BLACK);
	print_regs();
}

static void opcode_exception() {
	vd_prints("\nERROR: INVALID OPCODE EXCEPTION\n", WHITE, BLACK);
	print_regs();	
}

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID ){
		zero_division();}
	else if(exception == OPCODE_EXCEPTION_ID)
		opcode_exception();
}