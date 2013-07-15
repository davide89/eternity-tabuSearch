#!/bin/bash

#./eternity_fine_tuning2_pt02.sh pieces_10x10_pt02.txt 10x10_output_pt02.txt 10x10_fine_tuning2_stats_pt02.txt

TabuSizeCorner[4]=5
TabuSizeEdge[4]=5
TabuSizeInner[4]=23
ExpireTime[4]=23
TabuSizeCorner[5]=5
TabuSizeEdge[5]=7
TabuSizeInner[5]=10
ExpireTime[5]=30
TabuSizeCorner[6]=5
TabuSizeEdge[6]=10
TabuSizeInner[6]=17
ExpireTime[6]=33
TabuSizeCorner[7]=7
TabuSizeEdge[7]=17
TabuSizeInner[7]=17
ExpireTime[7]=23


echo $1 >$3

step=100000
imp=100 #not influent

for i in {4..7}
do
  echo $i
  for rand in {1..200}
  do
   ./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START -1>>$3
  done
done 

