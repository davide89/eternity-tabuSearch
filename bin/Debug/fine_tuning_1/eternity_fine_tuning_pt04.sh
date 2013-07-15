#!/bin/bash

#./eternity_fine_tuning_pt04.sh pieces_10x10_pt04.txt 10x10_output_pt04.txt 10x10_fine_tuning_stats_pt04.txt

TabuSizeCorner[29]=9
TabuSizeEdge[29]=13
TabuSizeInner[29]=13
ExpireTime[29]=33
TabuSizeCorner[30]=9
TabuSizeEdge[30]=13
TabuSizeInner[30]=15
ExpireTime[30]=33
TabuSizeCorner[31]=9
TabuSizeEdge[31]=13
TabuSizeInner[31]=17
ExpireTime[31]=33
TabuSizeCorner[32]=9
TabuSizeEdge[32]=15
TabuSizeInner[32]=15
ExpireTime[32]=30
TabuSizeCorner[33]=9
TabuSizeEdge[33]=15
TabuSizeInner[33]=23
ExpireTime[33]=33
TabuSizeCorner[34]=10
TabuSizeEdge[34]=10
TabuSizeInner[34]=15
ExpireTime[34]=30
TabuSizeCorner[35]=10
TabuSizeEdge[35]=13
TabuSizeInner[35]=20
ExpireTime[35]=27
TabuSizeCorner[36]=10
TabuSizeEdge[36]=13
TabuSizeInner[36]=23
ExpireTime[36]=27
TabuSizeCorner[37]=10
TabuSizeEdge[37]=15
TabuSizeInner[37]=17
ExpireTime[37]=30
TabuSizeCorner[38]=10
TabuSizeEdge[38]=17
TabuSizeInner[38]=17
ExpireTime[38]=19


echo $1 >$3

step=100000
imp=100 #not influent

for i in {29..38}
do
  echo $i
  for rand in {1..50}
  do
   ./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START -1>>$3
  done
done 

