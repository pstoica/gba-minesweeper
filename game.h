#define MINE 9
#define gameSize (gameHeight * gameWidth)
#define gamePos OFFSET(gameY, gameX, gameWidth)
#define gamePosOld OFFSET(gameYOld, gameXOld, gameWidth)
#define START_ROW 0
#define START_COL 0
#define DENSITY 5
#define PPI 8

// Game state
enum {TITLE, NORMAL, EXPLOSION, GAMEOVER};

// Cell status
enum {NONE, FLAG, UNKNOWN, REVEALED};

// Difficulties
enum {EASY, INTERMEDIATE, HARD};

// Colors
enum {BLACK, WHITE, RED};

/* Struture representing a tile */
typedef struct {
	int value; // mines surrounding, or if it is a MINE
	int status; // flagged, etc.
	volatile int redrawBuffer1;
	volatile int redrawBuffer2;
} TILE;

extern int frame;
extern int state;
extern const int BOARD_WIDTH[3];
extern const int BROAD_HEIGHT[3];
extern const int BOARD_MINES[3];

void refresh();
void reset();
void startGame();
void endGame(int won);
void drawBoard();
void drawCursor();
void moveCursor(int x, int y);
void checkTile(int r, int c);
void reveal();
void toggleFlag();
void putMine(int r, int c);
void countMines(int r, int c);
u16* getTileImage(TILE tile);
void drawTime();
void drawMenu();
void moveMenu(int y);
