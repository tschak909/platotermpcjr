#ifndef BIOSGFX_H
#define BIOSGFX_H

#define PCJR_320_200_16 0x09
#define OLIVETTI_640_400_2 0x40

#define BLACK        0
#define BLUE         1
#define GREEN        2
#define CYAN         3
#define RED          4
#define MAGENTA      5
#define BROWN        6
#define LIGHTGREY    7
#define DARKGREY     8
#define LIGHTBLUE    9
#define LIGHTGREEN   10
#define LIGHTCYAN    11
#define LIGHTRED     12
#define LIGHTMAGENTA 13
#define YELLOW       14
#define WHITE        15

/**
 * Set video mode
 * 
 * unsigned char m - video mode
 * 01  40x25 16 color text (CGA,EGA,MCGA,VGA)
 * 02  80x25 16 shades of gray text (CGA,EGA,MCGA,VGA)
 * 03  80x25 16 color text (CGA,EGA,MCGA,VGA)
 * 04  320x200 4 color graphics (CGA,EGA,MCGA,VGA)
 * 05  320x200 4 color graphics (CGA,EGA,MCGA,VGA)
 * 06  640x200 B/W graphics (CGA,EGA,MCGA,VGA)
 * 07  80x25 Monochrome text (MDA,HERC,EGA,VGA)
 * 08  160x200 16 color graphics (PCjr)
 * 09  320x200 16 color graphics (PCjr)
 * 0A  640x200 4 color graphics (PCjr)
 * 0B  Reserved (EGA BIOS function 11)
 * 0C  Reserved (EGA BIOS function 11)
 * 0D  320x200 16 color graphics (EGA,VGA)
 * 0E  640x200 16 color graphics (EGA,VGA)
 * 0F  640x350 Monochrome graphics (EGA,VGA)
 * 10  640x350 16 color graphics (EGA or VGA with 128K)
 *	 640x350 4 color graphics (64K EGA)
 * 11  640x480 B/W graphics (MCGA,VGA)
 * 12  640x480 16 color graphics (VGA)
 * 13  320x200 256 color graphics (MCGA,VGA)
 */
void mode(unsigned char mode);

/**
 * Plot a dot to screen.
 *
 * short x - X position
 * short y - Y position
 * unsigned char c - Color
 */
void set(short x, short y, unsigned char c);

/**
 * Get dot color from screen.
 *
 * short x - X position
 * short y - Y position
 * return - Color
 */
int get(short x, short y);

/**
 * Draw line (borrowed from beretta42's platoterm64 fork)
 *
 * short x1 - initial X position
 * short y1 - initial Y position
 * short x2 - final X position
 * short y2 - final Y position
 * unsigned char c - color
 */
void line(short x1, short y1, short x2, short y2, unsigned char c);

/**
 * Draw bar (with a bunch of lines)
 *
 * short x1 - initial left corner of box
 * short y1 - initial top corner of box
 * short x2 - final right corner of box
 * short y2 - final bottom corner of box
 * unsigned char c - color
 */
void bar( short x1, short y1, short x2, short y2, unsigned char c );

/**
 * Set background color
 *
 * unsigned char c - desired color
 */
void background(unsigned char c);

/**
 * dirt simple flood fill
 */
/* void flood(short x, short y, int new_col, int old_col); */


#endif /* BIOSGFX_H */
