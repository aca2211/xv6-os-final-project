// Low-priority periodic printer

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  // Set low priority
  setpriority(getpid(), 1);
  int i = 0;
  for (;;) {
    printf("low: tick %d\n", i++);
    pause(200); // sleep a while
  }
  return 0;
}
