#include "screen.h"
#include "protocol.h"
#include "biosgfx.h"
#include "font.h"
#include "scale.h"
#include "log.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <i86.h>

unsigned char CharWide=8;
unsigned char CharHigh=16;
unsigned char screen_mode;
extern padBool FastText; /* protocol.c */
padPt TTYLoc;
uint8_t FONT_SIZE_X;
uint8_t FONT_SIZE_Y;
uint16_t* scalex;
uint16_t* scaley;
uint8_t* font;
uint16_t* fontptr;
uint16_t width;
uint16_t height;
unsigned char current_foreground=WHITE;
unsigned char current_background=BLACK;
unsigned char fontm23[768];
unsigned char is_mono=0;

/**
 * screen_init() - Set up the screen
 */
void screen_init(void)
{
#ifdef OLIVETTI
  log("screen_init_for_olivetti\r\n");
  screen_mode=OLIVETTI_640_400_2;
  log("screen mode set to 0x%02x\r\n",screen_mode);
  width=640;
  log("width %d\r\n",width);
  height=400;
  log("height %d\r\n",height);
  FONT_SIZE_X=8;
  log("font X size: %d\r\n",FONT_SIZE_X);
  FONT_SIZE_Y=12;
  log("font Y size: %d\r\n",FONT_SIZE_Y);
  font=&font_640x400;
  log("Font set to 8x12 640x400 font\r\n");
  scalex=&scalex_640;
  log("scalex set to scalex_640\r\n");
  scaley=&scaley_400;
  log("scaley set to scaley_400\r\n");
  fontptr=&fontptr_12;
  log("fontptr set to fontptr_12\r\n");
  is_mono=true;
  log("is_mono = %d\r\n",is_mono);
#endif

#ifdef SANYO_MBC550
  screen_mode=0x03; // TBD
  log("screen_init for mbc550, mode 0x%02x\r\n",screen_mode);
  width=640;
  log("width: %d\r\n",width);
  height=200;
  log("height: %d\r\n",height);
  FONT_SIZE_X=8;
  log("font size X: %d\r\n",FONT_SIZE_X);
  FONT_SIZE_Y=6;
  log("font size Y: %d\r\n",FONT_SIZE_Y);
  font=&font_640x200;
  log("font set to 8x6 640x200 font.\r\n");
  scalex=&scalex_640;
  log("scalex set to scalex_640\r\n");
  scaley=&scaley_200;
  log("scaley set to scaley_200\r\n");
  fontptr=&fontptr_6;
  log("fontptr set to fontptr_6\r\n");
  is_mono=true;
  log("is_mono = %d\r\n\r\n",is_mono);
#endif
  
#ifdef TANDY_2000
  log("screen_init for tandy2k\r\n");
  if (is_mono==true)
    {
      screen_mode=TANDY2000_640_400_2;
      log("/MONO 640x400x2 mode selected. 0x%02x\r\n",screen_mode);
    }
  else
    {
      screen_mode=TANDY2000_640_400_16;
      log("/COLOR 640x400x16 mode selected. 0x%02x\r\n",screen_mode);
    }
  width=640;
  log("width: %d\r\n",width);
  height=400;
  log("height: %d\r\n",height);
  FONT_SIZE_X=8;
  log("font_size_x: %d\r\n",font_size_x);
  FONT_SIZE_Y=12;
  log("font_size_y: %d\r\n",font_size_y);
  font=&font_640x400;
  log("font set to 640x400 font\r\n");
  scalex=&scalex_640;
  log("font scalex set to scalex_640\r\n");
  scaley=&scaley_400;
  log("font scaley set to scaley_400\r\n");
  fontptr=&fontptr_12;
  log("fontptr set to fontptr_12\r\n");
#endif
  
#ifdef PCJR
  log("screen_init for pcjr\r\n");
  screen_mode=PCJR_320_200_16;
  log("screen_mode set to 0x%02x\r\n",screen_mode);
  width=320;
  log("width: %d\r\n",width);
  height=200;
  log("height: %d\r\n",height);
  FONT_SIZE_X=5;
  log("font size X: %d\r\n",FONT_SIZE_X);
  FONT_SIZE_Y=6;
  log("font size Y: %d\r\n",FONT_SIZE_Y);
  font=&font_320x200;
  log("font set to 5x6 320x200\r\n");
  scalex=&scalex_320;
  log("scalex table set to scalex_320\r\n");
  scaley=&scaley_200;
  log("scaley table set to scaley_200\r\n");
  fontptr=&fontptr_6;
  log("fontptr set to fontptr_6\r\n");
#endif
  log("about to set set mode: 0x%02x\r\n",screen_mode);
  mode(screen_mode);
}

