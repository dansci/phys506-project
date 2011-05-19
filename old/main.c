/* NB these includes are horrible-- sort them out */
#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_multimin.h>
#include "random.h" /* gsl_rng *r */
#include "geometry.h" /* const double radius */
#include "event.h"
#include "reconstruct.h"

const int NHITS = 1000;
const double tolerance = 0.0001;

void print_state(gsl_multimin_fminimizer *m, struct event *e);

int main(int argc, char *argv[])
{
     /* initialize random number resources */
     init_random();

     /* generate event */
     struct event e1;
     make_event(&e1, NHITS);

     /* initialize minimizer */
     const gsl_multimin_fminimizer_type *T =
	  gsl_multimin_fminimizer_nmsimplex2;
     gsl_multimin_fminimizer *m = gsl_multimin_fminimizer_alloc(T, 4);
     gsl_multimin_function mf;
     mf.f = f;
     mf.n = 4; 
     mf.params = &e1;

     /* set initial conditions */
     gsl_vector *x0 = gsl_vector_alloc(4);
     gsl_vector *s = gsl_vector_alloc(4);
     int i;

     for (i=0; i<3; i++) {
	  gsl_vector_set(x0, i, 0.1);
	  gsl_vector_set(s, i, 3);
     }
     if (argc > 1) {
	  gsl_vector_set(x0, 0, atof(argv[1]));
	  gsl_vector_set(x0, 1, atof(argv[2]));
	  gsl_vector_set(x0, 2, atof(argv[3]));
	  gsl_vector_set(s, 0, .2);
	  gsl_vector_set(s, 0, .2);
	  gsl_vector_set(s, 0, .2);
	  gsl_vector_set(x0, 3, 0);
	  gsl_vector_set(s, 3, scint_time/100);
     }
     gsl_vector_set(x0, 3, 0);
     gsl_vector_set(s, 3, scint_time);


     gsl_multimin_fminimizer_set(m, &mf, x0, s);

     /* iterate... */
     int status;
     do {
	  status = gsl_multimin_fminimizer_iterate(m);
	  /* dbg : */
	  print_state(m, &e1);
	  if (status)
	       break;

	  status = gsl_multimin_test_size (
	       gsl_multimin_fminimizer_size(m), tolerance);
     } while (status == GSL_CONTINUE);

     printf("actual location: \n");
     printf("(x0, y0, z0, t0) = (%g, %g, %g, %g)\n",
	    gsl_vector_get(e1.spawn_pos, 0),
	    gsl_vector_get(e1.spawn_pos, 1),
	    gsl_vector_get(e1.spawn_pos, 2),
	    e1.spawn_time);
     
     printf("fitted to:\n");
     printf("(x0, y0, z0, t0) = (%g, %g, %g, %g)\n",
	    gsl_vector_get(m->x, 0),
	    gsl_vector_get(m->x, 1),
	    gsl_vector_get(m->x, 2),
	    gsl_vector_get(m->x, 3));

     printf("function value: %g\n", m->f);
     printf("my function value: %g\n",
	    f(m->x, &e1));
     
     free_random();
     return 0;
}


void print_state(gsl_multimin_fminimizer *m, struct event *e)
{
     int size = m->x->size;

     int i;
     printf("x: (");
     for (i=0; i<size-1; i++)
	  printf("%g, ", gsl_vector_get(m->x, i));
     printf("%g)\n", gsl_vector_get(m->x, i));
	    
     printf("f(x) = %g\n", f(m->x, e));
}
