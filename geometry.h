const double radius = 6.0;

/* GET_INTERSECT/3: gives (through argument RES) the point of
 * intersection on a sphere of radius RADIUS given a starting point
 * vector POS and a direction vector DIR. */ 
void get_intersect(double *pos, double *dir, double *res)
{
     /* using formula
     alpha = -(dx*x0 + dy*y0 + dz*z0) + 
	  sqrt(pow(dx*x0+dy*y0+dz*z0, 2) - (x0*x0+y0*y0+z0*z0-R*R));
     */
     int i;
     double A, B;
     A = B = 0;

     /* FIXME: number of dimensions = 3 hardcoded in */
     for(i=0; i<3; i++) {
	  A += dir[i] * pos[i];
	  B += pos[i] * pos[i];
     }
     
     B -= pow(radius, 2);
     double alpha = -A + sqrt(pow(A, 2) - B);

     for (i=0; i<3; i++)
	  res[i] =  pos[i] +  alpha*dir[i];
}

/* VEC_LENGTH/1: returns the length of a vector */
double vec_length(double *v, int dim)
{
     int i;
     double total = 0;
     for (i=0; i<dim; i++)
	  total += v[i]*v[i];
     return sqrt(total);
}

double vec_distance(register double *v1, register double *v2, int dim)
{
     register double ret = 0;
     register double tmp;
     while (--dim) {
     	  tmp = v1[dim]-v2[dim];
     	  ret += tmp*tmp;
     }
     tmp = v1[0]-v2[0];
     ret += tmp*tmp;
     return sqrt(ret);
}
