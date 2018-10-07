#include <i86.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "io.h"
#include "protocol.h"
#include "log.h"

#define SERIAL 0x14
#define PORT 0

union REGS regs;

void io_init(void)
{
  log("io_init() - before port initialize.\r\n");
  // Initialize port.
  regs.x.dx = PORT;
  regs.h.ah = 0x04;
  int86(SERIAL, &regs, &regs);
  log("io_init() - after port initialize, returned %d\r\n",regs.x.ax);

  log("io_init() - About to set line characteristics.\r\n");
  // Set line characteristics. 
  regs.h.al = 0x07; // 1200bps
  regs.h.al = (regs.h.al < 5) | 0x03;   /* 8/N/1 */
  regs.x.dx = PORT;
  regs.h.ah = 0x00;
  int86(SERIAL,&regs,&regs);
  log("io_init() - Set line characteristics.\r\n");

  log("io_init() - Set flow control\r\n");
  /* // Set RTS/CTS Flow control */
  regs.h.ah = 0x0f;
  regs.h.al = 0x02;
  regs.x.dx = PORT;
  int86(SERIAL,&regs,&regs);
  log("io_init() - Set flow control success\r\n");

  io_raise_dtr();
}

void io_send_byte(unsigned char b)
{
  log("io_send_byte(0x%02x,%c)\r\n",b,b);
  regs.x.dx = PORT;
  regs.h.al = b;
  regs.h.ah = 0x01;
  int86(SERIAL, &regs, &regs);
  log("io_send_byte() end\r\n");
}

void io_lower_dtr(void)
{
  log("io_lower_dtr()\r\n");
  /* // Lower DTR */
  regs.h.ah = 0x06;
  regs.h.al = 0x00;
  regs.x.dx = PORT;
  int86(SERIAL,&regs,&regs);
  log("io_lower_dtr() successful\r\n");
}

void io_raise_dtr(void)
{
  log("io_raise_dtr(void)\r\n");
  /* // Raise DTR */
  regs.h.ah = 0x06;
  regs.h.al = 0x01;
  regs.x.dx = PORT;
  int86(SERIAL,&regs,&regs);
  log("io_raise_dtr(void) successful\r\n");
}

void io_hang_up(void)
{
  log("io_hang_up(void)\r\n");
  io_lower_dtr();
  delay(500);
  io_raise_dtr();
}

void io_main(void)
{
  unsigned char ch;
  // Get port status
  regs.x.dx = PORT;
  regs.h.al = 0;
  regs.h.ah = 3;
  int86(SERIAL, &regs, &regs);

  if ((regs.x.ax&0x100) !=0)
    {
      // Data is waiting, process it.
      regs.x.dx = PORT;
      regs.h.ah = 0x02;
      int86(SERIAL, &regs, &regs);
      ch=regs.x.ax&0x00ff;
      ShowPLATO(&ch,1);
    }
}

void io_done(void)
{
  io_hang_up();
  
  // Deinitialize driver.
  regs.h.ah = 0x05;
  regs.x.dx = PORT;
  int86(SERIAL,&regs,&regs);
}
