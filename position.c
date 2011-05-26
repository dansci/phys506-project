#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <nlopt.h>
#include "random.h"
#include "geometry.h"
#include "event.h"
#include "reconstruct.h"

/* Steps:

   - divide the geometry into chunks, and make a struct of these,
   holding position and maybe other stuff.

   - when generating events, have a quick way to check which chunk the
   hit belongs to.  or maybe just a hitcount for each chunk?

   - do a binned likelihood thing.  for every chunk, calculate the
   expected number of hits.  as a first approximation have this be
   proportional to \cos(\theta) r^{-2}.  calculate \cos(\theta) by
   (given panel position x_d, reconstructed position x_e):

   detector normal: n_d = origin - x_d
   photon direction: n_p = x_d - x_p

   then \cos\theta = n_d.n_p / (|n_d||n_p|)

   - maximize that jank.

*/

 
int main(int argc, char *argv[])
{
     int NHITS = 500;
     int NPHI = 10;
     int NTHETA = 5;
     double XTOL=0.5;

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

     if (argc > 1 ) {
	  double x[3] = {atof(argv[1]), atof(argv[2]), atof(argv[3])};
	  printf("mf_p(%g, %g, %g) = %g\n",
		 x[0], x[1], x[2], mf_p(3, x, NULL, &data));
	  printf("mf_P(%g, %g, %g) = %g\n",
		 e1.spawn_pos[0], e1.spawn_pos[1], e1.spawn_pos[2],
		 mf_p(3, e1.spawn_pos, NULL, &data));
	  return 0;
     }
     
     nlopt_set_min_objective(opt, mf_p, &data);
     double tols[4] = {XTOL, XTOL, XTOL};
     nlopt_set_xtol_abs(opt, tols);
     nlopt_set_maxtime(opt, 600.0);

     nlopt_add_inequality_constraint(opt, radius_check, &data, 1e-10);

     double x[3] = {0, 0, 0};
     double fval = mf_p(3, x, NULL, &data);

     nlopt_result ret;
     ret = nlopt_optimize(opt, x, &fval);
     printf("returned %d\n", ret);
     printf("actual: (%g, %g, %g)\n", e1.spawn_pos[0], 
	    e1.spawn_pos[1], e1.spawn_pos[2]);
     printf("fitted: (%g, %g, %g)\n", x[0], x[1], x[2]);
     return 0;
}
