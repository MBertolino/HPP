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

struct quad_node {
	struct quad_node *nw, *ne, *sw, *se;
	double origo_x, origo_y;
	double width;
	double x, y;
	double m;
	double vx, vy;
	int size;
} node_t;

/* Insert a new node */
void insert(node_t **node, double origo_x, double origo_y, double width,
						double x, double y, double m, double vx, double vy) {
	
	/* If null, this is a leaf node */
	if ((*node) == NULL) {
		*node = (node_t*)malloc(sizeof(node_t));
		(*node)->origo_x = origo_x;
		(*node)->origo_y = origo_y;
		(*node)->width = width;
		(*node)->x = x;
		(*node)->y = y;
		(*node)->m = m;
		(*node)->vx = vx;
		(*node)->vy = vy;
		(*node)->size = 1;
		(*node)->nw = NULL;
		(*node)->ne = NULL;
		(*node)->sw = NULL;
		(*node)->se = NULL;
	
	/* If this is a leaf node, split the node */
	} else if (size == 1) {
		(*node)->size += 1;
		
		/* Add previous particle */
		if ((*node)->x < (*node)->origo_x) {
			if ((*node)->y < (*node)->origo_y) // South West
				insert((&(*node)->sw), (*node)->x, (*node)->y,
							 (*node)->m, (*node)->vx, (*node)->vy, width/2);
			else // North West
				insert((&(*node)->nw), (*node)->x, (*node)->y,
							 (*node)->m, (*node)->vx, (*node)->vy, width/2);
		} else {
			if ((*node)->y < (*node)->origo_y) // South East
				insert((&(*node)->se), (*node)->x, (*node)->y,
							 (*node)->m, (*node)->vx, (*node)->vy, width/2);
			else // North East
				insert((&(*node)->ne), (*node)->x, (*node)->y,
							 (*node)->m, (*node)->vx, (*node)->vy, width/2);
		}
		
		/* Add new particle */
		if (x < (*node)->origo_x) {
			if (y < (*node)->origo_y) // South West
				insert((&(*node)->sw), x, y, m, vx, vy, width/2);
			else // North West
				insert((&(*node)->nw), x, y, m, vx, vy, width/2);
		} else {
			if (y < (*node)->origo_y) // South East
				insert((&(*node)->se), x, y, m, vx, vy, width/2);
			else // North East
				insert((&(*node)->ne), x, y, m, vx, vy, width/2);
		}
		
		/* Update this nodes properties */
		(*node)->x = ((*node)->x + x)/2;
		(*node)->y = ((*node)->y + y)/2;
		(*node)->m += m;
		//(*node)->vx = 0;
		//(*node)->vy = 0;
		
	/* If this is a cluster node */
	} else {
		
		/* Add new particle */
		if (x < (*node)->origo_x) {
			if (y < (*node)->origo_y) // South West
				insert((&(*node)->sw), x, y, m, vx, vy, width/2);
			else // North West
				insert((&(*node)->nw), x, y, m, vx, vy, width/2);
		} else {
			if (y < (*node)->origo_y) // South East
				insert((&(*node)->se), x, y, m, vx, vy, width/2);
			else // North East&(*node)->ne), x, y, m, vx, vy, width/2);
		}
		
		/* Update this nodes properties */
		(*node)->size += 1;
		(*node)->x = (((*node)->x)*size + x)/(size + 1);
		(*node)->y = (((*node)->y)*size + y)/(size + 1);
		(*node)->m += m;
	}
} //*/

int main(int argc, char *argv[]) {
  printf("Size of struct: %lu\n", sizeof(struct quad_node));
  
  /* Check for correct number of input arguments */
  if (argc != 6) {
    printf("Error: Expected exactly 5 input arguments.");
    return -1;
  }
    
  /* Input variables */
  int N = atoi(argv[1]);
  char *filename = argv[2];
  unsigned int nsteps = atoi(argv[3]);
  double delta_t = atof(argv[4]);
  int graphics = atoi(argv[5]);
  
  /* Constants */
  double G = 100/(double)N;
  double epsilon = 0.001;
  
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
  double *data = (double*)malloc(N*5*sizeof(double));
  read_doubles_from_file(N*5, data, filename);
  
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

