timing: timing.c event.h geometry.h random.h reconstruct.h
	gcc -o timing timing.c -lm -lgsl -lgslcblas -lnlopt -O2

timing_debug: timing.c event.h geometry.h random.h reconstruct.h
	gcc -o timing_debug timing.c -lm -lgsl -lgslcblas -lnlopt -g -pg

timing_accuracy: ./tests/timing_fit/timing_accuracy.c event.h geometry.h random.h reconstruct.h
	gcc -I ./ -o ./tests/timing_fit/timing_accuracy ./tests/timing_fit/timing_accuracy.c -lm -lgsl -lgslcblas -lnlopt -O2

position: position.c event.h geometry.h random.h reconstruct.h
	gcc -o position position.c -lm -lgsl -lgslcblas -lnlopt -O2

position_debug: position.c event.h geometry.h random.h reconstruct.h
	gcc -o position_debug position.c -lm -lgsl -lgslcblas -lnlopt -g -pg

position_accuracy: ./tests/position_fit/position_accuracy.c event.h geometry.h random.h reconstruct.h
	gcc -I ./ -o ./tests/position_fit/position_accuracy ./tests/position_fit/position_accuracy.c -lm -lgsl -lgslcblas -lnlopt -O2

combined: combined.c event.h geometry.h random.h reconstruct.h
	gcc -o combined combined.c -lm -lgsl -lgslcblas -lnlopt -O2

combined_debug: combined.c event.h geometry.h random.h reconstruct.h
	gcc -o combined combined.c -lm -lgsl -lgslcblas -lnlopt -g -pg

combined_accuracy: ./tests/combined_fit/combined_accuracy.c event.h geometry.h random.h reconstruct.h
	gcc -I ./ -o ./tests/combined_fit/combined_accuracy ./tests/combined_fit/combined_accuracy.c -lm -lgsl -lgslcblas -lnlopt -O2
