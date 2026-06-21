#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  uint64 before, after_sbrk, after_touch;
  int small = 40 * 1024 * 1024; // 40MB -- cabe en los 128MB de RAM

  before = freepages();
  printf("paginas libres antes de sbrk: %ld\n", before);

  char *p = sbrk(small);
  if(p == (char*)-1){
    printf("sbrk fallo\n");
    exit(1);
  }

  after_sbrk = freepages();
  printf("paginas libres justo despues de sbrk(%d bytes): %ld\n", small, after_sbrk);
  printf("paginas consumidas SOLO por pedir memoria: %ld\n", before - after_sbrk);

  for(int i = 0; i < 10; i++)
    p[i * 4096] = 'x';   // tocamos solo 10 paginas de las miles pedidas

  after_touch = freepages();
  printf("paginas libres despues de tocar 10 paginas: %ld\n", after_touch);
  printf("paginas consumidas por el toque real: %ld\n", after_sbrk - after_touch);

  sbrk(-small);
  exit(0);
}