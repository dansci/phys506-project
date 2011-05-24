const double scint_time = 5e-9; /* 5ns? */
const double light_speed = 3e8;

struct pmtmap {
     int N; /* number of PMTS */
     int ntheta, nphi; /* division of theta, phi directions */
     struct pmt *pmt;
};

struct pmt {
     double x[3]; /* position in cartesian */
     double p[3]; /* and polar coords */
}

struct event {
     /* timing fitter info */
     int N; /* number of photon hits */
     double *spawn_pos;
     double spawn_time;
     struct hit *hits;

     /* pos distn fitter info */
     double *pmt_hits; /* number of hits per PMT */
     double *e_pmt_hits; /* expected number of hits per PMT */
};

struct hit {
     double *hit_pos;
     double hit_time;
};

int hit_time_cmp(struct hit *h1, struct hit *h2)
{
     if (h1->hit_time > h2->hit_time)
	  return 1;
     else 
	  return -1;
}

void sort_event(struct event *e)
{
     qsort(e->hits, e->N, sizeof(struct hit), hit_time_cmp);
}

static double dir[3]; /* avoids having to malloc every time */
		 
/*
 * make_event/2: fill event structure *E with N hits originating from
 * same (random) position.
 *
 * NB, this only fills the parts relevant for the timing fitter so far
 */
void make_event(struct event *e, int N)
{
     e->N = N;
     e->spawn_pos = malloc(3*sizeof(double));
     spherical_random_position(e->spawn_pos, 6.0);

     /* FIXME: maybe you want to change this in future; make it
      * variable... */
     e->spawn_time = 0;

     e->hits = malloc(sizeof(struct hit)*N);
     int i;
     for (i=0; i<N; i++) {
	  e->hits[i].hit_pos = malloc(3*sizeof(double));
	  spherical_random_direction(dir);
	  get_intersect(e->spawn_pos, dir, e->hits[i].hit_pos);

	  /* FIXME: you may want to keep track of when the photon was
	   * actually emitted */
	  e->hits[i].hit_time = 
	       e->spawn_time + sample_scint_time(scint_time) +
	       vec_distance(e->spawn_pos, e->hits[i].hit_pos, 3)/light_speed;
     }
}

void fill_pmt_info(struct event *e, struct pmtmap *p)
{
     int i;
     /* clear the PMT hits initially */
     for (i=0; i<p->N; i++)
	  e->pmt_hits[i] = 0;

     /* loop over photons, incrementing the relevant PMT number */
     for (i=0; i<e->N; i++)
	  e->pmt_hits[get_pmt_number(p, e->hits[i].hit_pos)]++;
}

int get_pmt_number(struct pmtmap *p, double *pos)
{ 
     int ret = 0;
     /* we're dividing the detector by angle, so position in angle is
      * much more useful to us.  Maybe we'll put that information
      * directly in a struct one day */

     /* FIXME: inefficient; not using all the info available to us */
     double r,theta, phi;
     r = sqrt(pos[0]*pos[0] + pos[1]*pos[1] + pos[2]*pos[2]);
     theta = acos(pos[2]/r);
     phi = atan(pos[1]/pos[0]);
     
     double dtheta = M_PI / (2*p->ntheta); /* the regions covered by */
     double dphi = 2*M_PI / (2*p->nphi);   /* the pmt from its center*/

     int i;
     for (i=0; ; i++)
	  if (theta > p->pmt[i].p[1] + dtheta &&
	      theta < p->pmt[i+1].p[1] - dtheta) {
	       ret += i*p->nphi;
	       break;
	  }

     for (i=0; ; i++)
	  if (phi > p->pmt[i].p[2] + dphi &&
	      phi < p->pmt[i+1].p[2] - dphi) {
	       ret += i;
	       break;
	  }

     return ret;
}
