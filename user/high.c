// High-priority busy loop

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  // Set high priority
  setpriority(getpid(), 20);
  for (;;)
    ;
  return 0;
}
