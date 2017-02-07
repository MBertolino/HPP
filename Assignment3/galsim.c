#include "file_operations/file_operations.h"
#include <stdio.h>
#include <stdlib.h>

// Define a particle structure
typedef struct particle {
  double x;
  double y;
  double m;
  double vx,
  double vy,
} particle_t;


int main(int argc, char *argv[]) {
  
  /* Check for correct number of input arguments */
  if (argc != 6) {
    printf("Error: Expected exactly 5 input arguments.");
    return -1;
  }
  
  /* Input variables */
  int N = atoi(argv[1]);
  char *filename = argv[2];
  int nsteps = atoi(argv[3]);
  double delta_t = atof(argv[4]);
  int graphics = atoi(argv[5]);
	
  /* Read file */
  char *fileDest = "input_data/";
  strcat(fileDest, filename);
	int flag = read_doubles_from_file(nLines, data, fileDest);
  printf("Reading input file: flag = %i\n", flag);
  
  /* Creating the particles */
	double data[N*5];
  particle_t *particles[N];
  for (int i = 0; i < N; i++) {
    particles[i]->x = data[5*i];
    particles[i]->y = data[5*i + 1];
    particles[i]->m = data[5*i + 2];
    particles[i]->vx = data[5*i + 3];
    particles[i]->vy = data[5*i + 4];
  }
  particle_t *particlesPrev[N];
  
  
  
  
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
