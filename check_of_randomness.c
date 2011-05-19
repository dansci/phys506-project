#include <stdio.h>
#include <string.h>
#include "random.h"

const int N = 5000;

int main(int argc, char *argv[])
{
     init_random();

     int i;
     if (argc <= 1) {
	  printf("you need to specify what you want to check\n");
	  printf("e.g. ``pos'' or ``dir''\n");
	  return 127;
     }
     
     if (!strcmp(argv[1], "pos")) {
	  double pos[3];
	  double R=6;
	  for (i=0; i<N; i++) {
	       spherical_random_position(pos, R);
	       printf("%g %g %g\n", pos[0], pos[1], pos[2]);
	  }
     } else if (!strcmp(argv[1], "dir")) {
	  double dir[3];
	  for (i=0; i<N; i++) {
	       spherical_random_direction(dir);
	       printf("%g %g %g\n", dir[0], dir[1], dir[2]);
	  }
     } else
	  printf("unknown option: %g\n", argv[1]);

     return 0;
}
