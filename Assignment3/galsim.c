#define _BSD_SOURCE

#include "file_operations/file_operations.h"
#include "graphics/graphics.h"
#include "structures.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Sleep function

const int windowWidth = 800;

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
  const int graphics = atoi(argv[5]);
  
  /* Setup graphics */
  if (graphics) {
    InitializeGraphics(argv[5], windowWidth, windowWidth);
    SetCAxes(0,1);
  }
  float L = 1;
  float W = 1;
  float radius = 0.01*L;
  float circleColor = 0;
  
  /* Constants */
  const double G = 100/(double)N;
  const double epsilon = 0.001;
  
  /* Read file */
  char fileDest[50];
  strcpy(fileDest, "input_data/");
  strcat(fileDest, filename);
  double data[N*5];
  int flag = read_doubles_from_file(N*5, data, fileDest);
  printf("Reading input file: flag = %i\n", flag);
  
  /* Creating the particles */
  particle_t **particles = (particle_t**)malloc(N*sizeof(particle_t*));
  //particle_t *particles[N];
  for (short i = 0; i < N; i++) {
    particles[i] = (particle_t*)malloc(sizeof(particle_t));
    particles[i]->x = data[5*i];
    particles[i]->y = data[5*i + 1];
    particles[i]->m = data[5*i + 2];
    particles[i]->vx = data[5*i + 3];
    particles[i]->vy = data[5*i + 4];
  }
  
  /* Create an array for the previous particle states */
  particle_t **particlesPrev = (particle_t**)malloc(N*sizeof(particle_t*));
  //particle_t *particlesPrev[N];
  for (short i = 0; i < N; i++) {
    particlesPrev[i] = (particle_t*)malloc(sizeof(particle_t));
  }
  
  /* Loop over time */
  for (short k = 0; k < nsteps; k++) {
    printf("%d\n", k);
     
    /* Update previous particles */
    for (short j = 0; j < N; j++) {
      particlesPrev[j]->x = particles[j]->x;
      particlesPrev[j]->y = particles[j]->y;
      particlesPrev[j]->m = particles[j]->m;
      particlesPrev[j]->vx = particles[j]->vx;
      particlesPrev[j]->vy = particles[j]->vy;
    }
        
    /* Update particles */
    for (short i = 0; i < N; i++) {
      update((particles[i]), i, G, particlesPrev, N, epsilon, delta_t);
    }
    
    /* Do graphics. */
    if (graphics) {
      ClearScreen();
      for (short i = 0; i < N; i++) {
        DrawCircle(particles[i]->x, particles[i]->y, L, W, radius, circleColor);
      }
      Refresh();
      usleep(10);
    }
  }
    
  /* Close graphics */
  if (graphics) {
    FlushDisplay();
    CloseDisplay();
  }
  
  /* Write file */
  double outdata[N*5];
  for (int i = 0; i < N; i++) {
    outdata[5*i] = particles[i]->x;
    outdata[5*i + 1] = particles[i]->y;
    outdata[5*i + 2] = particles[i]->m;
    outdata[5*i + 3] = particles[i]->vx;
    outdata[5*i + 4] = particles[i]->vy;
  }
  flag = write_doubles_to_file(N*5, outdata, "result.gal");
  printf("Writing output file: flag = %i\n", flag);
  
  /* Free memory */
  for (int i = 0; i < N; i++) {
    free(particles[i]);
    free(particlesPrev[i]);
  } //*/
  
  return 0;
}
