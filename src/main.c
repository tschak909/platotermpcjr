#include <stdbool.h>
#include "screen.h"
#include "io.h"
#include "terminal.h"
#include "splash.h"
#include "keyboard.h"
#include "log.h"

void main(int argc, char* argv[])
{
#ifdef TANDY_2000
  extern bool is_mono;
  if (strcasecmp(argv[1],"/COLOR"))
    is_mono==0;
  else if (strcasecmp(argv[1],"/MONO"))
    is_mono==1;
#endif

  log("before screen_init()\r\n");
  screen_init();
  log("after screen_init()\r\n");
  log("before io_init();\r\n");
  io_init();
  log("after io_init();\r\n");
  log("before terminal_init();\r\n");
  terminal_init();
  log("after terminal_init();\r\n");
  log("sending splash size %d to ShowPLATO\r\n",sizeof(splash));
  ShowPLATO(splash,sizeof(splash));
  log("splash sent.\r\n");
  log("terminal_set_initial_position()\r\n");
  terminal_initial_position();
  log("terminal bringup successful. Falling into mainloop.\r\n");
  for (;;)
    {
      io_main();
      keyboard_main();
    }
}
