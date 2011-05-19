/* this penalty function method doesn't seem to work very well.
 * Granted, making the width small does mean the solution will stay
 * within the boundary, but I'm getting (1.7632, ~10^-18) for a
 * starting point of (50, 50) */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <gsl/gsl_multimin.h>

double min_f(const gsl_vector *v, void *p);
void min_df(const gsl_vector *v, void *p, gsl_vector *g);
void min_fdf(const gsl_vector *v, void *p, double *f, gsl_vector *g);
double f(double x, double y);
double dfdx(double x, double y);
double dfdy(double x, double y);
double d2fdx2(const gsl_vector *v, void *p);
double d2fdy2(const gsl_vector *v, void *p);
double d2fdxdy(const gsl_vector *v, void *p);

const double tolerance = 0.1;
const double step = .1;

int main(int argc, char *argv[])
{
     /* initialize minimizer */
     const gsl_multimin_fdfminimizer_type *T = 
	  gsl_multimin_fdfminimizer_steepest_descent;
     gsl_multimin_fdfminimizer *m =  gsl_multimin_fdfminimizer_alloc(T, 2);

     /* initialize function to be minimized */
     gsl_multimin_function_fdf mf;
     mf.f = min_f;
     mf.df = min_df;
     mf.fdf = min_fdf;
     mf.n = 2;
     mf.params = NULL;

     /* initialize initial position and stepsizes */
     /* NB: when I set initial conditions x = (100, 100) and s = (100,
      * 100), I get a not-so-good minimum.  I can't set boundaries
      * either (i.e. don't go lower than x, y = 0 */
     gsl_vector *x;
     x = gsl_vector_alloc(2);
     gsl_vector *s;
     s = gsl_vector_alloc(2);

     double x0, y0, step;
     if (argc > 1) {
	  x0 = atof(argv[1]);
	  y0 = atof(argv[2]);
	  gsl_vector_set(x, 0, x0);
	  gsl_vector_set(x, 1, y0);
     } else {
	  gsl_vector_set(x, 0, 1);
	  gsl_vector_set(x, 1, 1);
     }
     


     gsl_multimin_fdfminimizer_set(m, &mf, x, step, tolerance);
     int status;
     do {
	  status = gsl_multimin_fdfminimizer_iterate(m);
	  
	  if (status)
	       break;

	  status = gsl_multimin_test_gradient (m->gradient, 1e-3);
	  
     } while (status == GSL_CONTINUE);

     printf("(x, y) = (%g, %g)\n", 
	    gsl_vector_get(m->x, 0),
	    gsl_vector_get(m->x, 1));
     printf("f(x, y) = %g\n", m->f);
     printf("d2f/dx^2(x, y) = %g\n", d2fdx2(m->x, NULL));
     printf("d2f/dy^2(x, y) = %g\n", d2fdy2(m->x, NULL));
     printf("d2f/dxdy(x, y) = %g\n", d2fdxdy(m->x, NULL));
     
     return 0;
}

double f(double x, double y)
{
     return -100 * (exp(-x)*log(x+y) - y*y);
}

double dfdx(double x, double y)
{
     return -100*exp(-x)*( 1.0/(x+y) - log(x+y) );
}

double dfdy(double x, double y)
{
     return -100*(exp(-x)/(x+y) - 2*y);
}

double min_f(const gsl_vector *v, void *p)
{
     double x, y;
     x = gsl_vector_get(v, 0);
     y = gsl_vector_get(v, 1);

     return f(x, y);
}

void min_df(const gsl_vector *v, void *p, gsl_vector *g)
{
     double x, y;
     x = gsl_vector_get(v, 0);
     y = gsl_vector_get(v, 1);

     gsl_vector_set(g, 0, dfdx(x, y));
     gsl_vector_set(g, 1, dfdy(x, y));
}

void min_fdf(const gsl_vector *v, void *p, double *f, gsl_vector *g)
{
     *f = min_f(v, p);
     min_df(v, p, g);
}

double d2fdx2(const gsl_vector *v, void *p)
{
     double x, y;
     x = gsl_vector_get(v, 0);
     y = gsl_vector_get(v, 1);

     return -100 * exp(-x) * ( -1.0 / pow(x+y, 2) - 2.0 / (x+y) +
			       log(x + y) );
}

double d2fdy2(const gsl_vector *v, void *p)
{
     double x, y;
     x = gsl_vector_get(v, 0);
     y = gsl_vector_get(v, 1);
     return 100 *  ( exp(-x) / pow(x+y, 2) + 2 );
}

double d2fdxdy(const gsl_vector *v, void *p)
{
     double x, y;
     x = gsl_vector_get(v, 0);
     y = gsl_vector_get(v, 1);
     return -100 * exp(-x) * ( -1.0 / pow(x+y, 2) - 1.0 / (x+y) );
}
