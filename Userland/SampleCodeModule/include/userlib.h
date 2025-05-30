#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>


typedef struct color{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}Color;


/*--------------COLORES DISPONIBLES--------------*/
static const Color BLACK ={0,0,0};
static const Color WHITE = {0xFF,0xFF,0xFF};
static const Color RED = {0xFF, 0,0};
static const Color GREEN = {0,0xFF,0};
static const Color BLUE = {0,0,0xFF};
static const Color ORANGE = {0xFF,0x80,0};
static const Color YELLOW = {0xFF,0xE8,0};
static const Color LIGHT_GREEN = {0,0xE0,0x03};
static const Color DARK_GREEN = {0,0x9C,0x02};
static const Color LIGHT_BLUE={0,0xC1,0xFF};
static const Color DARK_BLUE={0,0,0xD2};
static const Color VIOLET = {0x93,0,0xFF};
static const Color DARK_PINK={0xDA,0,0xEC};
static const Color PINK={0xFF,0,0xFF};
static const Color WINE={0xB4,0,0x57};
// static const Color WHITE = {255, 255, 255};
// static const Color RED = {0, 0, 255};
// static const Color LIGHT_BLUE = {173, 216, 230};
// static const Color BLUE = {255, 0, 0};
// static const Color ORANGE = {16, 160, 255};
// static const Color YELLOW = {30, 224, 255};
// static const Color PURPLE = {255, 32, 160};
// static const Color PINK = {100, 0, 244};
// static const Color GREEN = {0, 255, 0};
// static const Color LIGHT_RED = {0, 255, 255};
// static const Color LIGHT_PURPLE = {255, 0, 255};
// static const Color LIGHT_ORANGE = {0, 160, 255};
// static const Color LIGHT_YELLOW = {0, 224, 255};
// static const Color LIGHT_PINK = {0, 100, 244};
// static const Color LIGHT_GREEN = {0, 255, 0};

int strcmp(char * str1, char * str2);

int strlen(char * str);

void print(char * c, int len);

void printColor(char * string, int len, Color fuente, Color fondo);

char getChar();

void putChar(char c);
 
void drawCursor();

void printString(char * string);

void printRegisters();

uint8_t get_hours();

uint8_t get_minutes();

uint8_t get_seconds();

void clear();

void getTime();

void printBase(uint64_t value, uint32_t base);

void printDec(uint64_t value);

void printBase(uint64_t value, uint32_t base);

void reduce();

void increase();

void drawRectangle(int x, int y, int width, int height, Color color);

void drawCircle(int centerX, int centerY, int radius, Color color);

#endif