// Patrick Stoica
/* Aliases */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#define WIDTH 240
#define HEIGHT 160

/* Images */

/* Buttons */
#define BUTTON_A      (1<<0)
#define BUTTON_B      (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_RIGHT  (1<<4)
#define BUTTON_LEFT   (1<<5)
#define BUTTON_UP     (1<<6)
#define BUTTON_DOWN   (1<<7)
#define BUTTON_R      (1<<8)
#define BUTTON_L      (1<<9)

/* GBA Settings */
#define MODE4 4
#define BUFFER0 (u16 *)0x6000000
#define BUFFER1 (u16 *)0x600A000
#define BUFFER1FLAG (1<<4)
#define ISBUFFER1 (REG_DISPCTL & BUFFER1FLAG)

#define PALETTE ((u16 *)0x5000000)
#define REG_DISPCTL *(u16 *)0x4000000
#define RGB(r, g, b)   ((r)  |  (g)<<5 |  (b)<<10)
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define BUTTONS (*( unsigned int *)0x04000130)
#define BUTTON_MASK     0x03FF
#define KEY_HIT(key) ((__key_curr & ~__key_prev) & key)
#define KEY_HELD(key) ((__key_curr &  __key_prev) & key)
#define SCANLINECOUNTER (*(unsigned short *)0x4000006)
#define OFFSET(r, c, rowlen) ((r)*(rowlen)+(c))

/* Colors */
/* Obsolete
#define RED RGB(31,0,0)
#define GREEN RGB(0,31,0)
#define BLUE RGB(0,0,31)
#define YELLOW  RGB(31,31,0)
#define ORANGE  RGB(31,20,0)
#define PURPLE RGB(31,0,31)
#define CYAN RGB(0,31,31)
#define BLACK 0
#define WHITE RGB(31,31,31)
#define LTGREY RGB(12, 12, 12)
#define BGCOLOR LTGREY
#define BOARDCOLOR BLACK
*/

/* External Variables */
extern u16 *videoBuffer;
extern u16 __key_curr, __key_prev;

/* DMA */

typedef struct {
	const volatile void *src;
	volatile void *dst;
	volatile u32 cnt;
} DMAREC;

#define DMA ((volatile DMAREC *)0x040000B0)

/* Defines */
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

// Note: The next 4 lines can tell you the default: DMA_DESTINATION_INCREMENT
// as well as which bits are used for this (i.e. 22 and 21)
#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)  // Used for sound

#define DMA_16 (0 << 26)  // Typically use this
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28) // Typically use this
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)  // The on switch!!!!

/* mylib.c */
void setPixel4(int row, int col, u8 index);
void drawRect4(int row, int col, int height, int width, u8 index);
void waitForVblank();
void drawImage4(int r, int c, int width, int height, const
u16* image);
void flipPage();
void fillScreen4(u8 color);
