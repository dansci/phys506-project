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

     struct pmtmap pmtmap;
     pmtmap.N = NPHI*NTHETA;
     pmtmap.nphi = NPHI;
     pmtmap.ntheta = NTHETA;


     struct event e1;
     init_random();
     init_pmtmap(&pmtmap, &e1);

     make_event(&e1, NHITS);
     fill_pmt_info(&e1, &pmtmap);
     
     double pos[3] = {0, 0, 0};
     fill_expected_info(pos, &e1, &pmtmap);

     return 0;
}
