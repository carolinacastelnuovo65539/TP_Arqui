#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <stdint.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}Color;

extern Color BLACK;
extern Color WHITE;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

static void drawChar(char c, Color fuente, Color fondo);

static void scroll();

void setPixel(uint16_t x, uint16_t y, Color color);

static uint32_t* getPixel(uint16_t x, uint16_t y);

#endif