#!/bin/bash

#./eternity_no_plateau.sh pieces_10x10.txt 10x10_output_no_plateau.txt 10x10_no_plateau_stats.txt

echo $1 >$3

step=1000000
imp=100 #not influent

for rand in {1..10}
do
   ./eternity_no_plateau $1 $2 TRIPLE 3 15 15 23 $step $imp CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START $rand>>$3
done
 

