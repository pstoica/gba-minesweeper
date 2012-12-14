// Patrick Stoica
#include "mylib.h"

u16 *videoBuffer = (u16 *) 0x6000000; // start of the video buffer

// Key state globals
u16 __key_curr = 0, __key_prev = 0;

/* Obsolete
// A function to set pixel (r, c) to the color passed in
void setPixel(int r, int c, u16 color) {
	videoBuffer[OFFSET(r, c, 240)] = color;
}

// A function to draw a FILLED rectangle starting at (r, c)
void drawRect(int r, int c, int width, int height, u16 color) {
	// we're basically looping through pixel by pixel
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			setPixel(r + i, c + j, color);
		}
	}
}*/

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

/* drawimage4
* A function that will draw an arbitrary sized image
* onto the screen (with DMA).
* @param r row to draw the image
* @param c column to draw the image
* @param width width of the image
* @param height height of the image
* @param image Pointer to the first element of the image.
*/
void drawImage4(int r, int c, int width, int height, const
u16* image) {
	for (int y = 0; y < height; y++) {
		DMA[3].src = &image[OFFSET(y, 0, width / 2)];
		DMA[3].dst = videoBuffer + OFFSET(r + y, c, WIDTH)/2;
		DMA[3].cnt = (width/2) | DMA_ON;
	}
}



/* setPixel4
* A mode 4 implementation of setPixel.
*/
void setPixel4(int row, int col, u8 index) {
	int whichPixel = OFFSET(row, col, 240);
	int whichShort = whichPixel/2;
	if (col & 1) {
		// odd column
		videoBuffer[whichShort] = (videoBuffer[whichShort] & 0x00FF) | (index << 8);
	} else {
		videoBuffer[whichShort] = (videoBuffer[whichShort] & 0xFF00) | (index);
	}
}

/* drawRect4
* A Mode 4 implementation of drawRect. Must have even width.
*/
void drawRect4(int row, int col, int height, int width, u8 index) {
	volatile u16 color = index | (index << 8);
	int r;
	for(r = 0; r < height; r++) {
		DMA[3].src = &color;
		DMA[3].dst = videoBuffer + OFFSET(row + r, col, WIDTH)/2;	
		DMA[3].cnt = (width/2) | DMA_ON | DMA_SOURCE_FIXED;
	}
}

/* fillScreen4
* Fills the screen with the given color index.
*/
void fillScreen4(u8 index) {
	volatile u16 color = index | (index << 8);
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = ((WIDTH * HEIGHT) / 2) | DMA_ON | DMA_SOURCE_FIXED;
}

void flipPage() {
	if(REG_DISPCTL & BUFFER1FLAG) {
		// We have been displaying BUFFER1
		REG_DISPCTL &= ~BUFFER1FLAG;
		videoBuffer = BUFFER1;
	} else {
		// WE have been displaying BUFFER0
		REG_DISPCTL |= BUFFER1FLAG;
		videoBuffer = BUFFER0;
	}
}
