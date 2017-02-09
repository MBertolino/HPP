#define _BSD_SOURCE

#include "file_operations/file_operations.h"
#include "graphics/graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
  float radius = 0.002*L;
  float circleColor = 0;
  
  /* Constants */
  const double G = 100/(double)N;
  const double epsilon = 0.001;
  
  /* Read file */
  char fileDest[50];
  strcpy(fileDest, "input_data/");
  strcat(fileDest, filename);
  double *data = (double*)malloc(N*5*sizeof(double));
  read_doubles_from_file(N*5, data, fileDest);
  
  /* Create an array for the previous data */
  double *dataPrev = (double*)malloc(N*5*sizeof(double));
  
  /* Define temporary variables */
  double xi, yi, vxi, vyi;
  double force_x, force_y;
	double acc_x, acc_y;
 	double rij_x, rij_y, rij, dist;
 	double mass_j;
  
  /* Loop over time */
  for (short k = 0; k < nsteps; k++) {
    
    /* Update previous data */
    for (short i = 0; i < N*5; i++) {
      dataPrev[i] = data[i];
    }
    
    /* Update particles */
    for (short i = 0; i < N; i++) {
      
      /* Position and velocity */
      xi = dataPrev[5*i];
      yi = dataPrev[5*i + 1];
      vxi = dataPrev[5*i + 3];
      vyi = dataPrev[5*i + 4];
      
      /* Compute each particle contribution */
      force_x = 0;
      force_y = 0;
      for (short j = 0; j < N; j++) {
        if (j == i) continue;
        
        rij_x = xi - dataPrev[5*j];
        rij_y = yi - dataPrev[5*j + 1];
        rij = sqrt(rij_x*rij_x + rij_y*rij_y);
        dist = rij + epsilon;
        
        /* Calculate forces */
        mass_j = dataPrev[5*j + 2];
        force_x += (mass_j)/(dist*dist*dist)*rij_x;
        force_y += (mass_j)/(dist*dist*dist)*rij_y;
      }
      
      /* Calculate accelerations */
      acc_x = -G*force_x;
      acc_y = -G*force_y;
      
      /* Calculate velocities */	
      vxi += delta_t*acc_x;
      vyi += delta_t*acc_y;
      
      /* Update velocities */	
      data[5*i + 3] = vxi;
      data[5*i + 4] = vyi;
      
      /* Update positions and mass */
      data[5*i] = xi + delta_t*vxi;
      data[5*i + 1] = yi + delta_t*vyi;
    }
    
    /* Do graphics. */
    if (graphics) {
      ClearScreen();
      for (short i = 0; i < N; i++) {
        DrawCircle(data[5*i], data[5*i + 1], L, W, radius, circleColor);
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
    
  /* Write result file */
  write_doubles_to_file(N*5, data, "result.gal");
  
  /* Free memory */
  free(data);
  free(dataPrev);
  
  return 0;
}

