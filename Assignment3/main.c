#include "file_operations.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	int a;
	int nParticles = 2;
	double* buffer = (double*)malloc(nParticles*sizeof(double));
	a = read_doubles_from_file(nParticles, buffer, "filename");
	
	return 0;
}
