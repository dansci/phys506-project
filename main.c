/* NB these includes are horrible-- sort them out */
#include <stdio.h>
#include <stdlib.h>
#include <nlopt.h>
#include "random.h" /* gsl_rng *r */
#include "geometry.h" /* const double radius */
#include "event.h"
#include "reconstruct.h"

const int NHITS = 500;
const double tolerance = 1e-8;

int main(int argc, char *argv[])
{
     /* initialize random number resources */
     init_random();

     /* generate event */
     struct event e1;
     make_event(&e1, NHITS);
     sort_event(&e1);

     nlopt_opt opt;
     opt = nlopt_create(NLOPT_GN_ISRES, 4);
     nlopt_result ret;
     double lb[4] = {-6, -6, -6, -10*scint_time};
     double ub[4] = {6, 6, 6, e1.hits[0].hit_time};
     nlopt_set_lower_bounds(opt, lb);
     nlopt_set_upper_bounds(opt, ub);

     nlopt_set_min_objective(opt, mf, &e1);
//     nlopt_set_maxtime(opt, .5);
     double tols[4] = {.1, .1, .1, 1e-9};
     nlopt_set_xtol_abs(opt, tols);

     ret = nlopt_add_inequality_constraint(opt, radius_check, &e1, 1e-10);
     ret = nlopt_add_inequality_constraint(opt, time_check, &e1, 1e-20);

     double x[4];
     if (argc > 1) {
	  x[0] = atof(argv[1]);
	  x[1] = atof(argv[2]);
	  x[2] = atof(argv[3]);
     } else
	  x[0] = x[1] = x[2] = 0;
     
     x[3] = 0;
     double fval = mf(4, x, NULL, &e1);
     
     ret = nlopt_optimize(opt, x, &fval);
     
     printf("actual location: \n");
     printf("(x0, y0, z0, t0) = (%g, %g, %g, %g)\n",
	    e1.spawn_pos[0], e1.spawn_pos[1],
	    e1.spawn_pos[2], e1.spawn_time);
     
     printf("fitted to:\n");
     printf("(x0, y0, z0, t0) = (%g, %g, %g, %g)\n",
	    x[0], x[1], x[2], x[3]);
     printf("with log-likelihood %g\n", fval);
     printf("and return value %d\n", ret);

     printf("log-likelihood at actual value is %g\n", 
	    mf(4, e1.spawn_pos, NULL, &e1));

     free_random();
     return 0;
}
