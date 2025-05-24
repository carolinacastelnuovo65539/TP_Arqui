#include <stdint.h>
#include <videoDriver.h>
#include <lib.h>
#include <time.h>
#include <keyboard.h>

static uint64_t sys_read(uint64_t fd, char* buffer){
    if (fd != 0){
        return -1;
    }
    driverRead(buffer);
    return 0;
}

static uint64_t sys_write(uint64_t fd, char buffer){
    if (fd != 1) {
        return -1;
    }
    drawChar(buffer, WHITE, BLACK);
    return 1;
}

uint64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax){
    switch (rax)
    {
    case 0:
        return sys_read(rdi, (char *)rsi);
        break;
    
    case 1:
        return sys_write(rdi, (char)rsi);
        break;
    }

}