#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
busy(int rounds)
{
  for(int i = 0; i < rounds; i++)
    for(volatile int j = 0; j < 30000000; j++);  // workload mas grande
}

int
main(int argc, char *argv[])
{
  int priorities[3] = {0, 5, 9}; // alta, media, baja
  int fds[2];
  if(pipe(fds) < 0){ printf("pipe fallo\n"); exit(1); }

  int start = uptime();

  for(int i = 0; i < 3; i++){
    int pid = fork();
    if(pid == 0){
      close(fds[0]);          // el hijo solo escribe
      int t0 = uptime();
      busy(10);
      int t1 = uptime();
      int result[3] = {i, getpid(), t1 - t0};
      write(fds[1], result, sizeof(result));
      close(fds[1]);
      exit(0);
    } else {
      setpriority(pid, priorities[i]);
    }
  }
  close(fds[1]);              // el padre solo lee

  // Solo el padre imprime -> sin interleaving en consola
  for(int i = 0; i < 3; i++){
    int result[3];
    read(fds[0], result, sizeof(result));
    printf("hijo %d (prioridad %d, pid %d): ticks = %d\n",
           result[0], priorities[result[0]], result[1], result[2]);
  }
  close(fds[0]);

  for(int i = 0; i < 3; i++) wait(0);
  printf("total ticks: %d\n", uptime() - start);
  exit(0);
}