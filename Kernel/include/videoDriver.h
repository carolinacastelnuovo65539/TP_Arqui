#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <keyboard.h>

#include <stdint.h>

typedef struct{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

extern Color BLACK;
extern Color WHITE;

void vd_prints(const char * str, Color fuente, Color fondo);

void vd_print(const char buffer, Color fuente, Color fondo);

void vd_read(char * buff);

void newLine();

void backspace();

void clear();

void putCursor();

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);

void setPixel(uint16_t x, uint16_t y, Color color);

void vd_reduce();

void vd_increase();

void vd_drawRectangle(int x, int y, int width, int height, Color color);

void vd_drawCircle(int centerX, int centerY, int radius, Color color);

uint64_t vd_get_height();

uint64_t vd_get_width();

void vd_set_cursorX(uint64_t x);

void vd_set_cursorY(uint64_t y);

uint64_t vd_get_char_height();
uint64_t vd_get_char_width();



#endif