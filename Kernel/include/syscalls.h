#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include "videoDriver.h"
#include "time.h"


static uint64_t sys_read(uint64_t fd, char * buffer);

static uint64_t sys_write(uint64_t fd, char * buffer, int len);

// static void sys_clear();

// static uint64_t wait(uint64_t time);

// static uint8_t sys_getSeconds();

// static uint8_t sys_getMinutes();

// static uint8_t sys_getHours();

// static uint64_t sys_cursor();

uint64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax);



#endif