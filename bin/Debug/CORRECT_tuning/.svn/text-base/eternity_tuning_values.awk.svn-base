#!/bin/awk -f

#awk -f eternity_tuning_values.awk -v out=10x10_tuning_values.txt 10x10_tuning_winner.txt


BEGIN {
    frame=0;
    new_frame=1; #first ---------- row considered
    printf("") >out;
    getline;
}

#NEW FRAME RECOGNITION
(/\-+/) {
  if (new_frame==1) {
    frame++;
    new_frame=0;
  } else {
    new_frame=1;
  }
}

(!(/\-+/)) {
    new_frame=0;
}


#PARAMETERS RECOGNITION
($1=="TabuSizeCorner:") {
  printf("TabuSizeCorner[%d]=%s\n",frame,$2) >>out;
}

($1=="TabuSizeEdge:") {
  printf("TabuSizeEdge[%d]=%s\n",frame,$2) >>out;
}

($1=="TabuSizeInner:") {
  printf("TabuSizeInner[%d]=%s\n",frame,$2) >>out;
}

($1=="ExpireTime:") {
  printf("ExpireTime[%d]=%s\n",frame,$2) >>out;
}