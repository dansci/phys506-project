/* consists of minimizing the log likelihood.  which i calculated to */
/* be (assuming the following definitions) : */
/*  t_i = time photon i was detected */
/*  t_0 = time of event */
/*  x_i = position photon i was detected */
/*  x_0 = position of event */
/*  alpha = scintillation characteristic time */
/*  c = speed of light */

/*  L = exp(t_i - t_0 - |x_i - x_0|/c) */
/* -ln L = \sum_{i=1}^N (t_i - t_0 - |x_i - x_0|/c)

/* we may want to have this floating?  */
/* for now set it equal to scint_time */

const double BIG = 1e300;
const double alpha = 5e-8;
const double PENALTY = 200;

double bigger() {
     static double factor = 1;

     factor += .1;
     return factor * BIG;
}

double f(const gsl_vector *v, void *p)
{

     /*
       The shape of *v is:
       {x0, y0, z0, t0}
     */
       
     /* the *p parameter is now just the event struct pointer */
	
     int N = ((struct event *) p)->N; /* number of photons */
     struct hit *hits = ((struct event *) p)->hits;

     gsl_vector_view v0 = 
	  gsl_vector_subvector (v, 0, 3);
     double t0 = gsl_vector_get(v, 3);

     if (vec_length(&v0.vector) > 6.0)
	  return bigger();

     /* if (t0 < 0) */
     /* 	  return bigger(); */
     
     int i;
     double t_i, d_i;
     double total = 0;
     for (i=0; i<N; i++) {
	  t_i = hits[i].hit_time;
	  d_i = vec_distance(hits[i].hit_pos, &v0.vector);
	  
	  if (t0 > t_i - d_i/light_speed)
	       total += PENALTY;
	  
	  total += t_i - t0 - d_i/light_speed;
     }
     
     return total;
}
	       
