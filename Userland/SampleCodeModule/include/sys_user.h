#include <stdint.h>
#include "userlib.h"

extern uint64_t sys_read(uint64_t fd, char * buffer);

extern uint64_t sys_write(uint64_t fd, char * buffer, int len);

extern uint64_t sys_write_color(uint64_t fd, char * buffer, int len, Color fuente, Color fondo);

extern uint64_t sys_wait(uint64_t time);

extern uint64_t sys_get_seconds();

extern uint64_t sys_get_minutes();

extern uint64_t sys_get_hours();

extern uint64_t sys_reduce();

extern uint64_t sys_clear();

extern uint64_t sys_increase();

// extern uint64_t sys_cursor();

extern uint64_t sys_registersInfo(uint64_t regs[19], uint64_t * flag);

extern uint64_t sys_drawRectangle(int x, int y, int width, int height, Color color);

extern uint64_t sys_drawCircle(int centerX, int centerY, int radius, Color color);

