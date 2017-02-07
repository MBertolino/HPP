#include "file_operations/file_operations.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct particle {
  double x;
  double y;
  double m;
  double vx,
  double vy,
} particle_t;

int main() {
	int a;
  int nParticles = 2;
	int nLines = nParticles*5;
	double data[nLines];// = (double*)malloc(nLines*sizeof(double));
	
  /* Read file */
	a = read_doubles_from_file(nLines, data, "input_data/circles_N_2.gal");
	for (int i = 0; i < nLines; i++) {
		printf("%lf\n", data[i]);
	}
  printf("Reading input file: a = %i", a);
  
  particle_t *particles[nParticles];
  for (int i = 0; i < nParticles; i++) {
    particles[i]->x = data[5*i];
    particles[i]->y = data[5*i + 1];
    particles[i]->m = data[5*i + 2];
    particles[i]->vx = data[5*i + 3];
    particles[i]->vy = data[5*i + 4];
  }
  
  
  particle_t *particlesPrev = particles;
  
  
  /*
  Read data
  Loop over time
   - compute acceleration
   - compute velocity
   - compute position
   - (draw graphics)
  Save output
  Display results
  */
  
	/* Write file */
	a = write_doubles_to_file(nLines, data, "output.gal");
	printf("\na = %i\n", a);
  
  
  // Compare output with ref
  
  
	//free(data);
	return 0;
}
