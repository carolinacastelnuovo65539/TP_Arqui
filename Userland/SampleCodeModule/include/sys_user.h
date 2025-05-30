#include <stdint.h>
#include "userlib.h"

extern uint64_t sys_read(uint64_t fd, char * buffer);

extern uint64_t sys_write(uint64_t fd, char * buffer, int len);

// extern void sys_clear();

extern uint64_t wait(uint64_t time);

extern uint8_t sys_getSeconds();

extern uint8_t sys_getMinutes();

extern uint8_t sys_getHours();

// extern uint64_t sys_cursor();

extern uint64_t sys_registersInfo(uint64_t regs[19], uint64_t * flag);


