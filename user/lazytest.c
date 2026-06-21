#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int big = 200 * 1024 * 1024; // 200MB logicos -- mas que los 128M reales de RAM

  printf("pidiendo %d bytes con sbrk()...\n", big);
  char *p = sbrk(big);
  if(p == (char*)-1){
    printf("sbrk fallo\n");
    exit(1);
  }
  printf("sbrk ok, no se asigno memoria fisica todavia\n");

  printf("tocando solo unas pocas paginas...\n");
  p[0] = 'a';
  p[4096 * 100] = 'b';
  p[big - 1] = 'c';
  printf("paginas tocadas sin errores\n");

  int pid = fork();
  if(pid == 0){
    printf("hijo: tambien puede tocar memoria heredada\n");
    p[4096 * 50] = 'd';
    exit(0);
  }
  wait(0);

  printf("achicando de vuelta...\n");
  sbrk(-big);
  printf("lazytest: OK\n");
  exit(0);
}