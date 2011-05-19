const double scint_time = 5e-9; /* 5ns? */
const double light_speed = 3e8;

struct event {
     int N; /* number of photon hits */
     double *spawn_pos;
     double spawn_time;
     struct hit *hits;
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
		 
/* make_event/2: fill event structure *E with N hits originating from
 * same (random) position*/
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
