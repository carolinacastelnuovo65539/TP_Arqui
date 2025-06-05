#include <keyboard.h>
#include <naiveConsole.h>
#include <lib.h>
#include <videoDriver.h>

#define BUFFER_MAX 256
#define TRUE 1
#define FALSE !TRUE

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
uint8_t pressed_keys[MAX_KEYS] = {FALSE};



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


void keyboard_handler() {
    get_key();
    if ((key & 0x80) == 0) {
        pressed_keys[key] = TRUE;   // la tecla esta presionada
        if (key == 0x2A || key == 0x36) {
            shift = 1;  // Shift pressed
        } else if (key == 0x38) {
            alt = 1;
            save_registers();
        } else {
            // any other key pressed
            next();
        }
    } else {
        key = key & 0x7F;
        pressed_keys[key] = FALSE;  // la tecla no esta presionada
        // break code
        if (key == 0x2A || key == 0x36) {
            shift = 0;  // Shift freed
        }
    }
}


void next() {
    if( elems == BUFFER_MAX ) {
        elems = 0;
    } 
    BUFFER[elems++] = mapaLetras[shift][(int)key];
}

char getBuff() {
    if (read_index == elems) {
        return 0; // no new keys
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

uint8_t * get_pressed_keys() {
    return pressed_keys;
}