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

static void drawChar(char c, Color fuente, Color fondo){
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

static void scroll(){
	Color * current;
	Color * nextPixel;

	for (int i = 0; i < VBE_mode_info->height - CHAR_HEIGHT; i++){
		for(int j = 0; j < VBE_mode_info->width - CHAR_WIDTH; j++){
			current = ( Color *) getPixel(i, j);
			nextPixel = ( Color *) getPixel(i + CHAR_HEIGHT, j);
			*current = *nextPixel;
		}
	}

	//que la linea de abajo despues de hacer el scroll hacia arriba sea negra
	for(int i = 0; i < CHAR_HEIGHT; i++){
		for(int j = 0; j < VBE_mode_info->width; j++){
			current = (Color *)getPixel(VBE_mode_info->height - CHAR_HEIGHT + i, j);
			*current = BLACK;
		}
	}
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

void driverRead(char *buff){
	*buff = getBuff();
	if(*buff == 0){
		return ;
	}
}
