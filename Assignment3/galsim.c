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
  int N = atoi(argv[1]);
  char *filename = argv[2];
  int nsteps = atoi(argv[3]);
  double delta_t = atof(argv[4]);
  const int graphics = atoi(argv[5]);
  
  /* Constants */
  const double G = 100/(double)N;
  const double epsilon = 0.001;
  
  /* Define temporary variables */
  double vxi, vyi;
 	double rij_x, rij_y, dist;
 	double mass_i, mass_j;
  
  /* Create an array for the previous data */
  double *dataPrev = (double*)malloc(N*5*sizeof(double));
  
  /* Force vectors */
  double *force_x = (double*)malloc(N*sizeof(double));
  double *force_y = (double*)malloc(N*sizeof(double));
  
  /* Read file */
  char fileDest[50];
  strcpy(fileDest, "input_data/");
  strcat(fileDest, filename);
  double *data = (double*)malloc(N*5*sizeof(double));
  read_doubles_from_file(N*5, data, fileDest);
  
  /* Setup graphics */
  if (graphics) {
    InitializeGraphics(argv[5], windowWidth, windowWidth);
    SetCAxes(0,1);
  }
  float L = 1;
  float W = 1;
  float radius = 0.002*L;
  float circleColor = 0;
  
  /* Loop over time */
  for (int k = 0; k < nsteps; k++) {
    
    /* Update previous data */
    for (int i = 0; i < 5*N; i++) {
      dataPrev[i] = data[i];
    }
    
    /* Reset the force vectors */
    for (int i = 0; i < N; i++) {
      force_x[i] = 0;
      force_y[i] = 0;
    }
    
    /* Compute the forces */
    for (int i = 0; i < N; i++) {
      for (int j = i+1; j < N; j++) {
        
        /* Compute the relative vector and the distance */
        rij_x = dataPrev[5*i] - dataPrev[5*j];
        rij_y = dataPrev[5*i + 1] - dataPrev[5*j + 1];
        dist = sqrt(rij_x*rij_x + rij_y*rij_y) + epsilon;
        
        /* Update the forces */
        mass_i = dataPrev[5*i + 2];
        mass_j = dataPrev[5*j + 2];
        force_x[i] += mass_j/(dist*dist*dist)*rij_x;
        force_x[j] += -mass_i/(dist*dist*dist)*rij_x;
        force_y[i] += mass_j/(dist*dist*dist)*rij_y;
        force_y[j] += -mass_i/(dist*dist*dist)*rij_y;
      }
    }
    
    /* Update positions and velocities */
    for (int i = 0; i < N; i++) {
      
      /* Calculate velocities */
      vxi = dataPrev[5*i + 3] - G*force_x[i]*delta_t;
      vyi = dataPrev[5*i + 4] - G*force_y[i]*delta_t;
      
      /* Update velocities */	
      data[5*i + 3] = vxi;
      data[5*i + 4] = vyi;
      
      /* Update positions */
      data[5*i] = dataPrev[5*i] + delta_t*vxi;
      data[5*i + 1] = dataPrev[5*i + 1] + delta_t*vyi;
    }
    
    /* Do graphics */
    if (graphics) {
      ClearScreen();
      for (int i = 0; i < N; i++) {
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
  free(force_x);
  free(force_y);
  
  return 0;
}

