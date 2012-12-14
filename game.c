#include <stdlib.h>
#include <stdio.h>
#include <debugging.h>
#include "mylib.h"
#include "game.h"
#include "tileImages.h"
#include "screenImages.h"
#include "text.h"

int frame;
int difficulty;
int state;
//TILE *board; MALLOC IS MESSING UP
static volatile int isWon;
static int found;
static int falseFound;
static int revealed;
static int gameWidth;
static int gameHeight;
static int gameMines;
static int gameX;
static int gameY;
static volatile int delay;
static volatile int toDraw;
static volatile int gameXOld;
static volatile int gameYOld;
static volatile int redrawBackground1;
static volatile int redrawBackground2;
const int BOARD_WIDTH[] = {8, 16, 30};
const int BOARD_HEIGHT[] = {8, 16, 16};
const int BOARD_MINES[] = {10, 30, 99};
TILE board[30 * 16];
char buffer[41];

/*
* Called once per loop, handles the drawing functions based on state.
*/
void refresh() {
	switch(state) {
	case TITLE:
		drawImage4(0, 0, TITLESCREEN_WIDTH, TITLESCREEN_HEIGHT, (u16*)titlescreen);
		frame++;
		drawMenu();
		break;
	case NORMAL:
		if ((ISBUFFER1 && redrawBackground1) || (!ISBUFFER1 && redrawBackground2)) {
			fillScreen4(0);
		}
		frame++;
		drawBoard();
		drawCursor();
		drawTime();
		break;
	case EXPLOSION:
		drawBoard();
		drawTime();
		if (delay > 0 && !isWon) {
			TILE *tile = &board[gamePos];
			drawImage4(START_ROW + (PPI * gameY), START_COL + (PPI * gameX), PPI, PPI, getTileImage(*tile));
		} else if (isWon && delay > 0) {
			delay--;
		}
		if (delay == 0) {
			state = GAMEOVER;
		}
		break;
	case GAMEOVER:
		if (isWon) { // winner!
			drawImage4(0, 0, WINSCREEN_WIDTH, WINSCREEN_HEIGHT, (u16*)winscreen);
			drawTime();
			sprintf(buffer, "You win!");
		} else { // loser!
			drawImage4(0, 0, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, (u16*)gameover);
			drawTime();
			sprintf(buffer, "You lose!");

		}
		drawString4(120, 90, buffer, WHITE); // draw the appropriate text
		break;
	}
}

/*
* Resets game variables and draws the title screen.
*/
void reset() {
	state = TITLE;
	gameX = gameXOld = 0;
	gameY = gameYOld = 0;
	frame = 0;
	delay = 0;
	found = 0;
	falseFound = 0;
	revealed = 0;
	toDraw = 0;
}

/*
* Starts the game by setting the state to normal and drawing the appropriate things.
*/
void startGame() {
	redrawBackground1 = 1;
	redrawBackground2 = 1;
	int i;
	state = NORMAL;
	gameWidth = BOARD_WIDTH[difficulty];
	gameHeight = BOARD_HEIGHT[difficulty];
	gameMines = BOARD_MINES[difficulty];
	srand(frame); // seed the random function with the current frame
	/* Malloc randomly messes up, just use the biggest board possible
	volatile int amount = gameWidth * gameHeight * sizeof(int*);
	if (board == NULL) { // malloc initially
		board = (TILE *) malloc(amount);
	} else {
		TILE *newBoard = (TILE *) realloc(board, gameSize); // otherwise, reallocate
		free(board); // free the board momentarily
		board = NULL;
		board = newBoard;
	}
	*/
	for (int volatile x = 0; x < gameSize; x++) {
		board[x].value = 0;
		board[x].status = NONE;
		board[x].redrawBuffer1 = 1;
		board[x].redrawBuffer2 = 1;
		toDraw += 2;
	}

	i = 0;
	// randomly generate mines
	while (i < BOARD_MINES[difficulty]) {
		int randY = rand() % gameHeight;
		int randX = rand() % gameWidth;
		if (board[OFFSET(randY, randX, gameWidth)].value != MINE) {
			putMine(randY, randX);
			i++;
		}
	}

	frame = 0; // reset the frame
}

