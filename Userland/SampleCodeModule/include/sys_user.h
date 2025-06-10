#include <stdint.h>
#include "userlib.h"

extern void sys_read(uint64_t fd, char * buffer);

extern void sys_write(uint64_t fd, char * buffer);

extern void sys_write_color(uint64_t fd, char * buffer, Color fuente, Color fondo);

extern void sys_wait(uint64_t time);

extern void sys_get_seconds(uint64_t * seconds);

extern void sys_get_minutes(uint64_t * minutes);

extern void sys_get_hours(uint64_t * hours);

extern void sys_get_day(uint64_t * seconds);

extern void sys_get_month(uint64_t * minutes);

extern void sys_get_year(uint64_t * hours);

extern void sys_reduce();

extern void sys_clear();

extern void sys_increase();

extern void sys_registersInfo(uint64_t regs[19], uint64_t * flag);

extern void sys_drawRectangle(int x, int y, int width, int height, Color color);

extern void sys_drawCircle(int centerX, int centerY, int radius, Color color);

extern void sys_sound(uint64_t frecuencia, uint64_t tiempo);

extern void sys_get_height(uint64_t * height);

extern void sys_get_width(uint64_t * width);

extern void sys_set_cursorX(uint64_t value);

extern void sys_set_cursorY(uint64_t value);

extern void sys_get_char_width(uint64_t * charWidth);

extern void sys_get_char_height(uint64_t * charHeight);

extern void sys_get_pressed_keys(uint8_t * key);

extern void sys_clean_buffer();

