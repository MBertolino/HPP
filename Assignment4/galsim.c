/**
 * Simulation of the gravitational N-body problem.
 *
 * Course: High Performance Programming, Uppsala University
 *
 * Authors: Adam Lindell <lindell.adam@yahoo.com>
 *          Arvi Jonnarth <arvi.jj@gmail.com>
 *			Mattias Bertolino <mattias.bertolino@gmail.com>
 */

#define _BSD_SOURCE

#include "file_operations/file_operations.h"
#include "graphics/graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h> // Sleep function

int main(int argc, char *argv[]) {
  
  /* Check for correct number of input arguments */
  if (argc != 7) {
    printf("Error: Expected exactly 6 input arguments.");
    return -1;
  }
    
  /* Input variables */
  int N = atoi(argv[1]);
  char *filename = argv[2];
  unsigned int nsteps = atoi(argv[3]);
  double delta_t = atof(argv[4]);
  int graphics = atoi(argv[5]);
  double theta = atof(argv[6]);
  
  /* Constants */
  double G = 100/(double)N;
  double epsilon = 0.001;
  
  /* Define temporary variables *
  double vxi, vyi;
  double rij_x, rij_y, dist;
  double mass_i, mass_j; //*/
  
  /* Read file */
  double *data = (double*)malloc(N*5*sizeof(double));
  read_doubles_from_file(N*5, data, filename);
  
  /* Build the first tree *
  ...
  ...
  ... //*/
  
  /* Setup graphics */
  int windowWidth = 800;
  float L = 1;
  float W = 1;
  float radius = 0.002*L;
  float circleColor = 0;
  if (graphics) {
    InitializeGraphics(argv[5], windowWidth, windowWidth);
    SetCAxes(0,1);
  }
  
  /* Loop over time */
  for (int k = 0; k < nsteps; k++) {
    
    /* Build the new tree by computing the new positions and velocities *
    ...
    ...
    ... //*/
    
    /* Clear the old tree *
    ...
    ...
    ... //*/
    
    /* Copy the new tree into the old tree */
    *temp = *new_tree;
    *new_tree = *tree;
    *tree = *temp;
    //*/
    
    /* Do graphics */
    if (graphics) {
      ClearScreen();
      //for (int i = 0; i < N; i++) {
      //  DrawCircle(data[5*i], data[5*i + 1], L, W, radius, circleColor);
      //}
      Refresh();
      usleep(10);
    }
  }
    
  /* Close graphics */
  if (graphics) {
    FlushDisplay();
    CloseDisplay();
  }
    
  /* Write result file *
  ...
  ...
  ... //*/
  write_doubles_to_file(N*5, data, "result.gal");
  
  /* Free memory *
  ...
  ...
  ... //*/
  
  return 0;
}

