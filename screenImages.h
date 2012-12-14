/*
 * Exported with BrandonTools v0.9
 * Invocation command was BrandonTools -mode4 -resize=240,160 -start=241 -palette=15 screenImages gameover.png titlescreen.png winscreen.png 
 * Time-stamp: Friday 04/06/2012, 03:42:20
 * 
 * Image Information
 * -----------------
 * gameover.png 240@160
 * titlescreen.png 240@160
 * winscreen.png 240@160
 * 
 * Quote/Fortune of the Day!
 * -------------------------
 * Today is the last day of some of your life.  ~Author Unknown
 * 
 * All bug reports / feature requests are to be sent to Brandon (brandon.whitehead@gatech.edu)
 */

#ifndef SCREENIMAGES_BITMAP_H
#define SCREENIMAGES_BITMAP_H

#define SCREENIMAGES_PALETTE_SIZE 256
#define SCREENIMAGES_PALETTE_OFFSET 241

extern const unsigned short screenImages_palette[256];

extern const unsigned short gameover[19200];
#define GAMEOVER_WIDTH 240
#define GAMEOVER_HEIGHT 160

extern const unsigned short titlescreen[19200];
#define TITLESCREEN_WIDTH 240
#define TITLESCREEN_HEIGHT 160

extern const unsigned short winscreen[19200];
#define WINSCREEN_WIDTH 240
#define WINSCREEN_HEIGHT 160

#endif