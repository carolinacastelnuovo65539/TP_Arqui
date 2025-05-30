#include <videoDriver.h>
#include <font.h>


Color BLACK = {0, 0, 0};
Color WHITE = {255, 255, 255};

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

uint16_t cursorX = 0;
uint16_t cursorY = 0;
uint8_t escalaPixel = 1;
#define CHAR_WIDTH 8 * escalaPixel
#define CHAR_HEIGHT 16 * escalaPixel
uint16_t cursorOn = 0;

static void vd_putChar(char c, Color fuente, Color fondo);
static uint32_t* getPixel(uint16_t x, uint16_t y);
static void scroll();


void vd_prints(const char * str, int len, Color fuente, Color fondo) {
	for (int i=0; str[i] != '\0'; i++) {
		vd_print(str[i], fuente, fondo);
	}
}

void vd_print(const char buffer, Color fuente, Color fondo){
	switch (buffer)
	{
	case '\n':	
		newLine();
		break;
	case '\b':
		backspace();
		break;
	case '\0':
		break;
	default:
		vd_putChar(buffer, fuente, fondo);
		break;
	}
}

void vd_drawRectangle(int x, int y, int width, int height, Color color) {
	Color * pixel;
	for (int i = 0; i < height; i++) {
		pixel = (Color*) getPixel(x,y+i);
		for (int j = 0; j < width; j++, pixel++) {
			*pixel = color;
		}
	}
}

