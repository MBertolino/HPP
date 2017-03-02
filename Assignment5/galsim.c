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
#include <sys/time.h>
#include <pthread.h>
#include "structs.h"


/* Global variables */
int N;
double delta_t;
double theta_max;
double G;
const double epsilon = 0.001;
double* data;
node_t* tree;
int N_threads;
pthread_t* threads;
arg_t** args;


/* Timings */
double get_wall_seconds() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	double seconds = tv.tv_sec + (double)tv.tv_usec/1000000;
	return seconds;
}


/* Insert a new node */
void insert(node_t **node, double origo_x, double origo_y, double width,
						double x, double y, double m, double vx, double vy, int index) {
	
	// If null, this is a leaf node
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
    (*node)->index = index;
		(*node)->nw = NULL;
		(*node)->ne = NULL;
		(*node)->sw = NULL;
		(*node)->se = NULL;
	
	// If this is a leaf node, split the node
	} else if ((*node)->size == 1) {
		
		// Add previous particle
		if ((*node)->x < (*node)->origo_x) {
			if ((*node)->y < (*node)->origo_y) // South West
				insert(&((*node)->sw), origo_x - width/4, origo_y - width/4, width/2,
                (*node)->x, (*node)->y, (*node)->m, (*node)->vx, (*node)->vy,
                (*node)->index);
			else // North West
				insert(&((*node)->nw), origo_x - width/4, origo_y + width/4, width/2,
               (*node)->x, (*node)->y, (*node)->m, (*node)->vx, (*node)->vy,
               (*node)->index);
		} else {
			if ((*node)->y < (*node)->origo_y) // South East
				insert(&((*node)->se), origo_x + width/4, origo_y - width/4, width/2,
               (*node)->x, (*node)->y, (*node)->m, (*node)->vx, (*node)->vy,
               (*node)->index);
			else // North East
				insert(&((*node)->ne), origo_x + width/4, origo_y + width/4, width/2,
               (*node)->x, (*node)->y, (*node)->m, (*node)->vx, (*node)->vy,
               (*node)->index);
		}
		
		// Add new particle
		if (x < (*node)->origo_x) {
			if (y < (*node)->origo_y) // South West
				insert(&((*node)->sw), origo_x - width/4, origo_y - width/4, width/2,
               x, y, m, vx, vy, index);
			else // North West
				insert(&((*node)->nw), origo_x - width/4, origo_y + width/4, width/2,
               x, y, m, vx, vy, index);
		} else {
			if (y < (*node)->origo_y) // South East
				insert(&((*node)->se), origo_x + width/4, origo_y - width/4, width/2,
               x, y, m, vx, vy, index);
			else // North East
				insert(&((*node)->ne), origo_x + width/4, origo_y + width/4, width/2,
               x, y, m, vx, vy, index);
		}
		
		// Update this nodes properties
    (*node)->size += 1;
		(*node)->x = (((*node)->x)*((*node)->m) + x*m)/(((*node)->m) + m);
		(*node)->y = (((*node)->y)*((*node)->m) + y*m)/(((*node)->m) + m);
		(*node)->m += m;
		
	// If this is a cluster node
	} else {
		
		// Add new particle
		if (x < (*node)->origo_x) {
			if (y < (*node)->origo_y) // South West
				insert(&((*node)->sw), origo_x - width/4, origo_y - width/4, width/2,
               x, y, m, vx, vy, index);
			else // North West
				insert(&((*node)->nw), origo_x - width/4, origo_y + width/4, width/2,
               x, y, m, vx, vy, index);
		} else {
			if (y < (*node)->origo_y) // South East
				insert(&((*node)->se), origo_x + width/4, origo_y - width/4, width/2,
               x, y, m, vx, vy, index);
			else // North East
        insert(&((*node)->ne), origo_x + width/4, origo_y + width/4, width/2,
               x, y, m, vx, vy, index);
		}
		
	  // Update this nodes properties
    (*node)->size += 1;
		(*node)->x = (((*node)->x)*((*node)->m) + x*m)/(((*node)->m) + m);
		(*node)->y = (((*node)->y)*((*node)->m) + y*m)/(((*node)->m) + m);
		(*node)->m += m;
	}
} //*/


/* Compute the total force on a particle */
void force_function(node_t *tree, double x, double y, double m,
                    double vx, double vy, double *force_x, double *force_y) {
	if (tree == NULL) return;
  
  // First check if this is a leaf node
  if (tree->size != 1) {
    double theta = (tree->width)/(sqrt((x-tree->x)*(x-tree->x) +
                   (y-tree->y)*(y-tree->y)));
    
    if (theta > theta_max) {
      // Traverse down the tree if the theta condition is not met
      force_function(tree->nw, x, y, m, vx, vy, force_x, force_y);
      force_function(tree->ne, x, y, m, vx, vy, force_x, force_y);
      force_function(tree->sw, x, y, m, vx, vy, force_x, force_y);
      force_function(tree->se, x, y, m, vx, vy, force_x, force_y);
      return;
    }
  }
  
  // Compute the relative vector and the distance
  double rij_x = x - tree->x;
  double rij_y = y - tree->y;
  double dist = sqrt(rij_x*rij_x + rij_y*rij_y) + epsilon;
  
  // Update the forces
  *force_x += (tree->m)/(dist*dist*dist)*rij_x;
  *force_y += (tree->m)/(dist*dist*dist)*rij_y;
} //*/


