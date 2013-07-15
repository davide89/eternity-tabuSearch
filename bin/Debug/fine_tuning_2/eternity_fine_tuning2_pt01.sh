#!/bin/bash

#./eternity_fine_tuning2_pt01.sh pieces_10x10_pt01.txt 10x10_output_pt01.txt 10x10_fine_tuning2_stats_pt01.txt

TabuSizeCorner[0]=3
TabuSizeEdge[0]=7
TabuSizeInner[0]=20
ExpireTime[0]=30
TabuSizeCorner[1]=3
TabuSizeEdge[1]=15
TabuSizeInner[1]=15
ExpireTime[1]=19
TabuSizeCorner[2]=3
TabuSizeEdge[2]=15
TabuSizeInner[2]=15
ExpireTime[2]=23
TabuSizeCorner[3]=3
TabuSizeEdge[3]=15
TabuSizeInner[3]=23
ExpireTime[3]=27


echo $1 >$3

step=100000
imp=100 #not influent

for i in {0..3}
do
  echo $i
  for rand in {1..200}
  do
   ./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START -1>>$3
  done
done 

