#include <stdint.h>
#include "userlib.h"

extern uint64_t sys_read(uint64_t fd, char * buffer);

extern uint64_t sys_write(uint64_t fd, char * buffer, int len);

extern uint64_t sys_write_color(uint64_t fd, char * buffer, int len, Color fuente, Color fondo);

extern uint64_t sys_wait(uint64_t time);

extern uint64_t sys_get_seconds(uint64_t * seconds);

extern uint64_t sys_get_minutes(uint64_t * minutes);

extern uint64_t sys_get_hours(uint64_t * hours);

extern uint64_t sys_get_day(uint64_t * seconds);

extern uint64_t sys_get_month(uint64_t * minutes);

extern uint64_t sys_get_year(uint64_t * hours);

extern uint64_t sys_reduce();

extern uint64_t sys_clear();

extern uint64_t sys_increase();

// extern uint64_t sys_cursor();

extern uint64_t sys_registersInfo(uint64_t regs[19], uint64_t * flag);

extern uint64_t sys_drawRectangle(int x, int y, int width, int height, Color color);

extern uint64_t sys_drawCircle(int centerX, int centerY, int radius, Color color);

extern uint64_t sys_sound(uint64_t frecuencia, uint64_t tiempo);

extern uint64_t sys_get_height();

extern uint64_t sys_get_width();

extern uint64_t sys_set_cursorX(uint64_t value);

extern uint64_t sys_set_cursorY(uint64_t value);

extern uint64_t sys_get_char_width(uint64_t * charWidth);

extern uint64_t sys_get_char_height(uint64_t * charHeight);