/*
* Ends the game and displays different things if it was won or not.
*/
void endGame(int won) {
	isWon = won;
	state = EXPLOSION;
	delay = 20;
	redrawBackground1 = 1;
	redrawBackground2 = 1;
}

/*
* Draws the menu, handles item selection.
*/
void drawMenu() {
	sprintf(buffer, "Easy");
	u8 color;
	color = (difficulty == EASY) ? RED : WHITE;
	drawString4(HEIGHT / 2, WIDTH / 2, buffer, color);
	sprintf(buffer, "Intermediate");
	color = (difficulty == INTERMEDIATE) ? RED : WHITE;
	drawString4(HEIGHT / 2 + 20, WIDTH / 2, buffer, color);
	sprintf(buffer, "Hard");
	color = (difficulty == HARD) ? RED : WHITE;
	drawString4(HEIGHT / 2 + 40, WIDTH / 2, buffer, color);
}

/*
* Goes through each tile and draws it.
*/
void drawBoard() {
	if (toDraw > 0 || redrawBackground1 || redrawBackground2) {
		for (int r = 0; r < gameHeight && (toDraw > 0 ||redrawBackground1 || redrawBackground2); r++) {
			for (int c = 0; c < gameWidth; c++) {
				TILE *tile = &board[OFFSET(r, c, gameWidth)];
				int draw = 0;
				if (ISBUFFER1 && tile->redrawBuffer1) {
					draw = 1;
					tile->redrawBuffer1 = 0;
					toDraw--;
				} else if (!ISBUFFER1 && tile->redrawBuffer2) {
					draw = 1;
					tile->redrawBuffer2 = 0;
					toDraw--;
				} else if ((ISBUFFER1 && redrawBackground1) || (!ISBUFFER1 && redrawBackground2)) {
					draw = 1;
				}

				if (draw == 1)
					drawImage4(START_ROW + (PPI * r), START_COL + (PPI * c), PPI, PPI, getTileImage(*tile));
			}
		}
	}
	if (ISBUFFER1)
		redrawBackground1 = 0;
	else
		redrawBackground2 = 0;
}

/*
* Gets the image corresponding to the given tile.
*/
u16* getTileImage(TILE tile) {
	int status = tile.status;
	int value = tile.value;
	switch (status) {
	case REVEALED:
		switch (value) {
		case 0:
			return (u16*) sq0;
		case 1:
			return (u16*) sq1;
		case 2:
			return (u16*) sq2;
		case 3:
			return (u16*) sq3;
		case 4:
			return (u16*) sq4;
		case 5:
			return (u16*) sq5;
		case 6:
			return (u16*) sq6;
		case 7:
			return (u16*) sq7;
		case 8:
			return (u16*) sq8;
		case MINE:
			if (delay > 15) {
				delay--;
				return (u16*) minered1;
			} else if (delay > 10) {
				delay--;
				return (u16*) minered2;
			} else if (delay > 5) {
				delay--;
				return (u16*) minered3;
			} else if (toDraw == 0) {
				delay--;
				return (u16*) minered4;
			}
		default:
			return NULL;
		}
		break;
	case NONE:
		if (state != NORMAL && value == MINE)
			return (u16*) mine;
		else
			return (u16*) sqt0;
	case FLAG:
		if (state == EXPLOSION && value == MINE)
			return (u16*) mine;
		else if (state == EXPLOSION && value != MINE)
			return (u16*) nomine;
		else
			return (u16*) sqt1;
	case UNKNOWN:
		if (state == EXPLOSION && value == MINE)
			return (u16*) mine;
		else if (state == EXPLOSION && value != MINE)
			return (u16*) nomine;
		else
			return (u16*) sqt2;
	default:
		return NULL;
	}
}

/*
* Draws the time. And also flag counts, whoop.
*/
void drawTime() {
	if ((frame / 60) % 60 < 10)
		sprintf(buffer, "Time: %d:0%d", frame / (60 * 60), (frame / 60) % 60);
	else
		sprintf(buffer, "Time: %d:%d", frame / (60 * 60), (frame / 60) % 60);
	drawRect4(140, 5, 18, 100, 0);
	drawString4(140, 5, buffer, WHITE);
	sprintf(buffer, "Flags: %d", gameMines - found - falseFound);
	drawString4(150, 5, buffer, WHITE);
}

