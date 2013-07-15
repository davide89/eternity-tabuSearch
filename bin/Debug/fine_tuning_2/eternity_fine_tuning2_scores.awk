#!/bin/awk -f

#awk -f eternity_fine_tuning2_scores.awk -v out=10x10_fine_tuning2_scores.txt 10x10_fine_tuning2_resume.txt


BEGIN {
    printf("") >out;
    bestTuning=0;
}

($1=="AverageScore:") {
  if (bestTuning<$2)  {
    bestTuning=$2;
  }
}

END {
  print ( "BestScore: " bestTuning) >>out; 
}