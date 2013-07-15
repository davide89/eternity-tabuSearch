#!/bin/bash

#./eternity_fine_tuning_pt02.sh pieces_10x10.txt 10x10_output_pt02.txt 10x10_fine_tuning_stats_pt02.txt

TabuSizeCorner[3]=3
TabuSizeEdge[3]=6
TabuSizeInner[3]=14
ExpireTime[3]=30
TabuSizeCorner[4]=3
TabuSizeEdge[4]=10
TabuSizeInner[4]=10
ExpireTime[4]=30
TabuSizeCorner[5]=3
TabuSizeEdge[5]=14
TabuSizeInner[5]=10
ExpireTime[5]=34


echo $1 >$3

step=100000
imp=100 #not influent

for i in {3..5}
do
  echo $i
  for rand in {1..100}
  do
./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_POS_OPPOSITE IINNER_NEXT RANDOM_START $rand>>$3
  done
done 

