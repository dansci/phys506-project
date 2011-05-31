timing: timing.c event.h geometry.h random.h reconstruct.h
	gcc -o timing timing.c -lm -lgsl -lgslcblas -lnlopt -O2

timing_debug: timing.c event.h geometry.h random.h reconstruct.h
	gcc -o timing_debug timing.c -lm -lgsl -lgslcblas -lnlopt -g

timing_accuracy: ./tests/timing_fit/timing_accuracy.c event.h geometry.h random.h reconstruct.h
	gcc -I ./ -o ./tests/timing_fit/timing_accuracy ./tests/timing_fit/timing_accuracy.c -lm -lgsl -lgslcblas -lnlopt

position: position.c event.h geometry.h random.h reconstruct.h
#	gcc -o position position.c -lm -lgsl -lgslcblas -lnlopt -g -pg
	gcc -o position position.c -lm -lgsl -lgslcblas -lnlopt -O2