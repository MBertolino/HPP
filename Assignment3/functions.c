#include "functions.h"
#include <math.h>
#include <stdlib.h>

#ifndef NULL
#define NULL 0;
#endif

double* update(double xi, double yi, double mi, double vxi, double vyi, double *dataPrev,
            short i, short N, const double G, const double epsilon,
            double delta_t) {
  
	/* Old particle */
	double force_x, force_y;
	double acc_x, acc_y;
 	double rij_x, rij_y, rij, dist;
 	double mass_j;
 	static double data[5];

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
	data[3] = vxi;
	data[4] = vyi;
	
	/* Update positions */
	data[0] = xi + delta_t*vxi;
	data[1] = yi + delta_t*vyi;
	data[2] = mi;
	
	return data;
}
