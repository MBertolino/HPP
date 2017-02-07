#include "file_operations.h"
#include <stdio.h>

int main() {
	int a;
	int nParticles = 2;
	double* buffer = malloc(nParticles*sizeof(double));
	a = read_doubles_from_file(nParticles, buffer, "filename");
	
	return 0;
}
