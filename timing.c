/* NB these includes are horrible-- sort them out */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <nlopt.h>
#include "random.h"
#include "geometry.h"
#include "event.h"
#include "reconstruct.h"

int main(int argc, char *argv[])
{
     /* default values */
     int NHITS = 500;
     double XTOL = .1;

     int c;
     while ((c = getopt(argc, argv, "N:t:")) != -1)
	  switch (c) {
	  case 'N':
	       NHITS = atoi(optarg);
	       break;
	  case 't':
	       XTOL = atof(optarg);
	       break;
	  case '?':
	       if (optopt == 'N' || optopt == 't')
		    fprintf(stderr, "Option -%c requires argument.\n", optopt);
	       else
		    fprintf(stderr, "unknown option -%c\n", optopt);
	       return 1;
	  default:
	       ;
	  }

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

     struct pos_data data;
     data.p = NULL; /* no pmtmap required for timing fit */
     data.e = &e1;

     nlopt_set_min_objective(opt, mf_t, &data);
//     nlopt_set_maxtime(opt, .5);
     double tols[4] = {XTOL, XTOL, XTOL, XTOL/light_speed};
     nlopt_set_xtol_abs(opt, tols);

     ret = nlopt_add_inequality_constraint(opt, radius_check, &data, 1e-10);
     ret = nlopt_add_inequality_constraint(opt, time_check, &data, 1e-15);

     double x[4];
     x[0] = x[1] = x[2] = x[3] = 0;

     double fval = mf_t(4, x, NULL, &data);
     
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
	    mf_t(4, e1.spawn_pos, NULL, &data));

     free_random();
     return 0;
}
