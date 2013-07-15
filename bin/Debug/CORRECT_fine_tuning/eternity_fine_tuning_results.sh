#!/bin/bash

# ./eternity_fine_tuning_results.sh <NxN_tuning_stats> <NxN_tuning_resume> <NxN_tuning_scores> <NxN_tuning_winner>
# ./eternity_fine_tuning_results.sh 10x10_fine_tuning_stats.txt 10x10_fine_tuning_resume.txt 10x10_fine_tuning_scores.txt 10x10_fine_tuning_winner.txt

awk -f eternity_fine_tuning.awk -v out=$2 $1
awk -f eternity_fine_tuning_scores.awk -v out=$3 $2
awk -f eternity_fine_tuning_winner.awk -v out=$4 -v scores=$3 $2