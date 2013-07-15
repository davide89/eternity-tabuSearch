#!/bin/awk -f

#awk -f eternity_politics_clean2.awk -v out=tmp.txt 10x10_politics_stats.txt

BEGIN {
  flip=1;
  print "" >out
}

($1=="RESULTS") {
  flip++;
  flip=flip%2;
  if (flip==1)
    print $0 >> out;
}

($1!="RESULTS") {
  if (flip==1) {
    print $0 >> out;
  }
}