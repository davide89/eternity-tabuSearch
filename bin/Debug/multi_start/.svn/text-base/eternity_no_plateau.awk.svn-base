#!/bin/awk -f

#awk -f eternity_no_plateau.awk -v out=10x10_no_plateau_resume.txt 10x10_no_plateau_stats.txt


BEGIN {
    sol=0;
    new_sol=0;
    frame=1;
    new_frame=0;
    rand_num=10;
    sum=0;
    printf("") >out;
    print "----------------------" >>out;
}

#NEW SOLUTION AND FRAME RECOGNITION
(/#+/) {
  if (new_frame==1) {
    print ("TabuSizeCorner: " corner_size) >>out
    print ("TabuSizeEdge: " edge_size) >>out
    print ("TabuSizeInner: " inner_size) >>out
    print ("ExpireTime: " expt) >>out
    avg=sum/sol;
    printf("MaximumScore: %d\n",max) >>out;
    printf("AverageScore: %.3f\n",avg) >>out;
    printf("AverageSolutionQuality: %.3f%%\n",avg/max*100) >>out;
    print "----------------------" >>out;
    sol=0;
    sum=0;
    new_frame=0;
  }
  new_sol=1;
}

(/\-+/) {
  if (new_sol==1) {
    sol++;
    if (sol==rand_num) {
      new_frame=1;
    }
    new_sol=0;
  }
}
(!(/#+/ || /\-+/)) {
  new_sol=0;
}


($1=="TabuSizeCorner:") {
  corner_size=$2;
}

($1=="TabuSizeEdge:") {
  edge_size=$2;
}

($1=="TabuSizeInner:") {
  inner_size=$2;
}

($1=="BestScore:") {
  curr=$2;
  sum+=curr;
}

($1=="MaximumScore:") {
  max=$2;
}

($1=="ExpireTime:") {
  expt=$2;
}  