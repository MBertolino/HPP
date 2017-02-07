#include "file_operations/file_operations.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	int a;
	int nLines = 10;
	double* buffer = (double*)malloc(nLines*sizeof(double));
	a = read_doubles_from_file(nLines, buffer, "input_data/circles_N_2.gal");
  
  int i;
  for (i = 0; i < nLines; i++) {
    printf("%lf\n", buffer[i]);
  }
  
	printf("\na = %i", a);
  
  free(buffer);
	return 0;
}
