#!/bin/bash

#./eternity_politics.sh <input_file> <output_file> <stats_output_file>

#./eternity pieces_10x10.txt 10x10_output.txt TRIPLE 10 10 10 15 100000 20 CCORNER_OPPOSITE EEDGE_NEXT IINNER_OPPOSITE STATIC_START

echo $1 >$3

TabuSizeCorner=( 3 5 7 9 10 )

TabuSizeEdge=( 5 7 10 13 15 17)

TabuSizeInner=( 7 10 13 15 17 20 23)

ExpireTime=( 15 19 23 27 30 33)

#MaxSteps=( 100000 250000 500000 750000 1000000 )

#MaxNotImp=( 10 15 20 25 )

step=100000
imp=100 #not influent

for csize in ${TabuSizeCorner[*]}
do
  for esize in ${TabuSizeEdge[*]}
  do
    if [ $esize -lt $csize ]
    then
      continue
    fi
    echo $esize
    for isize in ${TabuSizeInner[*]}
    do
      if [ $isize -lt $esize ]
      then
	continue
      fi
      for expt in ${ExpireTime[*]}
      do
	if [ $expt -gt $isize ]
	then
	  continue
	fi
	#for step in ${MaxSteps[*]}
	#do
	  #for imp in ${MaxNotImp[*]}
	  #do
	    for rand in {1..10}
	    do
	    ./eternity $1 $2 TRIPLE $csize $esize $isize $expt $step $imp CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START -1>>$3
	    done
	  #done
	#done
      done
    done
  done
done
