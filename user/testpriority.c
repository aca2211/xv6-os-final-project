#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
busy(int rounds)
{
  for(int i = 0; i < rounds; i++)
    for(volatile int j = 0; j < 5000000; j++);
}

int
main(int argc, char *argv[])
{
  int priorities[3] = {0, 5, 9}; // alta, media, baja
  int start = uptime();

  for(int i = 0; i < 3; i++){
    int pid = fork();
    if(pid == 0){
      int t0 = uptime();
      busy(6);
      int t1 = uptime();
      printf("hijo %d (prioridad %d, pid %d): ticks = %d\n",
             i, priorities[i], getpid(), t1 - t0);
      exit(0);
    } else {
      setpriority(pid, priorities[i]);
    }
  }
  for(int i = 0; i < 3; i++) wait(0);
  printf("total ticks: %d\n", uptime() - start);
  exit(0);
}