#!/bin/bash

#./eternity_fine_tuning_pt01.sh pieces_10x10.txt 10x10_output_pt01.txt 10x10_fine_tuning_stats_pt01.txt

TabuSizeCorner[0]=3
TabuSizeEdge[0]=8
TabuSizeInner[0]=14
ExpireTime[0]=26
TabuSizeCorner[1]=3
TabuSizeEdge[1]=12
TabuSizeInner[1]=14
ExpireTime[1]=28
TabuSizeCorner[2]=3
TabuSizeEdge[2]=14
TabuSizeInner[2]=6
ExpireTime[2]=28


echo $1 >$3

step=100000
imp=100 #not influent

for i in {0..2}
do
  echo $i
  for rand in {1..100}
  do
   ./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_POS_OPPOSITE IINNER_NEXT RANDOM_START $rand>>$3
  done
done 

