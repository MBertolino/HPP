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


/* Define the quad_node struct */
typedef struct quad_node {
	struct quad_node *nw, *ne, *sw, *se;
	double origo_x, origo_y;
	double width;
	double x, y;
	double m;
	double vx, vy;
	int size, index;
} node_t;


/* Timings */
double get_wall_seconds(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	double seconds = tv.tv_sec + (double)tv.tv_usec/1000000;
	return seconds;
}

/* Insert a new node */
void insert(node_t **node, double origo_x, double origo_y, double width,
						double x, double y, double m, double vx, double vy, int index) {
	
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
    (*node)->index = index;
		(*node)->nw = NULL;
		(*node)->ne = NULL;
		(*node)->sw = NULL;
		(*node)->se = NULL;
	
	/* If this is a leaf node, split the node */
	} else if ((*node)->size == 1) {
		
		/* Add previous particle */
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
		
		/* Add new particle */
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
		
		/* Update this nodes properties */
    (*node)->size += 1;
		(*node)->x = (((*node)->x)*((*node)->m) + x*m)/(((*node)->m) + m);
		(*node)->y = (((*node)->y)*((*node)->m) + y*m)/(((*node)->m) + m);
		(*node)->m += m;
		//(*node)->vx = 0;
		//(*node)->vy = 0;
		
	/* If this is a cluster node */
	} else {
		
		/* Add new particle */
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
		
		/* Update this nodes properties */
    (*node)->size += 1;
		(*node)->x = (((*node)->x)*((*node)->m) + x*m)/(((*node)->m) + m);
		(*node)->y = (((*node)->y)*((*node)->m) + y*m)/(((*node)->m) + m);
		(*node)->m += m;
	}
} //*/


/*  */
void force_function(node_t *root, node_t *tree, double x, double y, double m, double vx, double vy, 
										double *force_x, double *force_y, double theta_max,
										double G, const double epsilon, double delta_t) {
	if (tree == NULL) return;
  
  /* First check if this is a leaf node */
  if (tree->size != 1) {
    double theta = (tree->width)/(sqrt((x-tree->x)*(x-tree->x) +
                   (y-tree->y)*(y-tree->y)));
    
    if (theta > theta_max) {
      /* Traverse down the tree if the theta condition is not met */
      force_function(root, tree->nw, x, y, m, vx, vy, force_x, force_y,
                     theta_max, G, epsilon, delta_t);
      force_function(root, tree->ne, x, y, m, vx, vy, force_x, force_y,
                     theta_max, G, epsilon, delta_t);
      force_function(root, tree->sw, x, y, m, vx, vy, force_x, force_y,
                     theta_max, G, epsilon, delta_t);
      force_function(root, tree->se, x, y, m, vx, vy, force_x, force_y,
                     theta_max, G, epsilon, delta_t);
      return;
    }
  }
  
  /* Compute the relative vector and the distance */
  double rij_x = x - tree->x;
  double rij_y = y - tree->y;
  double dist = sqrt(rij_x*rij_x + rij_y*rij_y) + epsilon;
  
  /* Update the forces */
  *force_x += (tree->m)/(dist*dist*dist)*rij_x;
  *force_y += (tree->m)/(dist*dist*dist)*rij_y;
} //*/


