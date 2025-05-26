#include <stdint.h>
#include <userlib.h>
#include <sys_user.h>

#define STDIN 0
#define STDOUT 1


void printString(char * string){
    
}



void print(char * string, int len) {
    sys_write(STDOUT, string);
}

void drawCursor() {
    
}