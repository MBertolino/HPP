#include "structures.h"
#include "functions.h"
#include <math.h>
#include <stdlib.h>

#ifndef NULL
#define NULL 0;
#endif

void update(particle_t **particle, short i, const double G,
            particle_t **particles, short N, const double epsilon,
            double delta_t) {
  
	/* Old particle */
	double xi = (*particle)->x;
	double yi = (*particle)->y;
	double vxi = (*particle)->vx;
	double vyi = (*particle)->vy;
	
	double force_x, force_y;
	double acc_x, acc_y;
 	double rij_x, rij_y, rij, dist;
 	double mass_j;

	for (short j = 1; j <= N; j++) {
		if (j == i) continue;
		rij_x = xi - particles[j]->x;
		rij_y = yi - particles[j]->y;
		rij = sqrt(rij_x*rij_x + rij_y*rij_y);
		dist = rij + epsilon;
		
		/* Calculate forces */
		mass_j = particles[j]->m;
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
	(*particle)->vx = vxi;
	(*particle)->vy = vyi;
		
	/* Update positions */
	(*particle)->x = xi + delta_t*vxi;
	(*particle)->y = yi + delta_t*vyi;
}

