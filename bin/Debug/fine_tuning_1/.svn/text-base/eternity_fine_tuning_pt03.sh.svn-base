#!/bin/bash

#./eternity_fine_tuning_pt03.sh pieces_10x10_pt03.txt 10x10_output_pt03.txt 10x10_fine_tuning_stats_pt03.txt

TabuSizeCorner[19]=7
TabuSizeEdge[19]=10
TabuSizeInner[19]=23
ExpireTime[19]=27
TabuSizeCorner[20]=7
TabuSizeEdge[20]=13
TabuSizeInner[20]=13
ExpireTime[20]=23
TabuSizeCorner[21]=7
TabuSizeEdge[21]=15
TabuSizeInner[21]=17
ExpireTime[21]=30
TabuSizeCorner[22]=7
TabuSizeEdge[22]=17
TabuSizeInner[22]=17
ExpireTime[22]=23
TabuSizeCorner[23]=7
TabuSizeEdge[23]=17
TabuSizeInner[23]=17
ExpireTime[23]=33
TabuSizeCorner[24]=7
TabuSizeEdge[24]=17
TabuSizeInner[24]=20
ExpireTime[24]=33
TabuSizeCorner[25]=7
TabuSizeEdge[25]=17
TabuSizeInner[25]=23
ExpireTime[25]=30
TabuSizeCorner[26]=9
TabuSizeEdge[26]=10
TabuSizeInner[26]=10
ExpireTime[26]=30
TabuSizeCorner[27]=9
TabuSizeEdge[27]=10
TabuSizeInner[27]=23
ExpireTime[27]=30
TabuSizeCorner[28]=9
TabuSizeEdge[28]=13
TabuSizeInner[28]=13
ExpireTime[28]=27


echo $1 >$3

step=100000
imp=100 #not influent

for i in {19..28}
do
  echo $i
  for rand in {1..50}
  do
   ./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START -1>>$3
  done
done 

