#include "file_operations/file_operations.h"
#include "structures.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
  
  /* Check for correct number of input arguments */
  if (argc != 6) {
    printf("Error: Expected exactly 5 input arguments.");
    return -1;
  }
  
  /* Input variables */
  short N = (short)atoi(argv[1]);
  char *filename = argv[2];
  int nsteps = atoi(argv[3]);
  double delta_t = atof(argv[4]);
  //int graphics = atoi(argv[5]);
	
  /* Constants */
  const double G = 100/(double)N;
  const double epsilon = 0.001;
  
  /* Read file */
  char *fileDest = "input_data/";
  strcat(fileDest, filename);
  double data[N*5];
	int flag = read_doubles_from_file(N*5, data, fileDest);
  printf("Reading input file: flag = %i\n", flag);
  
  /* Creating the particles */
  particle_t *particles[N];
  for (short i = 0; i < N; i++) {
    particles[i]->x = data[5*i];
    particles[i]->y = data[5*i + 1];
    particles[i]->m = data[5*i + 2];
    particles[i]->vx = data[5*i + 3];
    particles[i]->vy = data[5*i + 4];
  }
  particle_t *particlesPrev[N];
  
  /* Loop over time */
  for (short k = 0; k < nsteps; k++) {
    
    // Update particles
    for (short i = 0; i < N; i++) {
      particles[i] = update(i, G, particlesPrev, N, epsilon, delta_t);
    }
    
    // Update previous particles
    *particlesPrev = *particles;
    //for (short j = 0; j < N; j++) {
    //  particlesPrev[j] = particles[j];
    //}
    
    /*
    Do graphics.
    */
  }
  
	/* Write file */
	flag = write_doubles_to_file(N*5, data, "output.gal");
	printf("Writing output file: flag = %i\n", flag);
  
  // Compare output with ref
  
	//free(data);
	return 0;
}
