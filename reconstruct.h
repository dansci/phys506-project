/* consists of minimizing the log likelihood.  which i calculated to */
/* be (assuming the following definitions) : */
/*  t_i = time photon i was detected */
/*  t_0 = time of event */
/*  x_i = position photon i was detected */
/*  x_0 = position of event */
/*  alpha = scintillation characteristic time */
/*  c = speed of light */

/*  L = exp(t_i - t_0 - |x_i - x_0|/c) */
/* -ln L = \sum_{i=1}^N (t_i - t_0 - |x_i - x_0|/c) */


double mf_t(unsigned n, const double *x, double *grad, void *f_data)
{
     struct event *e = (struct event *) ((struct pos_data *) f_data)->e;
     int N = e->N; /* number of photons */
     struct hit *hits = e->hits;

     int i;
     double t_i, d_i;
     double total = 0;
     double t0 = x[3];
     
     for (i=0; i<N; i++) {
	  t_i = hits[i].hit_time;
	  d_i = vec_distance(hits[i].hit_pos, x, 3);
	  total += t_i - t0 - d_i/light_speed;
     }
     
     return total;
}

double mf_p(unsigned n, const double *x, double *grad, void *f_data)
{
     struct pmtmap *p = ((struct pos_data *) f_data)->p;
     struct event *e = (struct event *) ((struct pos_data *) f_data)->e;
     
     /* number of pmts */
     int N = p->N;
     double total = 0;

     fill_expected_info(x, e, p);

     /* calculates the log-likelihood */
     int i;
     double n_i, v_i;
     for (i=0; i<N; i++) {
	  n_i = e->pmt_hits_normed[i];
	  v_i = e->e_pmt_hits_normed[i];
	  if (e->pmt_hits[i] == 0)
	       total += v_i;
	  else
	       total += v_i - n_i + n_i*log(n_i/v_i);
     }

     /* return the log likelihood; not the chisquare (which is
      * double) */
     return total;
}

double mf(unsigned n, const double *x, double *grad, void *f_data)
{
     double timing, position;

     struct event *e = (struct event *) ((struct pos_data *) f_data)->e;

     position = mf_p(n, x, grad, f_data);
     timing = mf_t(n, x, grad, f_data);

     /* returned the combined log-likelihood */
     return timing + position;
}

double radius_check(unsigned n, const double *x, double *grad, void *f_data)
{
     /* make sure the radius is smaller than 6m */
     return x[0]*x[0] + x[1]*x[1] + x[2]*x[2] - 36;
}

double time_check(unsigned n, const double *x, double *grad, void *f_data)
{
     struct event *e = (struct event *) ((struct pos_data *) f_data)->e;
     int N = e->N; /* number of photons */
     struct hit *hits = e->hits;

     int i;
     double t_i, d;
     double r;
     double t0 = x[3];

     for (i=0; i<N; i++) {
	  t_i = hits[i].hit_time;
	  d = vec_distance(hits[i].hit_pos, x, 3);
	  r = t0 + d/light_speed - t_i;
	  if (r > 0)
	       break;
     }
     return r;
}
