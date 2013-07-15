#!/bin/bash

#./eternity_politics.sh pieces_10x10.txt 10x10_output_pt01.txt 10x10_tuning_stats_pt01.txt

#./eternity pieces_10x10.txt 10x10_output.txt TRIPLE 10 10 10 15 100000 20 CCORNER_OPPOSITE EEDGE_NEXT IINNER_OPPOSITE STATIC_START

echo $1 >$3

TabuSizeCorner=( 3 4 )

TabuSizeEdge=( 6 8 10 12 14 )

TabuSizeInner=( 6 8 10 12 14 )

ExpireTime=( 26 28 30 32 34 36 )

#MaxSteps=( 100000 250000 500000 750000 1000000 )

#MaxNotImp=( 10 15 20 25 )

step=100000
imp=100 #not influent

for csize in ${TabuSizeCorner[*]}
do
  for expt in ${ExpireTime[*]}
  do
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
