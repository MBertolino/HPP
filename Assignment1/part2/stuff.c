#include <stdio.h>
#include "stuff.h"


void print_pyramid(int pyramidSize) {
  int i, k, nBlanks, nStars;
  printf("In print_pyramid: value of G is %d\n", G);
  for(i = 0; i < pyramidSize; i++) {
    /* Blank space */
    nBlanks = pyramidSize - i;
    for(k = 0; k < nBlanks; k++)
      printf(" ");
    /* stars */
    nStars = 2*i + 1;
    for(k = 0; k < nStars; k++)
      printf("*");
    printf("\n");
  }
}
