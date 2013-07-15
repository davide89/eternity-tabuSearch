#!/bin/bash

# ./eternity_fine_tuning2_results.sh <NxN_tuning_stats> <NxN_tuning_resume> <NxN_tuning_scores> <NxN_tuning_winner>
# ./eternity_fine_tuning2_results.sh 10x10_fine_tuning2_stats.txt 10x10_fine_tuning2_resume.txt 10x10_fine_tuning2_scores.txt 10x10_fine_tuning2_winner.txt

awk -f eternity_fine_tuning2.awk -v out=$2 $1
awk -f eternity_fine_tuning2_scores.awk -v out=$3 $2
awk -f eternity_fine_tuning2_winner.awk -v out=$4 -v scores=$3 $2