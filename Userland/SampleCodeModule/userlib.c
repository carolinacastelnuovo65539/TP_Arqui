#include <stdint.h>
#include <userlib.h>
#include <sys_user.h>

#define STDIN 0
#define STDOUT 1


void printString(char * string){
    
}

char getChar(){
    char c;
    sys_read(0, &c);
    return c;
}

void putChar(char c){
    char * buff = &c;
    print(buff, 1);
}

void print(char * string, int len) {
    sys_write(STDOUT, string, len);
}

void drawCursor() {
    
}