#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <keyboard.h>

#include <stdint.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}Color;

extern Color BLACK;
extern Color WHITE;

void prints(const char * str, Color fuente, Color fondo);
void print(const char buffer, Color fuente, Color fondo);
void newLine();
void backspace();
void clear();
void putCursor();
void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void driverRead(char *buff);
void setPixel(uint16_t x, uint16_t y, Color color);


#endif