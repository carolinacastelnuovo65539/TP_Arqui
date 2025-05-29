#include <stdint.h>
#include <userlib.h>
#include <sys_user.h>

#define STDIN 0
#define STDOUT 1
#define DIF_UTC 3

static char *nombreRegistros[19] = {"RAX","RBX","RCX","RDX","RSI","RDI","RBP","R8","R9","R10","R11","R12","R13","R14","R15","RIP","CS","RFLAGS", "RSP"};

static void toHex(uint64_t n, char buf[16]);

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

void drawCursor() {
    
}


static void toHex(uint64_t n, char buf[16]){
        for(int i = 0; i < 16; i++){
            int digit = n % 16;
            buf[i] = (digit < 10 ? '0' : ('A' - 10)) + digit;
            n /= 16;
        }
}

void printRegisters(){
    putChar('\n');
    char buff[19];
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

uint64_t getHours(){
    uint64_t hours;
    sys_getHours(&hours);
    return hours-DIF_UTC;
}

uint64_t getMinutes(){
    uint64_t minutes;
    sys_getMinutes(&minutes);
    return minutes;
}

uint64_t getSeconds(){
    uint64_t seconds;
    sys_getMinutes(&seconds);
    return seconds;
}

static uint8_t bcd_to_bin(uint8_t value) {
    return (value & 0x0F) + ((value >> 4) * 10);
}

// static void va_printf(const char* fmt, va_list args){
//     char buffer[MAX_CHARS] = {0};
//     const char * aux = fmt;           //puntero
//     while(*aux){
//         if(*aux == '%'){
//             aux++;
//             int dx = strtoi(aux, &aux);   //si es numero lo devuelve en formato decimal sino develve 0
//             int len;

//             switch (*aux)
//             {
//             case 'c':       //es un char
//                 putchar(va_arg(args, int));
//                 break;
            
//             case 'd':    //es un entero
//                 len = itoa(va_arg(args, int), buffer, 10);     //esta en base 10
//                 printchars('0', dx-len);
//                 puts(buffer);
//                 break;
            
//             case 'x':       //hexadcimal
//                 len = itoa(va_arg(args, int), buffer, 16);     //esta en base 16
//                 printchars('0', dx-len);
//                 puts(buffer);
//                 break;

//             case 's':       //es un string
//                 printchars(' ', dx);
//                 puts(va_arg(args, char *));
//                 break;
//             }
//         }else {
//             putchar(*aux);
//         }
//         aux++;
//     }
// }


void printInt(int num) {
    char buffer[12]; // Suficiente para almacenar int de 32 bits con signo
    int i = 0;
    if (num == 0) {
        putChar('0');
        return;
    }
    if (num < 0) {   // Maneja el signo
        putChar('-');
        num = -num;
    }
    while (num > 0) { // Convierte el número a caracteres
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    // Imprime el número en el orden correcto
    for (int j = i - 1; j >= 0; j--) {
        putChar(buffer[j]);
    }
}

void getTime(){
	uint8_t hours, minutes, seconds;

    hours = getHours();
    minutes = getMinutes();
    seconds = getSeconds();

    printInt(hours);
    printInt(minutes);
    printInt(seconds);

}

