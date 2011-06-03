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
     int NHITS = 100;
     int NPHI = 134;
     int NTHETA = 67;
     double XTOL=0.1;

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
     opt = nlopt_create(NLOPT_GN_ISRES, 3);
     double lb[3] = {-6, -6, -6};
     double ub[3] = {6, 6, 6};
     nlopt_set_lower_bounds(opt, lb);
     nlopt_set_upper_bounds(opt, ub);

     nlopt_set_min_objective(opt, mf_p, &data);
     double tols[4] = {XTOL, XTOL, XTOL};
     nlopt_set_xtol_abs(opt, tols);

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
