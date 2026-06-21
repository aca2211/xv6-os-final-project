// Simple priority test: fork several processes and set different priorities.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int i;

  for (i = 0; i < 4; i++) {
    int pid = fork();
    if (pid < 0) {
      printf("fork failed\n");
      return 1;
    }
    if (pid == 0) {
      int prio = 1 + i * 5;
      setpriority(getpid(), prio);
      printf("child pid=%d priority=%d started\n", getpid(), prio);
      for (int j = 0; j < 10; j++) {
        printf("pid=%d prio=%d iter=%d\n", getpid(), prio, j);
        pause(20);
      }
      return 0;
    }
  }

  for (i = 0; i < 4; i++) {
    wait(0);
  }

  printf("priotest done\n");
  return 0;
}
