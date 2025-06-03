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

uint64_t vd_get_char_width() {
	return CHAR_WIDTH;
}

uint64_t vd_get_char_height() {
	return CHAR_HEIGHT;
}

// void vd_drawCircle(int centerX, int centerY, int radius, Color color) {
//     Color * pixel;
//     int radiusSquared = radius * radius;
    
//     for (int y = centerY - radius; y <= centerY + radius; y++) {
//         int dy = y - centerY;
//         int dySquared = dy * dy;
        
//         // Encontrar el rango x válido para esta fila y
//         int startX = centerX - radius;
//         int endX = centerX + radius;
        
//         // Optimización: encontrar el primer x válido
//         while (startX <= centerX) {
//             int dx = startX - centerX;
//             if (dx * dx + dySquared <= radiusSquared) {
//                 break;
//             }
//             startX++;
//         }
        
//         // Encontrar el último x válido
//         while (endX >= centerX) {
//             int dx = endX - centerX;
//             if (dx * dx + dySquared <= radiusSquared) {
//                 break;
//             }
//             endX--;
//         }
        
//         // Dibujar la línea horizontal si hay píxeles válidos
//         if (startX <= endX) {
//             pixel = (Color*) getPixel(startX, y);
//             for (int x = startX; x <= endX; x++, pixel++) {
//                 *pixel = color;
//             }
//         }
//     }
// }


void vd_drawCircle(int centerX, int centerY, int radius, Color color) {
    int r2 = radius * radius;

    for (int dy = -radius; dy <= radius; dy++) {
        int y = centerY + dy;
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= r2) {
                Color *pixel = (Color*) getPixel(centerX + dx, y);
                *pixel = color;
            }
        }
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
	// (VBE_mode_info->bpp)/8 -> bytes por pixel
	for(uint32_t len = ((VBE_mode_info->bpp)/8) * (uint32_t)VBE_mode_info->width * VBE_mode_info->height; len; len--, current++){
		*current = 0;
	}
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
	//mask to know which bits to draw as text or background
	int mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

	const uint8_t * charMap = font_bitmap + 16 * (c -32);

	//checking that the cursor is within the screen bounds
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
		vd_prints("The size cannot be reduced any further.\n", 10, WHITE, BLACK);
	}
}

void vd_increase(){
	escalaPixel++;
}

static void scroll(){
    Color *current;
    Color *nextPixel;

	//Move all rows up
    for (int y = 0; y < VBE_mode_info->height - CHAR_HEIGHT; y++) {
        for (int x = 0; x < VBE_mode_info->width; x++) {
            current = (Color *)getPixel(x, y);
            nextPixel = (Color *)getPixel(x, y + CHAR_HEIGHT);
            *current = *nextPixel;
        }
    }

    // Fill the last row with black
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
	// uint8_t pixelWidth = VBE_mode_info->bpp/8;     
    // uint16_t pitch = VBE_mode_info->pitch;  

    // // CORRECTO: Primero calculamos el offset de la fila (y * pitch)
    // // luego añadimos el offset dentro de la fila (x * pixelWidth)
    // uintptr_t pixel = (uintptr_t)(VBE_mode_info->framebuffer) + (y * pitch) + (x * pixelWidth);
    // return (uint32_t*)pixel;
	
    uint8_t pixelwidth = VBE_mode_info->bpp/8;     //number of bytes to the next pixel to the right  (bpp: BITS per px)
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

uint64_t vd_get_width(){
	return VBE_mode_info->width;
}

uint64_t vd_get_height(){
	return VBE_mode_info->height;
}

void vd_set_cursorX(uint64_t x){
	if(x >= VBE_mode_info->width || x < 0){
		return;
	}
	cursorX = x;
}

void vd_set_cursorY(uint64_t y){
	if(y >= VBE_mode_info->height || y < 0){
		return;
	}
	cursorY = y;
}