void vd_drawCircle(int centerX, int centerY, int radius, Color color) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;

    while (x <= y) {
        // Dibuja los puntos simétricos del círculo
        setPixel(centerX + x, centerY + y, color);
        setPixel(centerX - x, centerY + y, color);
        setPixel(centerX + x, centerY - y, color);
        setPixel(centerX - x, centerY - y, color);
        setPixel(centerX + y, centerY + x, color);
        setPixel(centerX - y, centerY + x, color);
        setPixel(centerX + y, centerY - x, color);
        setPixel(centerX - y, centerY - x, color);

        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void newLine(){
	cursorX = 0;
	cursorY += CHAR_HEIGHT;
	
	if(cursorY + CHAR_HEIGHT > VBE_mode_info->height){
		cursorY -= CHAR_HEIGHT;
		scroll();
	}
}

void backspace(){
	if(cursorX == 0 && cursorY == 0){
		return;
	}else{
		cursorX -= CHAR_WIDTH;
		vd_putChar(' ', WHITE, BLACK);
		cursorX -= CHAR_WIDTH;
	}
}

void clear(){
	uint8_t * current = (uint8_t*)((uint64_t)VBE_mode_info->framebuffer);
	for(uint32_t len = 3 * (uint32_t)VBE_mode_info->width * VBE_mode_info->height; len; len--, current++){
		*current = 0;
	}
	//los seteo en 0
	cursorX = 0;
	cursorY = 0;
}

void putCursor() {

	int cx, cy;
	Color fuente = cursorOn ? BLACK : WHITE;
	Color fondo = cursorOn ? BLACK : WHITE;
	
	int mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	
	const unsigned char * charMap;

	if (cursorOn) {
		charMap = font_bitmap + 16 * (' ' - 32);
		cursorOn = 0;
	} else {
		charMap = font_bitmap + 16 * (' ' - 32);
		cursorOn = 1;
	}

	if (cursorX >= VBE_mode_info->width) {
		cursorX = 0;
		if ((cursorY + CHAR_HEIGHT) > VBE_mode_info->height) {
			cursorY -= CHAR_HEIGHT;
			scroll();
		} else {
			cursorY += CHAR_HEIGHT;
		}
	}

	for (cy = 0; cy<16; cy++) {
		for (cx=0; cx<8; cx++){
			for (int i=0; i<escalaPixel; i++) {
				for (int j=0; j<escalaPixel; j++) {
					setPixel(cursorX + (8-cx) * escalaPixel + i, cursorY + cy * escalaPixel + j, charMap[cy] & mask[cx] ? fuente : fondo);
				}
			}
		}
	}
}

static void vd_putChar(char c, Color fuente, Color fondo){
	//mascara para chequear los bits y saber si pintar ese píxel como parte del texto o del fondo
	int mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

	const uint8_t * charMap = font_bitmap + 16 * (c -32);

	//me fijo que el cursor no se vaya de la linea o pantalla
	if (cursorX >= VBE_mode_info->width) {
		cursorX = 0;
		if ((cursorY + CHAR_HEIGHT) >= VBE_mode_info->height) {
			scroll();
		} else {
			cursorY += CHAR_HEIGHT;
		}
	}

	int cx, cy;
	for(cy = 0; cy < 16; cy++){
		for (cx = 0; cx < 8; cx++){
			for(int i = 0; i < escalaPixel; i++){
				for(int j = 0; j < escalaPixel; j++){
					setPixel(cursorX + (8 - cx) * escalaPixel + i, cursorY + cy * escalaPixel + j, charMap[cy] & mask[cx] ? fuente : fondo);
				}
			}
		}
	}
	cursorX += CHAR_WIDTH;
	
}

void vd_reduce(){
	if(escalaPixel > 1){
		escalaPixel--;
	}else{
		vd_prints("No se puede reducir mas el tamaño\n", 10, WHITE, BLACK);
	}
}

void vd_increase(){
	escalaPixel++;
}

static void scroll(){
    Color *current;
    Color *nextPixel;

    // Desplazar las filas hacia arriba
    for (int y = 0; y < VBE_mode_info->height - CHAR_HEIGHT; y++) {
        for (int x = 0; x < VBE_mode_info->width; x++) {
            current = (Color *)getPixel(x, y);
            nextPixel = (Color *)getPixel(x, y + CHAR_HEIGHT);
            *current = *nextPixel;
        }
    }

    // Rellenar la última fila con negro
    for (int y = VBE_mode_info->height - CHAR_HEIGHT; y < VBE_mode_info->height; y++) {
        for (int x = 0; x < VBE_mode_info->width; x++) {
            current = (Color *)getPixel(x, y);
            *current = BLACK;
        }
    }

	// Color * current;
	// Color * nextPixel;

	// for (int i = 0; i < VBE_mode_info->height - CHAR_HEIGHT; i++){
	// 	for(int j = 0; j < VBE_mode_info->width - CHAR_WIDTH; j++){
	// 		current = ( Color *) getPixel(i, j);
	// 		nextPixel = ( Color *) getPixel(i + CHAR_HEIGHT, j);
	// 		*current = *nextPixel;
	// 	}
	// }

	// //que la linea de abajo despues de hacer el scroll hacia arriba sea negra
	// for(int i = 0; i < CHAR_HEIGHT; i++){
	// 	for(int j = 0; j < VBE_mode_info->width; j++){
	// 		current = (Color *)getPixel(VBE_mode_info->height - CHAR_HEIGHT + i, j);
	// 		*current = BLACK;
	// 	}
	// }
}

static uint32_t* getPixel(uint16_t x, uint16_t y) {
    uint8_t pixelwidth = VBE_mode_info->bpp/8;     //la cantidad de bytes hasta el siguiente pixel a la derecha (bpp: BITS per px)
    uint16_t pixelHeight = VBE_mode_info->pitch;  

    uintptr_t pixel = (uintptr_t)(VBE_mode_info->framebuffer) + (x * pixelwidth) + (y * pixelHeight);
    return (uint32_t*)pixel;
	
}

void setPixel(uint16_t x, uint16_t y, Color color) {
	if (x >= VBE_mode_info->width || y >= VBE_mode_info->height) {
		return;
	}
	Color * pixel = (Color*) getPixel(x,y);
	*pixel = color;
}

void vd_read(char * buff){
	*buff = getBuff();
	if(*buff == 0){
		return;
	}
}