/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
}

/**
 * screen_clear - Clear the screen
 */
void screen_clear(void)
{
  mode(screen_mode);
  /* background(current_background); */
}

/**
 * screen_wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{
  // initial naive implementation, draw a bunch of horizontal lines the size of bounding box.
  if (CurMode==ModeErase || CurMode==ModeInverse)
    current_foreground=current_background;
  else
    current_foreground=current_foreground;

  bar(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y],current_foreground);
}

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    current_foreground=current_background;
  else
    current_foreground=current_foreground;
  set(scalex[Coord->x],scaley[Coord->y],current_foreground);
}

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    current_foreground=current_background;
  else
    current_foreground=current_foreground;
  line(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y],current_foreground);
}

/**
 * screen_char_draw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count)
{
  short offset; /* due to negative offsets */
  unsigned short x;      /* Current X and Y coordinates */
  unsigned short y;
  unsigned short* px;   /* Pointers to X and Y coordinates used for actual plotting */
  unsigned short* py;
  unsigned char i; /* current character counter */
  unsigned char a; /* current character byte */
  unsigned char j,k; /* loop counters */
  char b; /* current character row bit signed */
  unsigned char width=FONT_SIZE_X;
  unsigned char height=FONT_SIZE_Y;
  unsigned short deltaX=1;
  unsigned short deltaY=1;
  unsigned char mainColor=WHITE;
  unsigned char altColor=BLACK;
  unsigned char *p;
  unsigned char* curfont;
  
  switch(CurMem)
    {
    case M0:
      curfont=font;
      offset=-32;
      break;
    case M1:
      curfont=font;
      offset=64;
      break;
    case M2:
      curfont=fontm23;
      offset=-32;
      break;
    case M3:
      curfont=fontm23;
      offset=32;      
      break;
    }

  if (CurMode==ModeRewrite)
    {
      altColor=current_background;
    }
  else if (CurMode==ModeInverse)
    {
      altColor=current_foreground;
    }
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    mainColor=current_background;
  else
    mainColor=current_foreground;

  current_foreground=mainColor;
  
  x=scalex[(Coord->x&0x1FF)];
  y=scaley[(Coord->y)+14&0x1FF];
  
  if (FastText==padF)
    {
      goto chardraw_with_fries;
    }

  /* the diet chardraw routine - fast text output. */
  
  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[fontptr[a]];
      
      for (j=0;j<FONT_SIZE_Y;++j)
  	{
  	  b=*p;
	  
  	  for (k=0;k<FONT_SIZE_X;++k)
  	    {
  	      if (b&0x80) /* check sign bit. */
		{
		  set(x,y,mainColor);
		}

	      ++x;
  	      b<<=1;
  	    }

	  ++y;
	  x-=width;
	  ++p;
  	}

      x+=width;
      y-=height;
    }

  return;

 chardraw_with_fries:
  if (Rotate)
    {
      deltaX=-abs(deltaX);
      width=-abs(width);
      px=&y;
      py=&x;
    }
    else
    {
      px=&x;
      py=&y;
    }
  
  if (ModeBold)
    {
      deltaX = deltaY = 2;
      width<<=1;
      height<<=1;
    }
  
  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[fontptr[a]];
      for (j=0;j<FONT_SIZE_Y;++j)
  	{
  	  b=*p;

	  if (Rotate)
	    {
	      px=&y;
	      py=&x;
	    }
	  else
	    {
	      px=&x;
	      py=&y;
	    }

  	  for (k=0;k<FONT_SIZE_X;++k)
  	    {
  	      if (b&0x80) /* check sign bit. */
		{
		  if (ModeBold)
		    {
		      set(*px+1,*py,mainColor);
		      set(*px,*py+1,mainColor);
		      set(*px+1,*py+1,mainColor);
		    }
		  set(*px,*py,mainColor);
		}
	      else
		{
		  if (CurMode==ModeInverse || CurMode==ModeRewrite)
		    {
		      if (ModeBold)
			{
			  set(*px+1,*py,altColor);
			  set(*px,*py+1,altColor);
			  set(*px+1,*py+1,altColor);
			}
		      set(*px,*py,altColor); 
		    }
		}

	      x += deltaX;
  	      b<<=1;
  	    }

	  y+=deltaY;
	  x-=width;
	  ++p;
  	}

      Coord->x+=width;
      x+=width;
      y-=height;
    }

  return;
}

