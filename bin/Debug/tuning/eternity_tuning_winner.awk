#!/bin/awk -f

#awk -f eternity_tuning_winner.awk -v out=10x10_tuning_winner.txt -v scores=10x10_tuning_scores.txt 10x10_tuning_resume.txt


BEGIN {
    while (getline <scores) {
      bestTuning=$2;
    }
    frame=0;
    new_frame=0; #first ---------- row considered
    printf("") >out;
    getline;
}

#NEW FRAME RECOGNITION
(/\-+/) {
    if (new_best==1) {
      print "----------------------" >>out;
      print "BEST_TUNING" >>out;
      print ("TabuSizeCorner: " corner_size) >>out
      print ("TabuSizeEdge: " edge_size) >>out
      print ("TabuSizeInner: " inner_size) >>out
      print ("ExpireTime: " expt) >>out
      printf("MaximumScore: %d\n",max) >>out;
      printf("AverageScore: %.3f\n",score) >>out;
      printf("AverageSolutionQuality: %.3f%%\n",score/max*100) >>out;
      print "----------------------" >>out;
    } else if (new_range==1) {
      print "----------------------" >>out;
      print "IN_RANGE_TUNING" >>out;
      print ("TabuSizeCorner: " corner_size) >>out
      print ("TabuSizeEdge: " edge_size) >>out
      print ("TabuSizeInner: " inner_size) >>out
      print ("ExpireTime: " expt) >>out
      printf("MaximumScore: %d\n",max) >>out;
      printf("AverageScore: %.3f\n",score) >>out;
      printf("AverageSolutionQuality: %.3f%%\n",score/max*100) >>out;
      print "----------------------" >>out;
    }
    frame++;
    new_best=0;
    new_range=0;
}


#PARAMETERS RECOGNITION
($1=="TabuSizeCorner:") {
  corner_size=$2;
}

($1=="TabuSizeEdge:") {
  edge_size=$2;
}

($1=="TabuSizeInner:") {
  inner_size=$2;
}

($1=="ExpireTime:") {
  expt=$2;
}  

($1=="MaximumScore:") {
  max=$2;
}

($1=="AverageSolutionQuality:") {
  qual=$2;
}

($1=="AverageScore:") {
  score=$2;
  #sscanf($2,"%f",score);
  if ( (bestTuning==score))  {
    new_best=1;
  } else if ((bestTuning-1.347)<=score) {
    new_range=1;
  }
}