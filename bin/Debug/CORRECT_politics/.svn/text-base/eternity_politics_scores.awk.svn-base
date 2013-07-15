#!/bin/awk -f

#awk -f eternity_politics_scores.awk -v out=05x05_politics_scores.txt 05x05_politics_resume.txt


BEGIN {
    printf("") >out;
    bestPolitic=0;
    avgPolitic=0;
}

($1=="BestScoreOverall:") {
  if (bestPolitic<$2)  {
    bestPolitic=$2;
  }
}

($1=="AverageScore:") {
  if (avgPolitic<$2)  {
    avgPolitic=$2;
  }
}

END {
  print ( "BestScore: " bestPolitic " AvgScore: " avgPolitic) >>out; 
}