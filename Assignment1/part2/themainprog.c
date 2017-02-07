#include <stdio.h>
#include <stdlib.h>
#include "stuff.h"

int main(int argc, char** argv) {
  int n = 5;
  if(argc > 1)
    n = atoi(argv[1]);
  printf("In main: value of G is %d\n", G);
  print_pyramid(n);
  return 0;
}
