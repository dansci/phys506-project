const double scint_time = 5e-8; /* 50ns? */
const double light_speed = 3e8;

struct event {
     int N; /* number of photon hits */
     gsl_vector *spawn_pos;
     double spawn_time;
     struct hit *hits;
};

struct hit {
     gsl_vector *hit_pos;
     double hit_time;
};

/* make_event/2: fill event structure *E with N hits originating from
 * same (random) position*/
void make_event(struct event *e, int N)
{
     e->N = N;
     e->spawn_pos = gsl_vector_alloc(3);
     spherical_random_position(e->spawn_pos, 6.0);

     /* FIXME: maybe you want to change this in future; make it
      * variable... */
     e->spawn_time = 0;

     gsl_vector *dir = gsl_vector_alloc(3);
     e->hits = malloc(sizeof(struct hit)*N);
     int i;
     for (i=0; i<N; i++) {
	  e->hits[i].hit_pos = gsl_vector_alloc(3);
	  spherical_random_direction(dir);
	  get_intersect(e->spawn_pos, dir, e->hits[i].hit_pos);

	  /* FIXME: you may want to keep track of when the photon was
	   * actually emitted */
	  e->hits[i].hit_time = 
	       e->spawn_time + sample_scint_time(scint_time) +
	       vec_distance(e->spawn_pos, e->hits[i].hit_pos)/light_speed;
     }
     gsl_vector_free(dir);
}
