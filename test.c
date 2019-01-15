#include "csprint.h"
#include <stdlib.h>

int main(int argc, const char *argv[])
{
  if (argc < 2) {
    printf("Usage: %s <int>\n", argv[0]);
    return 1;
  }

  int ip = atoi(argv[1]);
  csprintf("o inteiro %d equivale ao IP ${ip}!\n", ip, &ip);

  return 0;
}
