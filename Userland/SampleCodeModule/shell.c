#include <shell.h>
#include <userlib.h>

void start(){
    char c;
	
	while(1){
		c = getChar();
		putChar(c);
	}

    getTime();
}