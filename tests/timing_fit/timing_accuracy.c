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
     struct pos_data data;
     data.p = NULL;
     data.e = &e1;
     make_event(&e1, NHITS);
     sort_event(&e1);

     nlopt_opt opt;
     opt = nlopt_create(NLOPT_GN_ISRES, 4);
     nlopt_result ret;
     double lb[4] = {-6, -6, -6, -10*scint_time};
     double ub[4] = {6, 6, 6, e1.hits[0].hit_time};
     nlopt_set_lower_bounds(opt, lb);
     nlopt_set_upper_bounds(opt, ub);

     nlopt_set_min_objective(opt, mf_t, &data);
     double tols[4] = {XTOL, XTOL, XTOL, XTOL/light_speed};
     nlopt_set_xtol_abs(opt, tols);
     nlopt_set_maxtime(opt, 20.0); /* unstick this */
     nlopt_set_maxeval(opt, 4e5); /* if timing doesn't unstick it*/

     ret = nlopt_add_inequality_constraint(opt, radius_check, &data, 1e-10);
     ret = nlopt_add_inequality_constraint(opt, time_check, &data, 1e-20);

     double x[4];
     x[0] = x[1] = x[2] = x[3] = 0;

     double fval = mf_t(4, x, NULL, &data);
     
     ret = nlopt_optimize(opt, x, &fval);

     if (ret > 0)
	  printf("%g %g %g %g\n", x[0] - e1.spawn_pos[0], 
		 x[1] - e1.spawn_pos[1],
		 x[2] - e1.spawn_pos[2],
		 x[3] - e1.spawn_time);
     else
	  fprintf(stderr, "optimizing failed with code %d\n", ret);
     
     free_random();
     return 0;
}
