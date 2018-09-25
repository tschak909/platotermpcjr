#include "screen.h"
#include "io.h"
#include "terminal.h"
#include "splash.h"
#include "keyboard.h"

void main(void)
{
  screen_init();
  io_init();
  terminal_init();
  ShowPLATO(splash,sizeof(splash));
  terminal_initial_position();
  for (;;)
    {
      io_main();
      keyboard_main();
    }
}
