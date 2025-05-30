#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include "videoDriver.h"
#include "time.h"


static uint64_t sys_read(uint64_t fd, char * buffer);

static uint64_t sys_write(uint64_t fd, char * buffer, int len);

static uint64_t sys_write_color(uint64_t fd, char * buffer, int len, Color fuente, Color fondo);

static void sys_clear();

static uint64_t sys_wait(uint64_t time);

static uint8_t sys_get_seconds();

static uint8_t sys_get_minutes();

static uint8_t sys_get_hours();

static uint64_t sys_cursor();

static uint64_t sys_reduce();

static uint64_t sys_increase();

uint64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax);

static uint64_t sys_drawRectangle(int x, int y, int width, int height, Color color);

static uint64_t sys_drawCircle(int centerX, int centerY, int radius, Color color);

#endif