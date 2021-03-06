const double scint_time = 5e-9; /* 5ns? */
const double light_speed = 3e8;

struct pos_data {
     struct pmtmap *p;
     struct event *e;
};

struct pmtmap {
     int N; /* number of PMTS */
     int ntheta, nphi; /* division of theta, phi directions */
     struct pmt *pmt;
};

struct pmt {
     double x[3]; /* position in cartesian */
     double p[3]; /* and polar coords */
};

struct event {
     /* timing fitter info */
     int N; /* number of photon hits */
     double *spawn_pos;
     double spawn_time;
     struct hit *hits;

     /* pos distn fitter info */
     int *pmt_hits; /* number of hits per PMT */
     double *pmt_hits_normed; /* normalized to one */
     double *e_pmt_hits_normed; /* expected number of hits per PMT */
};

struct hit {
     double *hit_pos;
     double hit_time;
};

/* this should have the number of pmts and the division sorted out */
void init_pmtmap(struct pos_data *data)
{
     struct pmtmap *p = data->p;
     struct event *e = data->e;

     int i, j;

     /* allocate the space for the PMTs */
     p->pmt = malloc(p->N*sizeof(struct pmt));
     e->pmt_hits = malloc(p->N*sizeof(int));
     e->pmt_hits_normed = malloc(p->N*sizeof(double));
     e->e_pmt_hits_normed = malloc(p->N*sizeof(double));
     
     /* phi goes 0 -> 2 pi */
     double dphi = (2*M_PI)/(2*p->nphi);
     /* costheta goes from 1 -> -1  */
     double dcosth = 2.0 / (2*p->ntheta);

     double theta, phi;
     for (i=0; i<p->ntheta; i++)
	  for (j=0; j<p->nphi; j++) {
	       theta = acos(1-(2*i+1)*dcosth);
	       phi = (2*j+1)*dphi;
	       p->pmt[p->nphi*i+j].p[0] = radius;
	       p->pmt[p->nphi*i+j].p[1] = theta;
	       p->pmt[p->nphi*i+j].p[2] = phi;
	       /* FIXME: hecka inefficient */
	       p->pmt[p->nphi*i+j].x[0] = radius*sin(theta)*cos(phi);
	       p->pmt[p->nphi*i+j].x[1] = radius*sin(theta)*sin(phi);
	       p->pmt[p->nphi*i+j].x[2] = radius*cos(theta);
	  }
     
}

     
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

void fill_pmt_info(struct pos_data *data)
{
     struct event *e = data->e;
     struct pmtmap *p = data->p;

     int i;
     /* clear the PMT hits initially */
     for (i=0; i<p->N; i++)
	  e->pmt_hits[i] = 0;

     /* loop over photons, incrementing the relevant PMT number */
     for (i=0; i<e->N; i++)
	  e->pmt_hits[get_pmt_number(p, e->hits[i].hit_pos)]++;

     /* loop over pmts, not photons here */
     for (i=0; i<p->N; i++)
	  e->pmt_hits_normed[i] = (double) e->pmt_hits[i] / e->N;
}

int get_pmt_number(struct pmtmap *p, double *pos)
{ 
     int ret = 0;
     /* we're dividing the detector by angle, so position in angle is
      * much more useful to us.  Maybe we'll put that information
      * directly in a struct one day */

     double r, costheta, phi;
     r = sqrt(pos[0]*pos[0] + pos[1]*pos[1] + pos[2]*pos[2]);
     costheta = pos[2]/r;
     /* FIXME: i don't actually know if this'll give me the right answer */
     phi = atan2(pos[1],pos[0]);
     /* sort out the quadrant jank */
     phi += (phi<0)? 2*M_PI : 0;
    
     double dcostheta = 2.0 / (2*p->ntheta); /* the regions covered by */
     double dphi = 2*M_PI / (2*p->nphi);   /* the pmt from its center*/

     int i;
     for (i=0; ; i+=p->nphi) /* there are nphi duplicates of theta*/
	  /* note that costheta goes 1 -> -1, that's why the
	   * conditionals are weird. */
	  if (costheta < cos(p->pmt[i].p[1]) + dcostheta &&
	      costheta > cos(p->pmt[i].p[1]) - dcostheta)
	       break;
     
     ret += i;
     
     for (i=0; ; i++)
	  if (phi > p->pmt[i].p[2] - dphi &&
	      phi < p->pmt[i].p[2] + dphi)
	       break;

     ret += i;

     return ret;
}

static double nd[3], np[3]; /* vector of detector normal and photon direction*/

void fill_expected_info(double *pos, struct event *e, struct pmtmap *p)
{
     /* we're going to make it proportional to cos(theta)/r^2 */
     register double nd2, np2, costheta, total = 0;
     int i, j;
     for (i=0; i<p->N; i++) {
	  nd2 = np2 = costheta = 0;
	  for (j=0; j<3; j++) {
	       nd[j] = p->pmt[i].x[j]; /* outward normal from center*/
	       np[j] = p->pmt[i].x[j] - pos[j]; /*photon->pmt vec*/
	       costheta += nd[j] * np[j]; /* dot product */
	       np2 += np[j]*np[j];
	       nd2 += nd[j]*nd[j];
	  }
	  costheta /= (sqrt(np2)*sqrt(nd2));
	  e->e_pmt_hits_normed[i] = costheta/np2;
	  total += costheta / np2;
     }

     for (i=0; i<p->N; i++)
	  e->e_pmt_hits_normed[i] /= total;
     
}
