#!/bin/bash

#./eternity_data_pt01.sh pieces_10x10.txt 10x10_output_pt01.txt 10x10_data_pt01.txt

echo $1>$3

for rand in {1..50}
do
echo $rand
  ./eternity pieces_10x10.txt 10x10_output.txt 100000 25 $rand >>$3   
done