#!/bin/bash

#./eternity_fine_tuning2_pt03.sh pieces_10x10_pt03.txt 10x10_output_pt03.txt 10x10_fine_tuning2_stats_pt03.txt

TabuSizeCorner[8]=7
TabuSizeEdge[8]=17
TabuSizeInner[8]=23
ExpireTime[8]=30
TabuSizeCorner[9]=9
TabuSizeEdge[9]=10
TabuSizeInner[9]=10
ExpireTime[9]=30
TabuSizeCorner[10]=9
TabuSizeEdge[10]=10
TabuSizeInner[10]=23
ExpireTime[10]=30
TabuSizeCorner[11]=9
TabuSizeEdge[11]=13
TabuSizeInner[11]=13
ExpireTime[11]=27


echo $1 >$3

step=100000
imp=100 #not influent

for i in {8..11}
do
  echo $i
  for rand in {1..200}
  do
   ./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START -1>>$3
  done
done 

