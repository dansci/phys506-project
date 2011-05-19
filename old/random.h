#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <math.h>


const gsl_rng_type * T;
gsl_rng * r;

void init_random()
{
     gsl_rng_env_setup();
     
     T = gsl_rng_default;
     r = gsl_rng_alloc (T);
}

void free_random()
{
     gsl_rng_free(r);
}

void spherical_random_position(gsl_vector *pos, double rmax)
{
     double theta, phi, rad;
      
     phi = 2 * M_PI * gsl_rng_uniform(r);
     theta = acos(2*gsl_rng_uniform(r)-1);

     /* distribute according to r^3 */
     rad = rmax * pow(gsl_rng_uniform(r), .25);

     gsl_vector_set(pos, 0, rad * sin(theta) * cos(phi));
     gsl_vector_set(pos, 1, rad * sin(theta) * sin(phi));
     gsl_vector_set(pos, 2, rad * cos(theta));
}

void spherical_random_direction(gsl_vector *v)
{
     double theta, phi;
      
     phi = 2 * M_PI * gsl_rng_uniform(r);
     theta = acos(2*gsl_rng_uniform(r)-1);

     gsl_vector_set(v, 0, sin(theta) * cos(phi));
     gsl_vector_set(v, 1, sin(theta) * sin(phi));
     gsl_vector_set(v, 2, cos(theta));
}

double sample_scint_time(double t)
{
     return gsl_ran_exponential(r, t);
}
