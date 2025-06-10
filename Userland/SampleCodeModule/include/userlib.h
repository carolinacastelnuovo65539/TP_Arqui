#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>

typedef struct color{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}Color;


/*--------------AVAILABLE COLORS--------------*/
static const Color BLACK ={0,0,0};
static const Color WHITE = {0xFF,0xFF,0xFF};
static const Color RED = {0xFF, 0,0};
static const Color GREEN = {0,0xFF,0};
static const Color BLUE = {0,0,0xFF};
static const Color ORANGE = {0xFF,0xB3,0x66};
static const Color YELLOW = {0xFF,0xF1,0x76};
static const Color BRIGHT_YELLOW ={0xFF,0xAA,0x00};
static const Color LIGHT_GREEN = {0x9A,0xFF,0x9A};
static const Color DARK_GREEN = {0,0x9C,0x02};
static const Color LIGHT_BLUE={0x64,0x95,0xED};
static const Color DARK_BLUE={0,0,0xD2};
static const Color VIOLET = {0x8A,0x2B,0xE2};
static const Color DARK_PINK={0xDA,0,0xEC};
static const Color PINK={0xFF,0,0xFF};
static const Color WINE={0xB4,0,0x57};
static const Color LIGHT_RED={0xCD,0x5C,0x5C};
static const Color TEAL = {0x00,0x80,0x80};

int strcmp(char * str1, char * str2);

int strlen(char * str);

void print(char * c, int len);

void printColor(char * string, int len, Color fuente, Color fondo);

char getChar();

void putChar(char c);
 
void drawCursor();

void printRegisters();

uint64_t get_hours();

uint64_t get_minutes();

uint64_t get_seconds();

void clear();

void getTime();

void getDate();

void printBase(uint64_t value, uint32_t base);

void printDec(uint64_t value);

void printBase(uint64_t value, uint32_t base);

void reduce();

void increase();

void drawRectangle(int x, int y, int width, int height, Color color);

void drawCircle(int centerX, int centerY, int radius, Color color);

void zero();

void opcode(void);

void beep(uint64_t frecuencia, uint64_t tiempo);

void get_cmos(uint64_t (*fun1)(void), uint64_t (*fun2)(void), uint64_t (*fun3)(void), char separate);

char to_lower(char c);

void printColorCentered(char *msg, Color fg, Color bg, uint64_t char_width, uint64_t char_height, uint8_t centered);

uint64_t set_height();

uint64_t set_width();

void set_cursorX(uint64_t value);

void set_cursorY(uint64_t value);

uint64_t get_char_width();

uint64_t get_char_height();

void sleep(uint64_t time);

void get_pressed_keys(uint8_t * pressed_keys);

void clean_buff();

#endif