/*
* Draws the cursor.
*/
void drawCursor() {
	drawRect4(START_ROW + PPI * gameY, START_COL + PPI * gameX, PPI/2, PPI/2, BLACK);
}

/*
* Moves the cursor in the given direction. Supports wrapping around.
*/
void moveCursor(int x, int y) {
	gameXOld = gameX;
	gameYOld = gameY;
	board[gamePosOld].redrawBuffer1 = 1;
	board[gamePosOld].redrawBuffer2 = 1;
	toDraw += 2;
	if (gameX + x >= 0 && gameX + x < gameWidth && gameY + y >= 0 && gameY + y < gameHeight) {
		gameX += x;
		gameY += y;
	} else if (gameX + x < 0) {
		gameX = gameWidth - 1;
	} else if (gameX + x >= gameWidth) {
		gameX = 0;
	} else if (gameY + y < 0) {
		gameY = gameHeight - 1;
	} else if (gameY + y >= gameWidth) {
		gameY = 0;
	}
}

/*
* Toggles the flag between none, flagged, and unknown.
*/
void toggleFlag() {
	board[gamePos].redrawBuffer1 = 1;
	board[gamePos].redrawBuffer2 = 1;
	toDraw += 2;
	switch (board[gamePos].status) {
	case REVEALED:
		break;
	case NONE:
		if (found + falseFound < gameMines) {
			board[gamePos].status = FLAG;
			if (board[gamePos].value == MINE)
				found++;
			else
				falseFound++;
		}
		break;
	case FLAG:
		board[gamePos].status = UNKNOWN;
		if (board[gamePos].value == MINE)
			found--;
		else
			falseFound--;
		break;
	case UNKNOWN:
		board[gamePos].status = NONE;
		break;
	}
	if (found == gameMines && falseFound == 0)
		endGame(1);
}

/*
* Reveals the currently selected tile.
*/
void reveal() {
	if (board[gamePos].status != REVEALED) {
		checkTile(gameY, gameX);
	}
	if (revealed == (gameSize - gameMines) || (found == gameMines && falseFound == 0))
		endGame(1);
}

/*
* Checks the given tile if it needs to be revealed and if it's valid. Cascades recursively.
*/
void checkTile(int r, int c) {
	int position = OFFSET(r, c, gameWidth);
	if (r < 0 || r >= gameHeight || c < 0 || c >= gameWidth || board[position].status == REVEALED) {
		return;
	}

	int wasFlagged = (board[position].status == FLAG);
	board[position].status = REVEALED;
	board[position].redrawBuffer1 = 1;
	board[position].redrawBuffer2 = 1;
	toDraw += 2;
	revealed++;
	if (board[position].value == MINE) {
		if (wasFlagged) found--;
		endGame(0);
	} else {
		if (wasFlagged)	falseFound--;
		if (board[position].value == 0) {
			checkTile(r - 1, c - 1);
			checkTile(r - 1, c);
			checkTile(r - 1, c + 1);
			checkTile(r, c - 1);
			checkTile(r, c + 1);
			checkTile(r + 1, c - 1);
			checkTile(r + 1, c);
			checkTile(r + 1, c + 1);
		}
	}
}

/*
* Puts a mine in the given location and produces mine counts for adjacent tiles.
*/
void putMine(int r, int c) {
	int randPos = OFFSET(r, c, gameWidth);
	board[randPos].value = MINE;
	countMines(r - 1, c - 1);
	countMines(r - 1, c);
	countMines(r - 1, c + 1);
	countMines(r, c - 1);
	countMines(r, c + 1);
	countMines(r + 1, c - 1);
	countMines(r + 1, c);
	countMines(r + 1, c + 1);
}

/*
* Increases the number of adjacent mines for the given tile if it isn't a mine.
*/
void countMines(int r, int c) {
	int position = OFFSET(r, c, gameWidth);
	if (r < 0 || r >= gameHeight || c < 0 || c >= gameWidth) {
		return;
	}

	if (board[position].value != MINE) {
		board[position].value++;
	}
}

/*
* Moves the menu option in the given direction.
*/
void moveMenu(int y) {
	if (difficulty == 0 && y == -1)
		difficulty = 2;
	else
		difficulty = (difficulty + y) % 3;
}
