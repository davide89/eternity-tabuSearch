#!/bin/bash

#./eternity_fine_tuning_pt02.sh pieces_10x10_pt02.txt 10x10_output_pt02.txt 10x10_fine_tuning_stats_pt02.txt

TabuSizeCorner[10]=5
TabuSizeEdge[10]=10
TabuSizeInner[10]=20
ExpireTime[10]=33
TabuSizeCorner[11]=5
TabuSizeEdge[11]=13
TabuSizeInner[11]=13
ExpireTime[11]=15
TabuSizeCorner[12]=7
TabuSizeEdge[12]=7
TabuSizeInner[12]=10
ExpireTime[12]=30
TabuSizeCorner[13]=7
TabuSizeEdge[13]=7
TabuSizeInner[13]=13
ExpireTime[13]=15
TabuSizeCorner[14]=7
TabuSizeEdge[14]=7
TabuSizeInner[14]=15
ExpireTime[14]=23
TabuSizeCorner[15]=7
TabuSizeEdge[15]=7
TabuSizeInner[15]=17
ExpireTime[15]=30
TabuSizeCorner[16]=7
TabuSizeEdge[16]=7
TabuSizeInner[16]=23
ExpireTime[16]=30
TabuSizeCorner[17]=7
TabuSizeEdge[17]=10
TabuSizeInner[17]=15
ExpireTime[17]=30
TabuSizeCorner[18]=7
TabuSizeEdge[18]=10
TabuSizeInner[18]=23
ExpireTime[18]=23


echo $1 >$3

step=100000
imp=100 #not influent

for i in {10..18}
do
  echo $i
  for rand in {1..50}
  do
   ./eternity $1 $2 TRIPLE ${TabuSizeCorner[$i]} ${TabuSizeEdge[$i]} ${TabuSizeInner[$i]} ${ExpireTime[$i]} $step $imp CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START -1>>$3
  done
done 

