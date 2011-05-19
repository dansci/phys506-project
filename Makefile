main: main.c event.h geometry.h random.h reconstruct.h
	gcc -o main main.c -lm -lgsl -lgslcblas -lnlopt -O3

check: check_of_randomness.c
	gcc -o check check_of_randomness.c -lm -lgsl -lgslcblas -g
