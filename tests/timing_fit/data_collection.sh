#!/bin/sh

PROG=./timing_accuracy
NHITSMIN=100
NHITSMAX=1000
NHITSSTRIDE=100
NTRIALS=1000
TOL=.01

NHITS=$NHITSMIN
while [ $NHITS -le $NHITSMAX ]; do
    GSL_RNG_SEED=1
    echo "on nhits=$NHITS"
    while [ $GSL_RNG_SEED -le $NTRIALS ]; do
	export GSL_RNG_SEED
	$PROG -N $NHITS -t $TOL >> times_N${NHITS}_t$TOL.txt
	GSL_RNG_SEED=$(( GSL_RNG_SEED + 1 ))
    done
    NHITS=$(( NHITS + NHITSSTRIDE ))
done