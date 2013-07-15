#!/bin/bash

#./eternity_fine_tuning_pt03.sh pieces_10x10.txt 10x10_output_pt03.txt 10x10_fine_tuning_stats_pt03.txt

TabuSizeCorner[6]=3
TabuSizeEdge[6]=12
TabuSizeInner[6]=12
ExpireTime[6]=36
TabuSizeCorner[7]=4
TabuSizeEdge[7]=8
TabuSizeInner[7]=10
ExpireTime[7]=28
TabuSizeCorner[8]=4
TabuSizeEdge[8]=12
TabuSizeInner[8]=8
ExpireTime[8]=28



echo $1 >$3

step=100000
imp=100 #not influent

for i in {6..8}
do
  echo $i
  for rand in {1..100}
  do
 ./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_POS_OPPOSITE IINNER_NEXT RANDOM_START $rand>>$3
  done
done 

