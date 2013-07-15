#!/bin/bash

#./eternity_fine_tuning_pt04.sh pieces_10x10.txt 10x10_output_pt04.txt 10x10_fine_tuning_stats_pt04.txt

TabuSizeCorner[9]=4
TabuSizeEdge[9]=12
TabuSizeInner[9]=12
ExpireTime[9]=32
TabuSizeCorner[10]=4
TabuSizeEdge[10]=12
TabuSizeInner[10]=12
ExpireTime[10]=36
TabuSizeCorner[11]=3
TabuSizeEdge[11]=10
TabuSizeInner[11]=10
ExpireTime[11]=30


echo $1 >$3

step=100000
imp=100 #not influent

for i in {9..11}
do
  echo $i
  for rand in {1..100}
  do
./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_POS_OPPOSITE IINNER_NEXT RANDOM_START $rand>>$3
  done
done 

