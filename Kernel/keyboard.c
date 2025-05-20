#include <keyboard.h>

#define BUFFER_MAX 256

extern char getKey();
extern void guardar_registros();

int shift = 0;
int elems = 0;
unsigned char key;

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

static const keyboard_matriz[BUFFER_MAX] = { }
static const char * mapLetras[] = {minusc, mayusc};

void keyboardHandler(){
    key = getKey();

    if(key <= 0x79 || key == 0x38 || key == 0xAA || key == 0xB6){
      // if(key == 0x38){ //La tecla del ALT
      //       guardar_registros();
      // }


      // shift presionad
      if (key == 0x2A || key == 0x36) {
        shift = 1;
      }
      // shift no presionado
      if(key == 0xAA || key == 0xB6){
        shift = 0;
      }

      next();
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