/* Copy leaf node particle attributes, update the copied attributes and build a new */
void update_tree(node_t *root, node_t *tree, node_t **new_tree,
                 double theta_max, double G, const double epsilon, double delta_t) {
  if (tree == NULL) return;
  
  if (tree->size != 1) {
			update_tree(root, tree->nw, new_tree, theta_max, G, epsilon, delta_t);
			update_tree(root, tree->ne, new_tree, theta_max, G, epsilon, delta_t);
			update_tree(root, tree->sw, new_tree, theta_max, G, epsilon, delta_t);
			update_tree(root, tree->se, new_tree, theta_max, G, epsilon, delta_t);
  } else {
		double force_x = 0;
		double force_y = 0;
  	force_function(root, root, tree->x, tree->y, tree->m, tree->vx, tree->vy,
                   &force_x, &force_y, theta_max, G, epsilon, delta_t);
  	
  	/* Calculate velocities */
		double vx = tree->vx - G*force_x*delta_t;
		double vy = tree->vy - G*force_y*delta_t;
     
    /* Build the new tree */	
  	insert(new_tree, 0.5, 0.5, root->width, tree->x + delta_t*vx,
           tree->y + delta_t*vy, tree->m, vx, vy, tree->index);
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


/* Recursively write the tree into a file */
void write_tree(node_t *tree, double **data) {
  if (tree == NULL) {
    return;
  } else if (tree->size == 1) {
    (*data)[5*tree->index] = tree->x;
    (*data)[5*tree->index + 1] = tree->y;
    (*data)[5*tree->index + 2] = tree->m;
    (*data)[5*tree->index + 3] = tree->vx;
    (*data)[5*tree->index + 4] = tree->vy;
  } else {
    write_tree(tree->nw, data);
    write_tree(tree->ne, data);
    write_tree(tree->sw, data);
    write_tree(tree->se, data);
  }
}


/* Recursively freez the treez */
void free_tree(node_t **tree) {
	if(*tree) {
		free_tree(&((*tree)->nw));
		free_tree(&((*tree)->ne));
		free_tree(&((*tree)->sw));
		free_tree(&((*tree)->se));
		free(*tree);
	}
}


/* Draw the particles into the window */
void do_graphics(node_t *tree, float L, float W, float radius, float circleColor) {
  if (tree == NULL) return;
  DrawRectangle(tree->origo_x - (tree->width)/2,
                tree->origo_y - (tree->width)/2,
                W, L, tree->width, tree->width, circleColor + 0.8);
  
  if (tree->size == 1) {
    DrawCircle(tree->x, tree->y, L, W, radius, circleColor);
  } else {
    do_graphics(tree->nw, L, W, radius, circleColor);
    do_graphics(tree->ne, L, W, radius, circleColor);
    do_graphics(tree->sw, L, W, radius, circleColor);
    do_graphics(tree->se, L, W, radius, circleColor);
  }
}


/* Main method */
int main(int argc, char *argv[]) {
  printf("Size of struct: %lu\n", (long unsigned int)sizeof(struct quad_node));
  
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
  double theta_max = atof(argv[5]);
  int graphics = atoi(argv[6]);

  
  /* Constants */
  double G = 100/(double)N;
  const double epsilon = 0.001;
  
  /* Read file */
  double *data = (double*)malloc(N*5*sizeof(double));
  read_doubles_from_file(N*5, data, filename);
  
  /* Allocate memory for trees and build the first tree */
  node_t *tree = NULL;
  node_t *new_tree = NULL;
  node_t *temp = (node_t*)malloc(sizeof(node_t));
  for (int i = 0; i < N; i++) {
    insert(&tree, 0.5, 0.5, 100, data[5*i], data[5*i + 1],
           data[5*i + 2], data[5*i + 3], data[5*i + 4], i);
  }
  
  /* Print the initial tree *
  printf("Initial tree:\n");
  print_tree(tree, 0); //*/
  
  /* Setup graphics */
  const int windowWidth = 800;
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
    /* Build the new tree by computing the new positions and
    velocities */
    update_tree(tree, tree, &new_tree, theta_max, G, epsilon, delta_t);
        
    /* Clear the old tree */
    free_tree(&tree);
    tree = NULL;
    
    /* Copy the new tree into the old tree */
    temp = new_tree;
    new_tree = tree;
    tree = temp;
    
    /* Do graphics */
    if (graphics) {
      ClearScreen();
      do_graphics(tree, L, W, radius, circleColor);
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
  write_tree(tree, &data);
  write_doubles_to_file(N*5, data, "result.gal");
  
  /* Free memory */
	free_tree(&tree);
	free(data);
  
  return 0;
}

