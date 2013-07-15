#!/bin/bash

#./eternity_fine_tuning2_pt04.sh pieces_10x10_pt04.txt 10x10_output_pt04.txt 10x10_fine_tuning2_stats_pt04.txt

TabuSizeCorner[12]=10
TabuSizeEdge[12]=10
TabuSizeInner[12]=15
ExpireTime[12]=30
TabuSizeCorner[13]=10
TabuSizeEdge[13]=13
TabuSizeInner[13]=20
ExpireTime[13]=27
TabuSizeCorner[14]=10
TabuSizeEdge[14]=15
TabuSizeInner[14]=17
ExpireTime[14]=30



echo $1 >$3

step=100000
imp=100 #not influent

for i in {12..14}
do
  echo $i
  for rand in {1..200}
  do
   ./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START -1>>$3
  done
done 

