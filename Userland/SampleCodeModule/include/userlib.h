#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>

void print(char * c, int len);

char getChar();

void putChar(char c);
 
void drawCursor();

void printString(char * string);

#endif