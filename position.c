#include <stdio.h>

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
   
