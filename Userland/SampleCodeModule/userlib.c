#include <stdint.h>
#include <userlib.h>
#include <sys_user.h>

#define STDIN 0
#define STDOUT 1
#define DIF_UTC 3

static char buffer[64] = {'0'};

static char *nombreRegistros[19] = {"RAX","RBX","RCX","RDX","RSI","RDI","RBP","R8","R9","R10","R11","R12","R13","R14","R15","RIP","CS","RFLAGS", "RSP"};

static void toHex(uint64_t n, char buf[16]);
static uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);


char getChar(){
    char c;
    sys_read(0, &c);
    return c;
}

void putChar(char c){
    char * buff = &c;
    print(buff, 1);
}

void print(char * string, int len) { //preguntar si es más lógico que no se le pase la longitud
    sys_write(STDOUT, string, len);
}


void printColor(char * string, int len, Color fuente, Color fondo) {
    sys_write_color(STDOUT, string, len, fuente, fondo);
}



void drawRectangle(int x, int y, int width, int height, Color color) {
    sys_drawRectangle(x, y, width, height, color);
}

void drawCircle(int centerX, int centerY, int radius, Color color) {
    sys_drawCircle(centerX, centerY, radius, color);
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

void drawCursor() {
    
}


static void toHex(uint64_t n, char buf[16]){
	int i = 15;
	do
	{
		int num2 = n % 16;
		buf[i] = (num2 < 10 ? '0' : ('A' - 10)) + num2;
		n /= 16;
	} while (i-- != 0);
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
        for(int i = 0; i < 19; i++){
            print(nombreRegistros[i], 5); //este 5 hay que sacarlo
            print(": ", 2);
            toHex(regs[i], buff + 2);
            print(buff, 18);
			if (i % 4 == 3)
				putChar('\n');
			else
				print("   ", 3);
        }
    }else{
        print("ERROR: presionar primero la tecla ALT", 46);
    }
}

uint8_t get_hours(){
    return sys_get_hours() - DIF_UTC;
}

uint8_t get_minutes(){
    return sys_get_minutes();
}

uint8_t get_seconds(){
    return sys_get_seconds();
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

// void printInt(int num) {
//     char buffer[12]; // Suficiente para almacenar int de 32 bits con signo
//     int i = 0;
//     if (num == 0) {
//         putChar('0');
//         return;
//     }
//     if (num < 0) {   // Maneja el signo
//         putChar('-');
//         num = -num;
//     }
//     while (num > 0) { // Convierte el número a caracteres
//         buffer[i++] = (num % 10) + '0';
//         num /= 10;
//     }
//     // Imprime el número en el orden correcto
//     for (int j = i - 1; j >= 0; j--) {
//         putChar(buffer[j]);
//     }
// }

void getTime(){
	uint8_t hours, minutes, seconds;

    hours = get_hours();
    minutes = get_minutes();
    seconds = get_seconds();

    printDec(hours);
    putChar(':');
    printDec(minutes);
    putChar(':');
    printDec(seconds);
}


void printDec(uint64_t value)
{
	printBase(value, 10);
}

void printBase(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	print(buffer, 20);
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