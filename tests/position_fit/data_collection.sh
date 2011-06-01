#!/bin/sh

PROG=./position_accuracy
NTRIALS=100
TOL=.01

NHITSMIN=10
NHITSMAX=90
NHITSSTRIDE=10

NHITS=$NHITSMIN
while [ $NHITS -le $NHITSMAX ]; do
    GSL_RNG_SEED=1
    echo "on nhits=$NHITS"
    while [ $GSL_RNG_SEED -le $NTRIALS ]; do
	export GSL_RNG_SEED
	$PROG -N $NHITS -t $TOL >> pos_N${NHITS}_t$TOL.txt
	GSL_RNG_SEED=$(( GSL_RNG_SEED + 1 ))
    done
    NHITS=$(( NHITS + NHITSSTRIDE ))
done

NHITSMIN=100
NHITSMAX=1000
NHITSSTRIDE=100

NHITS=$NHITSMIN
while [ $NHITS -le $NHITSMAX ]; do
    GSL_RNG_SEED=1
    echo "on nhits=$NHITS"
    while [ $GSL_RNG_SEED -le $NTRIALS ]; do
	export GSL_RNG_SEED
	$PROG -N $NHITS -t $TOL >> pos_N${NHITS}_t$TOL.txt
	GSL_RNG_SEED=$(( GSL_RNG_SEED + 1 ))
    done
    NHITS=$(( NHITS + NHITSSTRIDE ))
done
