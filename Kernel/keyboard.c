#include <keyboard.h>
#include <naiveConsole.h>
#include <lib.h>
#include <videoDriver.h>

#define BUFFER_MAX 256

extern uint8_t inb(uint16_t port);
extern void get_key();
extern void save_registers();

extern char key;
extern char flag;

int alt = 0;
int shift = 0;
int elems = 0;
int read_index = 0;

char BUFFER[BUFFER_MAX] = {0};

static const char mayusc[] = {
      0,   27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+',
   '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
   '\n',    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
      0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0, '*',
      0, ' ',    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,    0,   0, '-',   0,   0,   0, '+',   0,   0,   0,   0,   0,
      0,   0,    0,   0,   0,   0
};

static const char minusc[] = {
      0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9',  '0', '-', '=',
   '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',  '[', ']',
   '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
      0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',    0, '*',
      0,  ' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,
      0,    0,   38,   0, '-',   37,   0,   39, '+',   0,   40,   0,    0,   0,
      0,    0,   0,   0,   0,   0,   0,   0,  0,    0,   0,   0,    0,   0,

};

static const char * mapaLetras[] = {minusc, mayusc};

// void keyboard_handler(){
//     // vd_prints("Tecla presionada: ", 16, WHITE, BLACK);

//     //Hasta acá está llegando
//     if(key <= 0x79 || key == 0x38 || key == 0xAA || key == 0xB6){
//       // if(key == 0x38){ //La tecla del ALT
//       //       guardar_registros();
//       // }
//       // shift presionad
//       if (key == 0x2A || key == 0x36) {
//         shift = 1;
//       }
//       // shift no presionado
//       if(key == 0xAA || key == 0xB6){
//         shift = 0;
//       }
//       char c = mapaLetras[shift][key];
//       vd_print(c, WHITE, BLACK);
//       next();
//     }
   
// }

void keyboard_handler() {
    get_key();
    // Solo procesamos make codes (bit 7 == 0)
    if ((key & 0x80) == 0) {
        if (key == 0x2A || key == 0x36) {
            shift = 1;  // Shift presionado
        } else if (key == 0x38) {
            alt = 1;
            save_registers();
        } else {
            // Cualquier otra tecla presionada
            next();
        }
    } else {
        key = key & 0x7F;
        // Código de tecla liberada (break code)
        if (key == 0x2A || key == 0x36) {
            shift = 0;  // Shift liberado
        }
    }
}


void next() {
    if( elems == BUFFER_MAX ) {
        elems = 0;
    } 
    BUFFER[ elems++ ] = mapaLetras[shift][(int)key];
}




// char getKeyboard(){
//     if((key >= 0x10 && key <= 0x19) || (key >= 0x1E && key <= 0x26) || (key >= 0x2C && key <= 0x32)){
//       return mapaLetras[shift][(int)key];
//     }
//     return mapaLetras[shift][(int)key];
// }

char getBuff() {
    if (read_index == elems) {
        return 0; // no hay teclas nuevas
    }
    if (read_index >= BUFFER_MAX) {
        read_index = 0;
    }
    return BUFFER[read_index++];
}

int getAltFlag(){
  int ans = alt;
  alt = 0;
  return ans;
}