/* The thread function to comptue the force some particles */
void* thread_func(void *arg) {
	arg_t *thread_arg = (arg_t*)arg;
	int from = thread_arg->from;
	int to = thread_arg->to;
  
  // Loop through the particles
  for (int i = from; i <= to; i++) {
    
    // Compute the force
	  double force_x = 0;
	  double force_y = 0;
	  force_function(tree, data[5*i], data[5*i + 1], data[5*i + 2], data[5*i + 3],
                   data[5*i + 4], &force_x, &force_y);
    
    // Calculate velocities
	  double vx = data[5*i + 3] - G*force_x*delta_t;
	  double vy = data[5*i + 4] - G*force_y*delta_t;
  	
    // Update values
    data[5*i] += delta_t*vx;
    data[5*i + 1] += delta_t*vy;
    data[5*i + 3] = vx;
    data[5*i + 4] = vy;
  }
  
	return NULL;
} //*/


/* Update the array of particles using the tree */
void update_array() {
  // Create the threads
  for (int i = 0; i < N_threads; i++) {
    pthread_create(&threads[i], NULL, thread_func, args[i]);
  }
  
  // Wait for the threads to finish
  for (int i = 0; i < N_threads; i++) {
    pthread_join(threads[i], NULL);
  }
} //*/


/* Print the tree */
void print_tree(node_t *tree, int nSpaces) {
  
  // Null node
  if (tree == NULL) {
    //printf("NULL\n");
    return;
  
  // Leaf node
  } else if (tree->size == 1) {
    printf("Leaf: x = %lf, y = %lf\n", tree->x, tree->y);
  
  // Cluster
  } else {
    printf("Cluster: ox = %lf, oy = %lf\n", tree->origo_x, tree->origo_y);
    if (tree->nw != NULL)
      printf("%*snw: ", nSpaces, " "); print_tree(tree->nw, nSpaces + 2);
    if (tree->ne != NULL)
      printf("%*sne: ", nSpaces, " "); print_tree(tree->ne, nSpaces + 2);
    if (tree->sw != NULL)
      printf("%*ssw: ", nSpaces, " "); print_tree(tree->sw, nSpaces + 2);
    if (tree->se != NULL)
      printf("%*sse: ", nSpaces, " "); print_tree(tree->se, nSpaces + 2);
  }
}


// Recursively freez the treez */
void free_tree(node_t **tree) {
	if(*tree) {
		free_tree(&((*tree)->nw));
		free_tree(&((*tree)->ne));
		free_tree(&((*tree)->sw));
		free_tree(&((*tree)->se));
		free(*tree);
	}
}


// Draw the particles into the window
void do_graphics(node_t *tree, float L, float W, float radius, float circleColor) {
  
  if (tree == NULL) return;
  DrawRectangle(tree->origo_x - (tree->width)/2,
                tree->origo_y - (tree->width)/2,
                W, L, tree->width, tree->width, 0.8);
  
  if (tree->size == 1) {
    //DrawCircle(tree->x, tree->y, L, W, radius, circleColor);
    DrawRectangle(tree->x, tree->y, W, L, 1/600., 1/600., circleColor);
  } else {
    do_graphics(tree->nw, L, W, radius, circleColor);
    do_graphics(tree->ne, L, W, radius, circleColor);
    do_graphics(tree->sw, L, W, radius, circleColor);
    do_graphics(tree->se, L, W, radius, circleColor);
  }
}


/* Main method */
int main(int argc, char *argv[]) {
  
  // Check for correct number of input arguments
  if (argc != 8) {
    printf("Error: Expected exactly 7 input arguments.");
    return -1;
  }
  
  // Input variables
  N = atoi(argv[1]);
  char *filename = argv[2];
  unsigned int nsteps = atoi(argv[3]);
  delta_t = atof(argv[4]);
  theta_max = atof(argv[5]);
  int graphics = atoi(argv[6]);
  N_threads = atoi(argv[7]);
  
  // Gravitational constant
  G = 100/(double)N;
  
  // Allocate thread variables
  threads = (pthread_t*)malloc(N_threads*sizeof(pthread_t));
  args = (arg_t**)malloc(N_threads*sizeof(arg_t*));
  
  // Set thread indicies
  for (int i = 0; i < N_threads; i++) {
    args[i] = (arg_t*)malloc(sizeof(arg_t));
    (args[i])->from = i*N/N_threads;
    (args[i])->to = (i+1)*N/N_threads - 1;
  }
  
  // Read file
  data = (double*)malloc(N*5*sizeof(double));
  read_doubles_from_file(N*5, data, filename);
  
  // Build the first tree
  tree = NULL;
  
  // Setup graphics
  const int windowWidth = 600;
  const float L = 1;
  const float W = 1;
  const float radius = 0.002*L;
  const float circleColor = 0;
  if (graphics) {
    InitializeGraphics(argv[5], windowWidth, windowWidth);
    SetCAxes(0,1);
  }
  
  /* Loop over time */
  for (int k = 0; k < nsteps; k++) {
    
    // Build tree
    for (int i = 0; i < N; i++) {
      insert(&tree, 0.5, 0.5, 100, data[5*i], data[5*i + 1],
             data[5*i + 2], data[5*i + 3], data[5*i + 4], i);
    }
    
    // Update particles (using force_function)
    update_array();
    
    // Do graphics
    if (graphics) {
      ClearScreen();
      do_graphics(tree, L, W, radius, circleColor);
      Refresh();
      usleep(10);
    }
    
    // Free the tree
    free_tree(&tree);
    tree = NULL;
  } //*/
  
  // Close graphics
  if (graphics) {
    FlushDisplay();
    CloseDisplay();
  }
   
  // Write result file
  write_doubles_to_file(N*5, data, "result.gal");
  
  // Free memory
	free(data);
  free(threads);
  for (int i = 0; i < N_threads; i++) {
    free(args[i]);
  }
  free(args);
  
  return 0;
}

