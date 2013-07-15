#!/bin/awk -f

#awk -f eternity_politics.awk -v out=05x05_politics_resume.txt 05x05_politics_stats.txt


BEGIN {
    sol=0;
    new_sol=0;
    frame=0;
    new_frame=0;
    init_frame=0;
    corner_gen="";
    edge_gen="";
    inner_gen="";
    tabu_num="";
    best=0;
    sum=0;
    printf("") >out;
}

#NEW SOLUTION RECOGNITION
(/#+/) {
  new_sol=1;
}

(/\-+/) {
  if (new_sol==1) {
    sol++;
    new_sol=0;
  }
}
(!(/#+/ || /\-+/)) {
  new_sol=0;
}


#NEW FRAME RECOGNITION
($1=="TabuListsNumber:") {
  if (tabu_num==""){
    tabu_num=$2;
    frame++;
    init_frame=1;
  } else if (tabu_num!=$2) {
    frame++;
    tabu_num=$2;
    new_frame=1;
  }
}

($1=="CornerGenerator:") {
  if (corner_gen==""){
    corner_gen=$2;
  } else if (corner_gen!=$2) {
    frame++;
    corner_gen=$2;
    new_frame=1;
  }
}

($1=="EdgeGenerator:") {
  if (edge_gen==""){
    edge_gen=$2;
  } else if (edge_gen!=$2) {
    frame++;
    edge_gen=$2;
    new_frame=1;
  }
}

($1=="InnerGenerator:") {
  if (inner_gen==""){
    inner_gen=$2;
  } else if (inner_gen!=$2) {
    frame++;
    inner_gen=$2;
    new_frame=1;
  }
  if (new_frame==1 || init_frame==1) {
    if (init_frame==0) {
      sol--;
      avg=sum/sol;
      print sol;
      printf("MaximumScore: %d\n",max) >>out;
      printf("AverageScore: %.3f\n",avg) >>out;
      printf("AverageSolutionQuality: %.3f%%\n",avg/max*100) >>out;
      printf("BestScoreOverall: %d\n",best) >>out;
      printf("BestSolutionQuality: %.3f%%\n",best/max*100) >>out;
      best=0;
      sol=1;
      sum=0;
      print "----------------------" >>out;
    } else init_frame=0;
    print "----------------------" >>out;
    print ("TabuListsNumber: " tabu_num) >>out
    print ("CornerGenerator: " corner_gen) >>out
    print ("EdgeGenerator: " edge_gen) >>out
    print ("InnerGenerator: " inner_gen) >>out
    new_frame=0;
  }
}

($1=="BestScore:") {
  curr=$2;
  sum+=curr;
  if (best<curr) best=curr;
}

($1=="MaximumScore:") {
  max=$2;
}

END {
  avg=sum/sol;
  printf("MaximumScore: %d\n",max) >>out;
  printf("AverageScore: %.3f\n",avg) >>out;
  printf("AverageSolutionQuality: %.3f%%\n",avg/max*100) >>out;
  printf("BestScoreOverall: %d\n",best) >>out;
  printf("BestSolutionQuality: %.3f%%\n",best/max*100) >>out;  
  print "----------------------" >>out;
} 