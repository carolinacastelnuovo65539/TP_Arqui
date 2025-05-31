#include <stdint.h>
#include <videoDriver.h>
#include <lib.h>
#include <time.h>
#include <keyboard.h>
#include <syscalls.h>




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

static uint64_t sys_get_seconds(uint64_t * seconds){ 
    *seconds = get_seconds();
    return 1;
}

static uint64_t sys_get_minutes(uint64_t * minutes){
    *minutes = get_minutes();
    return 1;
}

static uint64_t sys_get_hours(uint64_t * hours){
    *hours = get_hours();
    return 1;
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

static uint64_t sys_reduce(){
    vd_reduce();
    return 1;
}

static uint64_t sys_increase(){
    vd_increase();
    return 1;
}

static uint64_t sys_drawRectangle(int x, int y, int width, int height, Color color){
    vd_drawRectangle(x, y, width, height, color);
    return 1;
}

static uint64_t sys_drawCircle(int centerX, int centerY, int radius, Color color){
    vd_drawCircle(centerX, centerY, radius, color);
    return 1;
}

static uint64_t sys_write_color(uint64_t fd, char * buffer, int len, Color fuente, Color fondo){
    if (fd != 1) {
        return -1;
    }
    vd_prints(buffer, len, fuente, fondo);
    return 1;
}

uint64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax){
    uint8_t r, g, b;
    Color color1, color2;
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
        return sys_get_seconds((uint64_t *)rdi);
    case 4: //minutes
        return sys_get_minutes((uint64_t *)rdi);
    case 5: //hours
        return sys_get_hours((uint64_t *)rdi);
    case 6: //cursor
        sys_cursor();
        break;
    case 7:
        sys_wait(rdi);
        break;
    case 10: //registers
        sys_registersInfo((uint64_t *) rdi, (uint64_t *) rsi);
        break;
    case 11: //reduce size
        sys_reduce();
        break;
    case 12: //increase size
        sys_increase();
        break;
    case 13: //draw rectangle
        r = (r8 >> 16) & 0xFF;
        g = (r8 >> 8) & 0xFF;
        b = r8 & 0xFF;
        color1.red = r;
        color1.green = g;
        color1.blue = b;
        sys_drawRectangle(rdi, rsi, rdx, r10, color1);
        break;
    case 14:
        r = (r8 >> 16) & 0xFF;
        g = (r8 >> 8) & 0xFF;
        b = r8 & 0xFF;
        color1.red = r;
        color1.green = g;
        color1.blue = b;
        return sys_drawCircle(rdi, rsi, rdx, color1);
        break;
    case 15: 
        r = (r10 >> 16) & 0xFF;
        g = (r10 >> 8) & 0xFF;
        b = r10 & 0xFF;
        color1.red = r;
        color1.green = g;
        color1.blue = b; 

        r = (r8 >> 16) & 0xFF;
        g = (r8 >> 8) & 0xFF;
        b = r8 & 0xFF;
        color2.red = r;
        color2.green = g;
        color2.blue = b;
        return sys_write_color(rdi, (char *)rsi, rdx, color1, color2);
        break;
    return 1;
    }
}
