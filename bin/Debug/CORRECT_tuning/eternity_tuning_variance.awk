#!/bin/awk -f

#awk -f eternity_fine_tuning_variance.awk -v out=10x10_tuning_variance.txt 10x10_tuning_stats.txt


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
  if (new_frame==2) {
    print ("TabuSizeCorner: " corner_size) >>out
    print ("TabuSizeEdge: " edge_size) >>out
    print ("TabuSizeInner: " inner_size) >>out
    print ("ExpireTime: " expt) >>out
    var=sum/sol;
    printf("Variance: %.3f\n",var) >>out;
    printf("StandardDev: %.3f\n",sqrt(var)) >>out;
    print "----------------------" >>out;
    new_frame=3;
  }
  new_sol=1;
}

(/\-+/) {
  if (new_sol==1) {
    if (new_frame==1) {
      sol++;
      if (sol==rand_num) {
	new_frame=2;
      }
    }
    new_sol=0;
  }
}
(!(/#+/ || /\-+/)) {
  new_sol=0;
}


($1=="TabuSizeCorner:") {
  if (new_frame==0)
    corner_size=$2;
}

($1=="TabuSizeEdge:") {
  if (new_frame==0)
    edge_size=$2;
}

($1=="TabuSizeInner:") {
  if (new_frame==0)
    inner_size=$2;
}

($1=="BestScore:") {
  curr=$2;
  if (new_frame==1) {
    sum+=(159-curr)*(159-curr);
  }
}

($1=="MaximumScore:") {
  max=$2;
}

($1=="ExpireTime:") {
  if (new_frame==0) {
    expt=$2;
  }
  if (corner_size==3 && edge_size==12 && inner_size==12 && expt==36 && new_frame==0) {
    sum=0;
    new_frame=1;
    sol=1;
  }
}