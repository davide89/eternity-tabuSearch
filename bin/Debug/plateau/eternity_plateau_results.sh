#!/bin/bash

# ./eternity_plateau_results.sh <NxN_tuning_stats> <NxN_tuning_resume> <NxN_tuning_scores> <NxN_tuning_winner>
# ./eternity_plateau_results.sh 10x10_plateau_stats.txt 10x10_plateau_resume.txt 10x10_plateau_scores.txt 10x10_plateau_winner.txt

awk -f eternity_plateau.awk -v out=$2 $1
awk -f eternity_plateau_scores.awk -v out=$3 $2
awk -f eternity_plateau_winner.awk -v out=$4 -v scores=$3 $2