#!/bin/awk -f

#awk -f eternity_data.awk -v out=10x10_data_resume.txt 10x10_data_stats.txt


BEGIN {
    sol=0;
    new_sol=0;
    sum=0;
    best=0;
    printf("") >out;
    print "----------------------" >>out;
}

#NEW SOLUTION AND FRAME RECOGNITION
(/#+/) {
  new_sol=1;
}

(/\-+/) {
  if (new_sol==1) {
    sol++;
    print (sol " " seed)
  }
  new_sol=0;
}

(!(/#+/ || /\-+/)) {
  new_sol=0;
}

($1=="BestScore:") {
  curr=$2;
  sum+=curr;
   if (best<curr) best=curr;
}

($1=="MaximumScore:") {
  max=$2;
} 

($1=="RandomizeSeed:") {
  seed=$2;
} 

END{
    avg=sum/sol;
    printf("Data: %d\n",sol) >>out;
    printf("MaximumScore: %d\n",max) >>out;
    printf("BestScore: %d\n",best) >>out;
    printf("AverageScore: %.3f\n",avg) >>out;
    printf("AverageSolutionQuality: %.3f%%\n",avg/max*100) >>out;
    print "----------------------" >>out;
}