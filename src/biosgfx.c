#include <i86.h>
#include "biosgfx.h"

#define INT_VIDEO 0x10

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
void mode(unsigned char mode)
{
  union REGS regs;
  regs.h.ah=0x00;
  regs.h.al=mode;
  int86(INT_VIDEO,&regs,&regs);
}

/**
 * Plot a dot to screen.
 *
 * short x - X position
 * short y - Y position
 * unsigned char c - Color
 */
void set(short x, short y, unsigned char c)
{
  union REGS regs;
  regs.h.ah=0x0C;
  regs.h.al=c;
  regs.h.bh=0; // Page 0
  regs.x.cx=x;
  regs.x.dx=y;
  int86(INT_VIDEO,&regs,&regs);
}

/**
 * Get dot color from screen.
 *
 * short x - X position
 * short y - Y position
 * return - Color
 */
int get(short x, short y)
{
  union REGS regs;
  regs.h.ah=0x0d;
  regs.h.bh=0; // Page 0
  regs.x.cx=x;
  regs.x.dx=y;
  int86(INT_VIDEO,&regs,&regs);
  return regs.h.al;
}

/**
 * Draw line (borrowed from beretta42's platoterm64 fork)
 *
 * short x1 - initial X position
 * short y1 - initial Y position
 * short x2 - final X position
 * short y2 - final Y position
 * unsigned char c - color
 */
void line(short x1, short y1, short x2, short y2, unsigned char c)
{
  short dx, dy; 
  short stepx, stepy;
  dx = x2 - x1;
  dy = y2 - y1;
  if (dx < 0) { 
    dx = -dx;
    stepx = -1;
  } else { 
    stepx = 1; 
  } 
  if (dy < 0)  { 
    dy = -dy;
    stepy = -1;
  } else {
    stepy = 1;
  } 
  dx <<= 1; 
  dy <<= 1;
  // draw first point 
  set(x1, y1,c);
  //check for shallow line
  if (dx > dy) { 
    int fraction = dy - (dx >> 1); 
    while (x1 != x2) { 
      if (fraction >= 0) {
	y1 += stepy;
	fraction -= dx;
      } 
      x1 += stepx;
      fraction += dy;
      set(x1, y1,c);
    }
  } //otherwise steep line 
  else { 
    int fraction = dx - (dy >> 1);
    while (y1 != y2) {
      if (fraction >= 0) {
	x1 += stepx;
	fraction -= dy;
      } 
      y1 += stepy; 
      fraction += dx; 
      set(x1, y1,c);
    }
  }
}

/**
 * Draw bar (with a bunch of lines)
 *
 * short x1 - initial left corner of box
 * short y1 - initial top corner of box
 * short x2 - final right corner of box
 * short y2 - final bottom corner of box
 * unsigned char c - Color
 */
void bar( short x1, short y1, short x2, short y2, unsigned char c)
{
  short d;
  short h;
  if (y1 < y2){
    d = 1;
    h = y2 - y1;
  } else {
    d = -1;
    h = y1 - y2;
  }
  do {
    line(x2,y1,x1,y1,c);
    y1 += d;
  } while (h--);
}

/**
 * Set background color
 *
 * unsigned char c - desired color
 */
void background(unsigned char c)
{
  union REGS regs;
  regs.h.ah=0x0B;
  regs.h.bh=0x00;
  regs.h.bl=c;
  int86(INT_VIDEO,&regs,&regs);
}

/**
 * dirt simple flood fill
 */
/* void flood(short x, short y, int new_col, int old_col) */
/* { */
/*   if (old_col==-1) */
/*     { */
/*       // check current pixel is old_color or not */
/*       if (get(x, y) != new_col) { */
	
/*         // put new pixel with new color */
/*         set(x, y, new_col); */
	
/*         // recursive call for bottom pixel fill */
/*         flood(x + 1, y, new_col, old_col); */
	
/*         // recursive call for top pixel fill */
/*         flood(x - 1, y, new_col, old_col); */
	
/*         // recursive call for right pixel fill */
/*         flood(x, y + 1, new_col, old_col); */
	
/*         // recursive call for left pixel fill */
/*         flood(x, y - 1, new_col, old_col); */
/*       } */
/*     } */
/*   else */
/*     { */
/*       // check current pixel is old_color or not */
/*       if (get(x, y) == old_col) { */
	
/*         // put new pixel with new color */
/*         set(x, y, new_col); */
	
/*         // recursive call for bottom pixel fill */
/*         flood(x + 1, y, new_col, old_col); */
	
/*         // recursive call for top pixel fill */
/*         flood(x - 1, y, new_col, old_col); */
	
/*         // recursive call for right pixel fill */
/*         flood(x, y + 1, new_col, old_col); */
	
/*         // recursive call for left pixel fill */
/*         flood(x, y - 1, new_col, old_col); */
/*       } */
/*     } */
/* } */
