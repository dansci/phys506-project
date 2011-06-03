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

     /* geometry like SNO+'s (as close to 9,000 PMTS as possible) */
     int NTHETA = 67;
     int NPHI = 134;
     struct pmtmap pmtmap;
     pmtmap.N = NPHI*NTHETA;
     pmtmap.nphi = NPHI;
     pmtmap.ntheta = NTHETA;


     struct event e1;
     init_random();
     init_pmtmap(&pmtmap, &e1);

     make_event(&e1, NHITS);
     fill_pmt_info(&e1, &pmtmap);
     
     nlopt_opt opt;
     opt = nlopt_create(NLOPT_GN_ISRES, 3);
     double lb[3] = {-6, -6, -6};
     double ub[3] = {6, 6, 6};
     nlopt_set_lower_bounds(opt, lb);
     nlopt_set_upper_bounds(opt, ub);

     struct pos_data data;
     data.p = &pmtmap;
     data.e = &e1;

     nlopt_set_min_objective(opt, mf_p, &data);
     double tols[3] = {XTOL, XTOL, XTOL};
     nlopt_set_xtol_abs(opt, tols);
     nlopt_set_maxtime(opt, 10.0); /* unstick this */
     nlopt_set_maxeval(opt, 4e5); /* if timing doesn't unstick it*/

     nlopt_add_inequality_constraint(opt, radius_check, &data, 1e-10);

     double x[3] = {0, 0, 0};
     double fval = mf_p(3, x, NULL, &data);

     nlopt_result ret;
     ret = nlopt_optimize(opt, x, &fval);

     if (ret > 0)
	  printf("%g %g %g\n", x[0] - e1.spawn_pos[0], 
		 x[1] - e1.spawn_pos[1],
		 x[2] - e1.spawn_pos[2]);
     else
	  fprintf(stderr, "optimizing failed with code %d\n", ret);

     return 0;
}
