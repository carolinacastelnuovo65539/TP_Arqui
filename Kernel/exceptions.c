#include <videoDriver.h>

#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6

int flag_zero = 1; // Flag to indicate if a zero division exception has occurred



extern uint64_t exc_regs[19];
static char * nombreRegistros[19] = {"RAX","RBX","RCX","RDX","RSI","RDI","RBP","R8","R9","R10","R11","R12","R13","R14","R15","RIP","CS","RFLAGS", "RSP"};



static void pasoHexa(uint64_t num1, char buffer[16]){
	int i = 15;
	do
	{
		int num2 = num1 % 16;
		buffer[i] = (num2 < 10 ? '0' : ('A' - 10)) + num2;
		num1 /= 16;
	} while (i-- != 0);
	buffer[15]='\0';
}


void print_regs(){
	char buffer[16];
	for(int i = 0; i < 19; i++){
		vd_prints(nombreRegistros[i], 100, WHITE, BLACK); //este 5 hay que sacarlo
		vd_prints(": ", 2, WHITE, BLACK);
		pasoHexa(exc_regs[i], buffer);
		vd_prints(buffer, 16, WHITE, BLACK);
		vd_print('\n', WHITE, BLACK);
	}
}

static void zero_division() {
	vd_prints("\nZERO DIVISION EXCEPTION\n", 25, WHITE, BLACK);
	print_regs();
	vd_print('\n', WHITE, BLACK);	
}

static void opcode_exception() {
	vd_prints("\nINVALID OPCODE EXCEPTION\n", 28, WHITE, BLACK);
	print_regs();
	vd_print('\n', WHITE, BLACK);	
	// while(1){
	// 	_hlt();
	// }
}

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID && flag_zero)
		{flag_zero = 0; // Reset the zero flag
		zero_division();}
	else if(exception == OPCODE_EXCEPTION_ID)
		opcode_exception();
}