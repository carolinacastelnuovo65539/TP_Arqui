#include <stdint.h>
#include <videoDriver.h>
#include <lib.h>
#include <time.h>
#include <keyboard.h>
#include <syscalls.h>

extern int alt;
extern uint64_t regs[19];

extern void sound(uint64_t frecuencia);
extern void stop_sound();
extern uint8_t get_cmos(uint8_t port);

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
    *seconds = get_cmos(0x00);
    return 1;
}

static uint64_t sys_get_minutes(uint64_t * minutes){
    *minutes = get_cmos(0x02);
    return 1;
}

static uint64_t sys_get_hours(uint64_t * hours){
    *hours = get_cmos(0x04);
    return 1;
}

static uint64_t sys_get_day(uint64_t * day){ 
    *day = get_cmos(0x07);
    return 1;
}

static uint64_t sys_get_month(uint64_t * month){
    *month = get_cmos(0x08);
    return 1;
}

static uint64_t sys_get_year(uint64_t * year){
    *year = get_cmos(0x09);
    return 1;
}

static uint64_t sys_clean_buffer(){
    clean_buffer();
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

static uint64_t sys_sound(uint64_t frecuencia, uint64_t tiempo){
    if(frecuencia > 0)
        sound(frecuencia);
    wait(tiempo);
    if(frecuencia > 0)
        stop_sound();
    return 1;
}

static uint64_t sys_get_width(uint64_t * width){
    *width = vd_get_width();
    return 1;
}


static uint64_t sys_get_height(uint64_t * height){
    *height = vd_get_height();
    return 1;
}

static uint64_t sys_get_char_height(uint64_t * char_height) {
    *char_height = vd_get_char_height();
    return 1;
}

static uint64_t sys_get_char_width(uint64_t * char_width) {
    *char_width = vd_get_char_width();
    return 1;
}

static uint64_t sys_set_cursorX(uint64_t x){
    vd_set_cursorX(x);
    return 1;
}

static uint64_t sys_set_cursorY(uint64_t y){
    vd_set_cursorY(y);
    return 1;
}

static uint64_t sys_get_pressed_keys(uint8_t * arreglo) {
    uint8_t * aux = get_pressed_keys();
    for (int i=0; i<MAX_KEYS; i++) {
        arreglo[i] = aux[i];
    }
    return 1;
}

static void separate_rgb(Color * color, uint64_t * arg) {
    uint8_t r, g, b;
    r = (*arg >> 16) & 0xFF;
    g = (*arg >> 8) & 0xFF;
    b = *arg & 0xFF;
    color->red = r;
    color->green = g;
    color->blue = b;
}

uint64_t syscall_dispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax){
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
        separate_rgb(&color1, &r8);
        sys_drawRectangle(rdi, rsi, rdx, r10, color1);
        break;
    case 14:
        separate_rgb(&color1, &r10);
        return sys_drawCircle(rdi, rsi, rdx, color1);
        break;
    case 15: 
        separate_rgb(&color1, &r10);
        separate_rgb(&color2, &r8);
        return sys_write_color(rdi, (char *)rsi, rdx, color1, color2);
        break;
    case 16:
        return sys_sound(rdi, rsi);
        break;
    case 17:
        return sys_get_day((uint64_t*)rdi);
        break;
    case 18:
        return sys_get_month((uint64_t*)rdi);
        break;
    case 19:
        return sys_get_year((uint64_t*)rdi);
        break;
    case 20:
        return sys_get_height((uint64_t*)rdi);
        break;
    case 21:
        return sys_get_width((uint64_t*)rdi);
        break;
    case 22:
        return sys_set_cursorX(rdi);
        break;
    case 23:
        return sys_set_cursorY(rdi);
        break;
    case 24:
        return sys_get_char_width((uint64_t*)rdi);
        break;
    case 25:
        return sys_get_char_height((uint64_t*)rdi);
        break;
    case 26:
        return sys_get_pressed_keys((uint8_t*)rdi);
        break;
    case 27:
        return sys_clean_buffer();
        break;
    }
    return -1;
}
