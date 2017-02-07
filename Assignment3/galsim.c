#include "file_operations/file_operations.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	int a;
	int nLines = 10;
	double* buffer = (double*)malloc(nLines*sizeof(double));
	
	/* Read file */
	a = read_doubles_from_file(nLines, buffer, "input_data/circles_N_2.gal");
	for (int i = 0; i < nLines; i++) {
		printf("%lf\n", buffer[i]);
	}
  	printf("\na = %i", a);

	/* Write file*/
	a = write_doubles_to_file(nLines, buffer, "output.gal");
	printf("\na = %i\n", a);
	
	/* Read output file */
	printf("Read and print output file\n");
	a = read_doubles_from_file(nLines, buffer, "output.gal");
	//int j;
	for (int j = 0; j < nLines; j++) {
		printf("%lf\n", buffer[j]);
	}
  	printf("\na = %i\n", a);

	free(buffer);
	return 0;
}
