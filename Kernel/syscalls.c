#include <stdint.h>
#include <videoDriver.h>
#include <lib.h>
#include <time.h>
#include <keyboard.h>

extern int alt;
extern uint64_t regs[19];

static uint64_t sys_read(uint64_t fd, char * buffer){
    if (fd != 0){
        return -1;
    }
    vd_read(buffer);
    return 0;
}

static uint64_t sys_write(uint64_t fd, char * buffer, int len){
    if (fd != 1) {
        return -1;
    }
    vd_prints(buffer, len, WHITE, BLACK);
    return 1;
}

static void sys_clear(){
    clear();
    return;
}

static uint64_t sys_wait(uint64_t time){
    wait(time);
    return 1;
}

static uint64_t sys_getSeconds(uint64_t * seconds){
    return getSeconds();
}

static uint64_t sys_getMinutes(uint64_t * minutes){
    return getMinutes();
}

static uint64_t sys_getHours(uint64_t * hours){
    return getMinutes();
}

static uint64_t sys_cursor(){
    putCursor();
    return 1;
}

static uint64_t sys_registersInfo(uint64_t registros[19], uint64_t * flag){
    *flag = getAltFlag();
    if(*flag){
        for(int i = 0; i < 19; i++){
            registros[i] = regs[i];
        }
    }
    return 1;
}

uint64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax){
    switch (rax)
    {
    case 0:  //read
        return sys_read(rdi, (char *)rsi);
        break;
    case 1: //write
        return sys_write(rdi, (char *)rsi, rdx);
        break;
    case 2: //clear
        sys_clear();
        break;
    case 3: //seconds
        sys_getSeconds((uint64_t *) rdi);
        break;
    case 4: //minutes
        sys_getMinutes((uint64_t *) rdi);
        break;
    case 5: //hours
        sys_getHours((uint64_t *) rdi);
        break;
    case 6: //cursor
        sys_cursor();
        break;
    case 7:
        sys_wait(rdi);
        break;
    case 10: //registers
        sys_registersInfo((uint64_t *) rdi, (uint64_t *) rsi);
        break;
    return 1;
    }
}