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

void update_tree(node_t *tree, node_t *new_tree) {
	

}

void force_function(node_t *tree, double x, double y, double m, double vx, double vy){


}

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

