#!/bin/bash

#./eternity_politics_pt05.sh pieces_10x10.txt 10x10_output_pt05.txt 10x10_politics_stats_pt05.txt

#program calling example
#eternity pieces_10x10.txt 10x10_output.txt TRIPLE 10 10 10 15 100000 20 CCORNER_OPPOSITE EEDGE_NEXT IINNER_OPPOSITE STATIC_START i

#PoliticCorner[0]="CCORNER_FIRST"
#PoliticCorner[0]="CCORNER_NEXT"
PoliticCorner[0]="CCORNER_OPPOSITE"

#PoliticEdge[0]="EEDGE_POS_OPPOSITE"
PoliticEdge[0]="EEDGE_NEXT"
#PoliticEdge[2]="EEDGE_NEXT_ANTI"

PoliticInner[0]="IINNER_NEXT"
PoliticInner[1]="IINNER_OPPOSITE"
PoliticInner[2]="IINNER_OPPOSITE_AUX"
#PoliticInner[3]="IINNER_RAND"

TabuSizeCorner[0]=3
TabuSizeCorner[1]=5
TabuSizeCorner[2]=10
TabuSizeCorner[3]=15
TabuSizeCorner[4]=20

TabuSizeEdge[0]=3
TabuSizeEdge[1]=5
TabuSizeEdge[2]=10
TabuSizeEdge[3]=15
TabuSizeEdge[4]=20

TabuSizeInner[0]=3
TabuSizeInner[1]=5
TabuSizeInner[2]=10
TabuSizeInner[3]=15
TabuSizeInner[4]=20

ExpireTime[1]=25
ExpireTime[2]=30
ExprieTime[3]=35

echo $1 >$3

for corner in ${PoliticCorner[*]}
do
  for edge in ${PoliticEdge[*]}
  do
   for inner in ${PoliticInner[*]}
   do
    echo $inner
    for csize in ${TabuSizeCorner[*]}
    do
      for esize in ${TabuSizeEdge[*]}
      do
	for isize in ${TabuSizeInner[*]}
	do
	  for expt in ${ExpireTime[*]}
	  do
	    for i in {1..4}
	      do
		./eternity $1 $2 TRIPLE $csize $esize $isize $expt 100000 100 $corner $edge $inner RANDOM_START $i >>$3
	      done
	    done
	  done
	done
      done
    done
  done
done