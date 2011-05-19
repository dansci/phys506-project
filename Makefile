timing: timing.c event.h geometry.h random.h reconstruct.h
	gcc -o timing timing.c -lm -lgsl -lgslcblas -lnlopt -O2

timing_debug: timing.c event.h geometry.h random.h reconstruct.h
	gcc -o timing_debug timing.c -lm -lgsl -lgslcblas -lnlopt -g
