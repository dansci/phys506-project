
const double radius = 6.0;

/* GET_INTERSECT/3: gives (through argument RES) the point of
 * intersection on a sphere of radius RADIUS given a starting point
 * vector POS and a direction vector DIR. */ 
void get_intersect(gsl_vector *pos, gsl_vector *dir, gsl_vector *res)
{
     /* using formula
     alpha = -(dx*x0 + dy*y0 + dz*z0) + 
	  sqrt(pow(dx*x0+dy*y0+dz*z0, 2) - (x0*x0+y0*y0+z0*z0-R*R));
     */
     int i;
     double A, B;
     A = B = 0;
     
     for(i=0; i<pos->size; i++) {
	  A += gsl_vector_get(dir, i) * gsl_vector_get(pos, i);
	  B += pow(gsl_vector_get(pos, i), 2);
     }
     
     B -= pow(radius, 2);
     double alpha = -A + sqrt(pow(A, 2) - B);

     /* FIXME: maybe i want to use the gsl vector operations to do
      * this? */
     for (i=0; i<pos->size; i++)
	  gsl_vector_set(res, i, gsl_vector_get(pos, i) + 
			 alpha*gsl_vector_get(dir, i));
}

/* VEC_LENGTH/1: returns the length of a vector */
double vec_length(gsl_vector *v)
{
     int i;
     double total = 0;
     for (i=0; i<v->size; i++)
	  total += pow(gsl_vector_get(v, i), 2);
     return sqrt(total);
}

/* VEC_DISTANCE/1: returns the distance between two points */
double vec_distance(gsl_vector *v1, gsl_vector *v2)
{
     gsl_vector *vd = gsl_vector_alloc(v1->size);
     
     gsl_vector_memcpy (vd, v1);
     gsl_vector_sub (vd, v2);
     
     double ret = vec_length(vd);
     gsl_vector_free(vd);
     
     return ret;
}
