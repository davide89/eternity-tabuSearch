#!/bin/bash

#./eternity_data_pt02.sh pieces_10x10.txt 10x10_output_pt02.txt 10x10_data_pt02.txt

echo $1>$3

for rand in {51..100}
do
echo $rand
  ./eternity pieces_10x10.txt 10x10_output.txt 100000 25 $rand >>$3   
done