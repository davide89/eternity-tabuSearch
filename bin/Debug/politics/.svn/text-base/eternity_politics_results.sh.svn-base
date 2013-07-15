#!/bin/bash

# ./eternity_politics_results.sh <NxN_politics_stats> <NxN_politics_resume> <NxN_politics_scores> <NxN_politics_winner>
# ./eternity_politics_results.sh 10x10_politics_stats.txt 10x10_politics_resume.txt 10x10_politics_scores.txt 10x10_politics_winner.txt

awk -f eternity_politics.awk -v out=$2 $1
awk -f eternity_politics_scores.awk -v out=$3 $2
awk -f eternity_politics_winner.awk -v out=$4 -v scores=$3 $2