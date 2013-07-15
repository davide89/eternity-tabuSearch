#!/bin/bash

#./eternity_fine_tuning_pt01.sh pieces_10x10_pt01.txt 10x10_output_pt01.txt 10x10_fine_tuning_stats_pt01.txt

TabuSizeCorner[0]=3
TabuSizeEdge[0]=7
TabuSizeInner[0]=20
ExpireTime[0]=30
TabuSizeCorner[1]=3
TabuSizeEdge[1]=13
TabuSizeInner[1]=15
ExpireTime[1]=30
TabuSizeCorner[2]=3
TabuSizeEdge[2]=15
TabuSizeInner[2]=15
ExpireTime[2]=19
TabuSizeCorner[3]=3
TabuSizeEdge[3]=15
TabuSizeInner[3]=15
ExpireTime[3]=23
TabuSizeCorner[4]=3
TabuSizeEdge[4]=15
TabuSizeInner[4]=20
ExpireTime[4]=27
TabuSizeCorner[5]=3
TabuSizeEdge[5]=15
TabuSizeInner[5]=23
ExpireTime[5]=27
TabuSizeCorner[6]=5
TabuSizeEdge[6]=5
TabuSizeInner[6]=23
ExpireTime[6]=23
TabuSizeCorner[7]=5
TabuSizeEdge[7]=7
TabuSizeInner[7]=10
ExpireTime[7]=30
TabuSizeCorner[8]=5
TabuSizeEdge[8]=7
TabuSizeInner[8]=20
ExpireTime[8]=23
TabuSizeCorner[9]=5
TabuSizeEdge[9]=10
TabuSizeInner[9]=17
ExpireTime[9]=33

echo $1 >$3

step=100000
imp=100 #not influent

for i in {0..9}
do
  echo $i
  for rand in {1..50}
  do
   ./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START -1>>$3
  done
done 

