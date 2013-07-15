#!/bin/bash

#./eternity_politics.sh <input_file> <output_file> <stats_output_file>

#program calling example
#eternity pieces_10x10.txt 10x10_output.txt TRIPLE 10 10 10 15 100000 20 CCORNER_OPPOSITE EEDGE_NEXT IINNER_OPPOSITE STATIC_START

PoliticStart[0]="STATIC_START"
PoliticStart[1]="RANDOM_START"
StartDim=2

PoliticTln[0]="SINGLE"
PoliticTln[1]="TRIPLE"
TlnDim=2

PoliticCorner[0]="CCORNER_FIRST"
PoliticCorner[1]="CCORNER_NEXT"
PoliticCorner[2]="CCORNER_OPPOSITE"
CornerDim=3

PoliticEdge[0]="EEDGE_POS_OPPOSITE"
PoliticEdge[1]="EEDGE_NEXT"
PoliticEdge[2]="EEDGE_NEXT_ANTI"
EdgeDim=3

PoliticInner[0]="IINNER_NEXT"
PoliticInner[1]="IINNER_OPPOSITE"
PoliticInner[2]="IINNER_OPPOSITE_AUX"
PoliticInner[3]="IINNER_RAND"
InnerDim=4

#TabuSizeCorner[0]=3
TabuSizeCorner[0]=7
TabuSizeCorner[1]=10
SizeCornerDim=2

#TabuSizeEdge[0]=5
TabuSizeEdge[0]=10
TabuSizeEdge[1]=15
SizeEdgeDim=2

#TabuSizeInner[0]=7
TabuSizeInner[0]=10
TabuSizeInner[1]=15
SizeInnerDim=2

ExpireTime[0]=15
ExpireTime[2]=20
ExpireDim=2

#MaxSteps[0]=10000
MaxSteps[0]=50000
MaxSteps[1]=100000
StepsDim=2

MaxNotImp[0]=15
MaxNotImp[1]=25
ImpDim=2

echo $1 >$3

for tln in ${PoliticTln[*]}
do
  for corner in ${PoliticCorner[*]}
  do
    for edge in ${PoliticEdge[*]}
    do
      echo $edge
      for inner in ${PoliticInner[*]}
      do
      for csize in ${TabuSizeCorner[*]}
      do
	for esize in ${TabuSizeEdge[*]}
	do
	  for isize in ${TabuSizeInner[*]}
	  do
	    for expt in ${ExpireTime[*]}
	    do
	      for step in ${MaxSteps[*]}
	      do
		for imp in ${MaxNotImp[*]}
		do
		  #DEBUG
		  #echo "./eternity pieces_10x10.txt 10x10_output.txt $tln $csize $esize $isize $expt $step $imp $corner $edge $inner STATIC_START"
		  ./eternity $1 $2 $tln $csize $esize $isize $expt $step $imp $corner $edge $inner STATIC_START -1>>$3
		 done
		done
	      done
	    done
	  done
	done
      done
    done
  done
done