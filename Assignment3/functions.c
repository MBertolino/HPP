#include "functions.h"
#include "strucures.h"

particle_t update(short i, const double G, particle_t** particles, short N, const double epsilon, double delta_t) {
	
	/* Old particle */
	particle_t *particle_i = particles[i];
	xi = particle_i->x;
	yi = particle_i->y;
	mi = particel_i->m;
	vxi = particle_i->vx;
	vyi = particle_i->vy;
	
	/* New particle */
	particle_t *particle_i_new;
	
	/* Other particle*/
	particle_t *partj;
	
	double force_x, force_y;
	double forceFactor = -G*mi;
	double acc_x, acc_y;
	double vxi_new, xyi_new;
	double xi_new, yi_new;
	
	for (short j = 0; j < N; j++) {
		if (j == i) continue;
		
		partj = particles[j];
		rij_x = xi - partj->x;
		rij_y = yi - partj->y;
		rij = sqrt(rij_x*rij_x + rij_y*rij_y);
		dist = rij + epsilon;
		
		/* Calculate forces */
		mass_j = partj->m;
		force_x += (mass_j)/(dist*dist*dist)*rij_x;
		force_y += (mass_j)/(dist*dist*dist)*rij_y;
	}

	/* Calculate accelerations */
	acc_x = forceFactor*force_x/mi;
	acc_y = forceFactor*force_y/mi;
	
	/* Update velocities */	
	vxi_new = vxi + delta_t*acc_x;
	vyi_new = vyi + delta_t*acc_y;
		
	/* Update positions*/
	particle_i_new->x = xi + delta_t*vxi_new;
	particle_i_new->y = yi + delta_t*vyi_new;		
	particle_i_new->m = mi;
	particle_i_new->vx = vxi_new;
	particle_i_new->vy = vyi_new;	
	
	return particle_i_new;
}
