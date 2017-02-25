/* Define the particle struct *
typedef struct particle {
  double x;
  double y;
  double m;
  double vv;
  double vy;
} particle_t; //*/

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