/**
 * screen_tty_char - Called to plot chars when in tty mode
 */
void screen_tty_char(padByte theChar)
{
  if ((theChar >= 0x20) && (theChar < 0x7F)) {
    screen_char_draw(&TTYLoc, &theChar, 1);
    TTYLoc.x += CharWide;
  }
  else if ((theChar == 0x0b)) /* Vertical Tab */
    {
      TTYLoc.y += CharHigh;
    }
  else if ((theChar == 0x08) && (TTYLoc.x > 7))	/* backspace */
    {
      TTYLoc.x -= CharWide;
      // screen_block_draw(&scalex[TTYLoc.x],&scaley[TTYLoc.y],&scalex[TTYLoc.x+CharWide],&scaley[TTYLoc.y+CharHigh]);
    }
  else if (theChar == 0x0A)			/* line feed */
    TTYLoc.y -= CharHigh;
  else if (theChar == 0x0D)			/* carriage return */
    TTYLoc.x = 0;
  
  if (TTYLoc.x + CharWide > 511) {	/* wrap at right side */
    TTYLoc.x = 0;
    TTYLoc.y -= CharHigh;
  }
  
  if (TTYLoc.y < 0) {
    screen_clear();
    TTYLoc.y=495;
  }
}

/**
 * screen_color - return closest match to requested color.
 */
unsigned char screen_color(padRGB* theColor)
{
  unsigned char newRed;
  unsigned char newGreen;
  unsigned char newBlue;

  newRed=(theColor->red*0.008);
  newGreen=(theColor->green*0.008);
  newBlue=(theColor->blue*0.008);

  if (is_mono==1)
    {
      if ((theColor->red==0) && (theColor->green==0) && (theColor->blue==0))
	{
	  return 0;
	}
      else
        {
#ifdef SANYO_MBC550
	  return 2; // emulating CGA I guess...
#else
	  return 1;
#endif
	}
    }
  else
    {
      if ((newRed==0) && (newGreen==0) && (newBlue==0))
	return BLACK;
      else if ((newRed==1) && (newGreen==1) && (newBlue==1))
	return LIGHTGREY;
      else if ((newRed==0) && (newGreen==0) && (newBlue==1))
	return BLUE;
      else if ((newRed==0) && (newGreen==0) && (newBlue==2))
	return LIGHTBLUE;
      else if ((newRed==0) && (newGreen==1) && (newBlue==0))
	return GREEN;
      else if ((newRed==0) && (newGreen==2) && (newBlue==0))
	return LIGHTGREEN;
      else if ((newRed==0) && (newGreen==1) && (newBlue==1))
	return CYAN;
      else if ((newRed==0) && (newGreen==2) && (newBlue==2))
	return LIGHTCYAN;
      else if ((newRed==1) && (newGreen==0) && (newBlue==0))
	return RED;
      else if ((newRed==2) && (newGreen==0) && (newBlue==0))
	return LIGHTRED;
      else if ((newRed==1) && (newGreen==0) && (newBlue==1))
	return MAGENTA;
      else if ((newRed==2) && (newGreen==0) && (newBlue==2))
	return LIGHTMAGENTA;
      else if ((newRed==1) && (newGreen==1) && (newBlue==0))
	return BROWN;
      else if ((newRed==2) && (newGreen==2) && (newBlue==0))
	return YELLOW;
      else if ((newRed==2) && (newGreen==2) && (newBlue==2))
	return WHITE;
    }
  return WHITE;
}

/**
 * screen_foreground - Called to set foreground color.
 */
void screen_foreground(padRGB* theColor)
{
  current_foreground=screen_color(theColor);
}

/**
 * screen_background - Called to set foreground color.
 */
void screen_background(padRGB* theColor)
{
  current_background=screen_color(theColor);
}

/**
 * screen_paint - Called to paint at location.
 */
void screen_paint(padPt* Coord)
{
  //  flood(scalex[Coord->x],scaley[Coord->y],current_foreground,-1);
}

/**
 * touch_allow - Set whether touchpanel is active or not.
 */
void touch_allow(padBool allow)
{
}
