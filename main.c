#include "mylib.h"
#include "text.h"
#include "game.h"
#include "start_image.h"
#include "tileImages.h"
#include "screenImages.h"
//#include <debugging.h>
#include <stdlib.h> // Include this because we are using rand
#include <stdio.h> // Include this header because we are using sprintf

int main() {
	PALETTE[0] = RGB(0, 0, 0);
	PALETTE[1] = RGB(31, 31, 31);
	PALETTE[2] = RGB(31, 0, 0);
	int i = 3;
	for (i = 3; i < TILEIMAGES_PALETTE_SIZE; i++) {
		PALETTE[i] = tileImages_palette[i];
	}

	for (i = SCREENIMAGES_PALETTE_OFFSET; i < SCREENIMAGES_PALETTE_SIZE; i++) {
		PALETTE[i] = screenImages_palette[i];
	}

	

	REG_DISPCTL = MODE4 | BG2_ENABLE | BUFFER1FLAG;

	while(1) {
		//fillScreen4((u8) 50);
		__key_prev = __key_curr; // save the previous key hit
		__key_curr = ~BUTTONS & BUTTON_MASK; // get the current key hit
		if (KEY_HIT(BUTTON_START)) {
			switch(state) {
			case TITLE: // start the game
				state = NORMAL;
				startGame();
				break;
			case GAMEOVER: // reset
				reset();
				break;
			}
		} else if (KEY_HIT(BUTTON_SELECT)) {
			reset(); // reset whenever select is hit or at the start
		} else if (state == TITLE) {
			if (KEY_HIT(BUTTON_UP)) {
				moveMenu(-1);
			} else if (KEY_HIT(BUTTON_DOWN)) {
				moveMenu(1);
			}
		} else if (state == NORMAL) {
			if (KEY_HIT(BUTTON_UP)) {
				moveCursor(0, -1);
			} else if (KEY_HIT(BUTTON_DOWN)) {
				moveCursor(0, 1);
			} else if (KEY_HIT(BUTTON_LEFT)) {
				moveCursor(-1, 0);
			} else if (KEY_HIT(BUTTON_RIGHT)) {
				moveCursor(1, 0);
			} else if (KEY_HIT(BUTTON_A)) {
				reveal();
			} else if (KEY_HIT(BUTTON_B)) {
				toggleFlag();
			}
		}
		refresh();
		waitForVblank();
		flipPage();
	}
}
