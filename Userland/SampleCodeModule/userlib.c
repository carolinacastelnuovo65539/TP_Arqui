#include <stdint.h>
#include <userlib.h>
#include <sys_user.h>
#include <exceptions.h>

#define STDIN 0
#define STDOUT 1
#define DIF_UTC 3
#define DIF_LOWER 'a'-'A'
#define MAX_KEYS 128
#define TRUE 1
#define FALSE !TRUE


static char buffer[64] = {'0'};

static char *nombreRegistros[19] = {"RAX","RBX","RCX","RDX","RSI","RDI","RBP","R8","R9","R10","R11","R12","R13","R14","R15","RIP","CS","RFLAGS", "RSP"};

static void to_hex(uint64_t n, char buf[16]);
static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);



uint64_t height, width;

char getChar(){
    char c;
    sys_read(0, &c);
    return c;
}

void putChar(char c){
    char buff[2];
    buff[0] = c;
    buff[1] = '\0';
    print(buff);
}

void print(char * string) {
    sys_write(STDOUT, string);
}


void printColorCentered(char *msg, Color fg, Color bg, uint64_t char_width, uint64_t char_height, uint8_t centered) {
    int screen_width = set_width(); 
    int screen_height = set_height();

    if (centered) {
        height=(screen_height)/2;
    }

    int len = strlen(msg);
    int spaces = (screen_width - char_width*len) / 2;
    
    if (spaces < 0) spaces = 0;

    set_cursorX(spaces); // posición horizontal
    set_cursorY(height); // posicion vertical
    printColor(msg, fg, bg);
    height += char_height; // Desplazamiento vertical
}


void printColor(char * string, Color fuente, Color fondo) {
    sys_write_color(STDOUT, string, fuente, fondo);
}


void drawRectangle(int x, int y, int width, int height, Color color) {
    sys_drawRectangle(x, y, width, height, color);
}

void drawCircle(int centerX, int centerY, int radius, Color color) {
    sys_drawCircle(centerX, centerY, radius, color);
}

void beep(uint64_t frecuencia, uint64_t tiempo){
    sys_sound(frecuencia, tiempo);
}

void sleep(uint64_t time){
    sys_wait(time);
}

//Devuelve 0 si son iguales, distinto de cero sino 
int strcmp(char * str1, char * str2){
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0'){
        if(str1[i] != str2[i]){
            return str1[i] - str2[i];
        }
        i++;
    }
    return str1[i] - str2[i];
}

int strlen(char * str){
    int i = 0;
    while(str[i] != '\0'){
        i++;
    }
    return i;
}


char to_lower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + DIF_LOWER;  // Diferencia entre 'A' y 'a' en ASCII
    }
    return c;
}

static void to_hex(uint64_t n, char buf[16]){
	for (int i=15; i>=0; i--) {
		int num2 = n % 16;
		buf[i] = (num2 < 10 ? '0' : ('A' - 10)) + num2;
		n /= 16;
    }
}

void printRegisters(){
    putChar('\n');
    char buff[19] = {'0'};
	buff[0] = '0';
	buff[1] = 'x';
	buff[18] = '\0';
    uint64_t regs[19];
    uint64_t flag = 0;
    sys_registersInfo(regs, &flag);
    if(flag == 1){
        print("Registers:\n");
        for(int i = 0; i < 19; i++){
            print(nombreRegistros[i]);
            print(": ");
            to_hex(regs[i], buff + 2);
            print(buff);
			if (i % 4 == 3)
				putChar('\n');
			else
				print("   ");
        }
    }else{
        print("ERROR: press the ALT key first");
    }
}

uint64_t get_hours(){
    uint64_t hour;
    sys_get_hours(&hour);
    if (hour >= DIF_UTC) {
        return hour - DIF_UTC;
    } else {
        return hour + (24 - DIF_UTC);
    }
    // return hour - DIF_UTC; // Ajuste para la zona horaria UTC-3
}

uint64_t get_minutes(){
    uint64_t minutes;
    sys_get_minutes(&minutes);
    return minutes;
}

uint64_t get_seconds(){
    uint64_t seconds;
    sys_get_seconds(&seconds);
    return seconds;
}

uint64_t get_day(){
    uint64_t day;
    sys_get_day(&day);
    return day;
}

uint64_t get_month(){
    uint64_t month;
    sys_get_month(&month);
    return month;
}

uint64_t get_year(){
    uint64_t year;
    sys_get_year(&year);
    return year+2000;
}

uint64_t set_height(){
    sys_get_height(&height);
    return height;
}

uint64_t set_width(){
    sys_get_width(&width);
    return width;
}

uint64_t get_char_width() {
    uint64_t char_width;
    sys_get_char_width(&char_width);
    return char_width;
}

uint64_t get_char_height() {
    uint64_t char_height;
    sys_get_char_height(&char_height);
    return char_height;
}

void scanf(char *buffer, int max_length) {
    int i = 0;
    char c;

    while (i < max_length - 1) {
        c = getChar(); 
        if (c == '\n' || c == '\r') { 
            break;
        }
        buffer[i++] = c;
    }

    buffer[i] = '\0'; 
}

void reduce(){
    sys_reduce();
}

void clear(){
    sys_clear();
}

void increase(){
    sys_increase();
}

void zero(){
    exc_zero();
}

void opcode(void) { 
    exc_opcode();
}

void getTime(){
    get_cmos(get_hours, get_minutes, get_seconds, ':');
}


void getDate() {
    get_cmos(get_month, get_day, get_year, '/');
}

void get_cmos(uint64_t (*fun1)(void), uint64_t (*fun2)(void), uint64_t (*fun3)(void), char separate) {
    int first, second, third;
    first = fun1();
    second = fun2();
    third = fun3();

    printDec(first);
    putChar(separate);
    printDec(second);
    putChar(separate);
    printDec(third);
}

void printDec(uint64_t value)
{
    // Si el número es menor que 10, agrega un 0 adelante
    if (value >= 0 && value < 10) {
        char paddedBuffer[3];
        paddedBuffer[0] = '0';
        paddedBuffer[1] = value + '0';
        paddedBuffer[2] = '\0';
        print(paddedBuffer);
    } else {
        printBase(value, 10);
    }
}

void printBase(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	print(buffer);
}

void set_cursorX(uint64_t value) {
    sys_set_cursorX(value);
}

void set_cursorY(uint64_t value) {
    sys_set_cursorY(value);
}

void get_pressed_keys(uint8_t * pressed_keys) {
    sys_get_pressed_keys(pressed_keys);
}

void clean_buff(){
    sys_clean_buffer();
}

static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
	return digits;
}