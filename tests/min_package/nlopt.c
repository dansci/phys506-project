#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <nlopt.h>

double mf(unsigned n, const double *x, double *grad, void *f_data);
double f(double x, double y);

const double tolerance = 0.001;

int main(int argc, char *argv[])
{
     nlopt_opt opt;
     opt = nlopt_create(NLOPT_GN_ISRES, 2);

     nlopt_result res;
     double lb[2] = {0.0, 0.0};
     double ub[2] = {1000, 1000};
     res = nlopt_set_lower_bounds(opt, lb);
     res = nlopt_set_upper_bounds(opt, ub);
     res = nlopt_set_min_objective(opt, mf, NULL);
     res = nlopt_set_ftol_rel(opt, tolerance);
     nlopt_set_maxtime(opt, 1.0);

     double x[2];
     if (argc > 1) {
	  x[0] = atof(argv[1]);
	  x[1] = atof(argv[2]);
     } else {
	  x[0] = 1;
	  x[1] = 1;
     }

     double fval = mf(2, x, NULL, NULL);
     res = nlopt_optimize(opt, x, &fval);

     printf("res=%d\n", res);
     printf("(x, y) = (%g, %g)\n", x[0], x[1]);
     printf("f(x, y) = %g\n", fval);
     return 0;
}


double mf(unsigned n, const double *x, double *grad, void *f_data)
{
     return f(x[0], x[1]);
}

double f(double x, double y)
{
     return -100*(exp(-x)*log(x+y) - y*y);
}
