#!/bin/bash

#./eternity_tuning_pt04.sh pieces_10x10.txt 10x10_output_pt04.txt 10x10_tuning_stats_pt04.txt

echo $1 >$3

TabuSizeCorner=( 4 )

TabuSizeEdge=( 6 8 10 12 14 )

TabuSizeInner=( 6 8 10 12 14 )

ExpireTime=( 32 34 36 )

#MaxSteps=( 100000 250000 500000 750000 1000000 )

#MaxNotImp=( 10 15 20 25 )

step=100000
imp=100 #not influent

for csize in ${TabuSizeCorner[*]}
do
  for expt in ${ExpireTime[*]}
  do
  echo $expt
    for esize in ${TabuSizeEdge[*]}
    do
      for isize in ${TabuSizeInner[*]}
      do
	for rand in {1..10}
	do
	  ./eternity $1 $2 TRIPLE $csize $esize $isize $expt $step $imp CCORNER_NEXT EEDGE_POS_OPPOSITE IINNER_NEXT RANDOM_START $rand>>$3
	done
      done
    done
  done
done
