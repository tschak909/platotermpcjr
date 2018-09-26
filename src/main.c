#include <stdbool.h>
#include "screen.h"
#include "io.h"
#include "terminal.h"
#include "splash.h"
#include "keyboard.h"

void main(int argc, char* argv[])
{
#ifdef TANDY_2000
  extern bool is_mono;
  if (strcasecmp(argv[1],"/COLOR"))
    is_mono==0;
  else if (strcasecmp(argv[1],"/MONO"))
    is_mono==1;
#endif

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
