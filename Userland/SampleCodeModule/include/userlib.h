#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>

void print(char * c, int len);

char getChar();

void putChar(char c);
 
void drawCursor();

void printString(char * string);

void printRegisters();

uint64_t getHours();

uint64_t getMinutes();

uint64_t getSeconds();

void getTime();

void printBase(uint64_t value, uint32_t base);



#endif