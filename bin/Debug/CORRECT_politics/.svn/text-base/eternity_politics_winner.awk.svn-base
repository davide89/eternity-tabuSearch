#!/bin/awk -f

#awk -f eternity_politics_winner.awk -v out=05x05_politics_winner.txt -v scores=05x05_politics_scores.txt 05x05_politics_resume.txt


BEGIN {
    while (getline <scores) {
      bestPolitic=$2;
      avgPolitic=$4;
    }
    frame=0;
    new_frame=1; #first ---------- row considered
    printf("") >out;
}

#NEW FRAME RECOGNITION
(/\-+/) {
  if (new_frame==1) {
    if (new_best==1) {
      print "----------------------" >>out;
      print "BEST_SOLUTION_POLITIC" >>out;
      print ("TabuListsNumber: " tabu_num) >>out
      print ("CornerGenerator: " corner_gen) >>out
      print ("EdgeGenerator: " edge_gen) >>out
      print ("InnerGenerator: " inner_gen) >>out
      printf("MaximumScore: %d\n",max) >>out;
      printf("AverageScore: %.3f\n",score_avg) >>out;
      printf("AverageSolutionQuality: %.3f%%\n",qual_avg) >>out;
      printf("BestScoreOverall: %d\n",score_best) >>out;
      printf("BestSolutionQuality: %.3f%%\n",qual_best) >>out;  
      print "----------------------" >>out;
    }
    if (new_avg==1) {
      print "----------------------" >>out;
      print "AVERAGE_SOLUTION_POLITIC" >>out;
      print ("TabuListsNumber: " tabu_num) >>out
      print ("CornerGenerator: " corner_gen) >>out
      print ("EdgeGenerator: " edge_gen) >>out
      print ("InnerGenerator: " inner_gen) >>out
      printf("MaximumScore: %d\n",max) >>out;
      printf("AverageScore: %.3f\n",score_avg) >>out;
      printf("AverageSolutionQuality: %.3f%%\n",qual_avg) >>out;
      printf("BestScoreOverall: %d\n",score_best) >>out;
      printf("BestSolutionQuality: %.3f%%\n",qual_best) >>out;  
      print "----------------------" >>out;
    }
    frame++;
    new_frame=0;
    new_best=0;
    new_avg=0;
  } else {
    new_frame=1;
  }
}

(!(/\-+/)) {
  new_frame=0;
}

#PARAMETERS RECOGNITION
($1=="TabuListsNumber:") {
    tabu_num=$2;
}

($1=="CornerGenerator:") {
  corner_gen=$2;
}

($1=="EdgeGenerator:") {
  edge_gen=$2;
}

($1=="InnerGenerator:") {
  inner_gen=$2;
}

($1=="MaximumScore:") {
  max=$2;
}

($1=="BestSolutionQuality:") {
  qual_best=$2;
}

($1=="AverageSolutionQuality:") {
  qual_avg=$2;
}

($1=="BestScoreOverall:") {
  score_best=$2
  if (bestPolitic==score_best)  {
    new_best=1;
  }
}

($1=="AverageScore:") {
  score_avg=$2;
  if (avgPolitic==score_avg)  {
    new_avg=1;
  }
}