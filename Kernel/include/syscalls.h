#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>
#include "videoDriver.h"
#include "time.h"

static uint64_t sys_read(uint64_t fd, char* buffer);

static uint64_t sys_write(uint64_t fd, char buffer);


#endif