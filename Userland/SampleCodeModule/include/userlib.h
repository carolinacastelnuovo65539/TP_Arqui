#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>

void print(char * c, int len);

char getChar();

void putChar(char c);
 
void drawCursor();

void printString(char * string);

void printRegisters();

uint8_t getHours();

uint8_t getMinutes();

uint8_t getSeconds();

void getTime();

void printBase(uint64_t value, uint32_t base);

void printDec(uint64_t value);

void printBase(uint64_t value, uint32_t base);


#endif