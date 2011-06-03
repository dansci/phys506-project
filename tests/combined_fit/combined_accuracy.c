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
          double XTOL = -1; /* to be set by "user" */
     int NHITS = -1;
     
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

     if (XTOL < 0 || NHITS < 0) {
	  fprintf(stderr, 
		  "please specify NHITS and XTOL with -N and -t\n");
	  return 1;
     }
     
     int NPHI = 134;
     int NTHETA = 67;
     struct pmtmap pmtmap;
     pmtmap.N = NPHI*NTHETA;
     pmtmap.nphi = NPHI;
     pmtmap.ntheta = NTHETA;

     struct event e1;
     struct pos_data data;
     data.p = &pmtmap;
     data.e = &e1;

     init_random();
     init_pmtmap(&data);

     make_event(&e1, NHITS);
     fill_pmt_info(&data);
     
     
     nlopt_opt opt;
     opt = nlopt_create(NLOPT_GN_ISRES, 4);
     nlopt_result ret;
     double lb[4] = {-6, -6, -6, e1.hits[0].hit_time-10*scint_time};
     double ub[4] = {6, 6, 6, e1.hits[0].hit_time};
     nlopt_set_lower_bounds(opt, lb);
     nlopt_set_upper_bounds(opt, ub);

     nlopt_set_min_objective(opt, mf, &data);
     double tols[4] = {XTOL, XTOL, XTOL, XTOL/light_speed};
     nlopt_set_xtol_abs(opt, tols);

     ret = nlopt_add_inequality_constraint(opt, radius_check, &data, 1e-10);
     ret = nlopt_add_inequality_constraint(opt, time_check, &data, 1e-15);

     double x[4];
     x[0] = x[1] = x[2] = x[3] = 0;

     double fval = mf(4, x, NULL, &data);

     ret = nlopt_optimize(opt, x, &fval);
     if (ret > 0)
	  printf("%g %g %g\n", x[0] - e1.spawn_pos[0], 
		 x[1] - e1.spawn_pos[1],
		 x[2] - e1.spawn_pos[2]);
     else
	  fprintf(stderr, "optimizing failed with code %d\n", ret);

     return 0;
}
