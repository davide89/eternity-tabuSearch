/******LIST OF INPUT PARAMETERS******/

/*DEFAULT VERSION
1) File Input Name
2) File Output Name
3) Max Steps
4) Not Improving Max Steps
5) Randomization Seed
*/


/*LONG VERSION
1) File Input Name
2) File Output Name
3) Tabu List Single/Triple
4) Tabu Size Corner
5) Tabu Size Edge
6) Tabu Size Inner
7) Expire Time
8) Max Steps
9) Not Improving Max Steps
10) Corner Generator
11) Edge Generator
12) Inner Generator
13) Random Initial Solution
14) Randomization Seed
*/

/*PARAMETERS BEST COMBINATIONS*/
/*3 10 10 30*/
/*pieces_10x10.txt 10x10_output.txt TRIPLE 3 10 10 30 100000 100000 CCORNER_NEXT EEDGE_POS_OPPOSITE IINNER_NEXT RANDOM_START 484*/
/*3 12 12 36*/

//pieces_10x10.txt 10x10_output.txt TRIPLE 9 10 10 30 100000 100 CCORNER_NEXT EEDGE_NEXT_ANTI IINNER_OPPOSITE RANDOM_START 20


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "error.h"
#include "solution.h"
#include "input.h"
#include "output.h"
#include "neigh_swap_rotate.h"
#include "tabu.h"

#define RANDOM_START 1
#define STATIC_START 0

#define MAX_STEPS_FIRST 5000
#define MAX_NOT_IMP_FIRST 10

/* Return 1 if the difference is negative, otherwise 0.  */
int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;

    return (diff<0);
}



int main (int argc, char** argv) {
     int n;
     solution_t *start=NULL;
     solution_t *curr=NULL;
     tabuListNum Tln;
     int TabuSizeCorner,TabuSizeEdge,TabuSizeInner;
     int ExpireTime,MaxSteps,MaxNotImp;
     cornerGenerator Cg;
     edgeGenerator Eg;
     innerGenerator Ig;
     int rd;
     int seed;

   struct timeval tvBegin, tvEnd, tvDiff;




/*   //DEBUG
     neigh_t* neighborhood_inner=NULL;
     neigh_t* neighborhood_edge=NULL;
     neigh_t* neighborhood_corner=NULL;
     neighbor_t* candidate=NULL;
*/
     if (argc==15) {

          if (strcmp(argv[3],"SINGLE")==0) Tln=SINGLE;
          else if (strcmp(argv[3],"TRIPLE")==0) Tln=TRIPLE;
          else {
          print_err(WRONG_PARAM_TLN);
          return(FAILURE);
          }
          sscanf(argv[4],"%d",&TabuSizeCorner);
          sscanf(argv[5],"%d",&TabuSizeEdge);
          sscanf(argv[6],"%d",&TabuSizeInner);
          sscanf(argv[7],"%d",&ExpireTime);
          sscanf(argv[8],"%d",&MaxSteps);
          sscanf(argv[9],"%d",&MaxNotImp);

          if (strcmp(argv[10],"CCORNER_FIRST")==0) {
               Cg=CCORNER_FIRST;
          } else if (strcmp(argv[10],"CCORNER_NEXT")==0) {
               Cg=CCORNER_NEXT;
          } else if (strcmp(argv[10],"CCORNER_OPPOSITE")==0) {
               Cg=CCORNER_OPPOSITE;
          } else {
               print_err(WRONG_PARAM_CORNER);
               return(FAILURE);
          }

          if (strcmp(argv[11],"EEDGE_POS_OPPOSITE")==0) {
               Eg=EEDGE_POS_OPPOSITE;
          } else if (strcmp(argv[11],"EEDGE_NEXT")==0) {
               Eg=EEDGE_NEXT;
          } else if (strcmp(argv[11],"EEDGE_NEXT_ANTI")==0) {
               Eg=EEDGE_NEXT_ANTI;
          } else {
               print_err(WRONG_PARAM_EDGE);
               return(FAILURE);
          }

          if (strcmp(argv[12],"IINNER_NEXT")==0) {
               Ig=IINNER_NEXT;
          } else if (strcmp(argv[12],"IINNER_OPPOSITE")==0) {
               Ig=IINNER_OPPOSITE;
          } else if (strcmp(argv[12],"IINNER_OPPOSITE_AUX")==0) {
               Ig=IINNER_OPPOSITE_AUX;
          } else if (strcmp(argv[12],"IINNER_RAND")==0) {
               Ig=IINNER_RAND;
          } else {
               print_err(WRONG_PARAM_INNER);
               return(FAILURE);
          }

           if (strcmp(argv[13],"RANDOM_START")==0) {
               rd=RANDOM_START;
          } else if (strcmp(argv[13],"STATIC_START")==0) {
               rd=STATIC_START;
          } else {
               print_err(WRONG_PARAM_START);
               return(FAILURE);
          }

          sscanf(argv[14],"%d",&seed);
     } else {
          if (argc==6) {
               sscanf(argv[3],"%d",&MaxSteps);
               sscanf(argv[4],"%d",&MaxNotImp);
               sscanf(argv[5],"%d",&seed);
               rd=RANDOM_START;
          } else {
          //DEBUG
          //printf("%d\n",argc);
          print_err(WRONG_NUM_PARAM);
          return(FAILURE);
          }
     }

     squares=read_input(argv[1], &n);

     if (argc==15) {
          printf("####################################\n");
          printf("------------------------------------\nPARAMETERS\n");
          printf("PiecesNumber: %dx%d\n",n,n);
          printf("MaximumScore: %d\n",solution_max_matches(n));
          printf("TabuListsNumber: %s\n",argv[3]);
          printf("TabuSizeCorner: %s\n",argv[4]);
          printf("TabuSizeEdge: %s\n",argv[5]);
          printf("TabuSizeInner: %s\n",argv[6]);
          printf("ExpireTime: %s\n",argv[7]);
          printf("StepsThreshold: %s\n",argv[8]);
          printf("NotImprovingStepsThreshold: %s\n",argv[9]);
          printf("CornerGenerator: %s\n",argv[10]);
          printf("EdgeGenerator: %s\n",argv[11]);
          printf("InnerGenerator: %s\n",argv[12]);
          printf("RandomStarting: %s\n",argv[13]);
          printf("RandomizeSeed: %s\n",argv[14]);
          printf("------------------------------------\n");
     } else {
          printf("####################################\n");
          printf("------------------------------------\nPARAMETERS\n");
          printf("PiecesNumber: %dx%d\n",n,n);
          printf("MaximumScore: %d\n",solution_max_matches(n));
          printf("StepsThreshold: %s\n",argv[3]);
          printf("NotImprovingStepsThreshold: %s\n",argv[4]);
          printf("RandomizeSeed: %s\n",argv[5]);
          printf("------------------------------------\n");

     }

    //BEGIN TIME
    gettimeofday(&tvBegin, NULL);

     start=solution_greedy(n);
     if (rd==RANDOM_START)
          start=solution_shuffle(start,seed);
     printf("StartScore: %d\n",solution_get_matches(start));

     curr=solution_cpy(curr,start);

     if(argc==15) {

          //PARAM Version
          //(solution_t *start,tabuListNum,tabuSizeA,tabuSizeB,tabuSizeC,expire,step,noImpr,cornerGenerator corner,edgeGenerator edge,innerGenerator inner,innerInit ii);

         //First short run to provide a good starting solution for real tabu search
         curr=tabuSearchP(curr,Tln,TabuSizeCorner,TabuSizeEdge,TabuSizeInner,ExpireTime,MAX_STEPS_FIRST,MAX_NOT_IMP_FIRST,Cg,Eg,Ig,seed, PRE, SILENT);
          //Local search
          //curr=tabuSearchP(curr,Tln,0,0,0,ExpireTime,MAX_STEPS_FIRST,MAX_NOT_IMP_FIRST,Cg,Eg,Ig,seed, SILENT);
          printf("SecondStartScore: %d\n",solution_get_matches(curr));
         //Real tabu Search
         curr=tabuSearchP(curr,Tln,TabuSizeCorner,TabuSizeEdge,TabuSizeInner,ExpireTime,MaxSteps,MaxNotImp,Cg,Eg,Ig,seed,POST,NORMAL);
     } else {
          //First short run to provide a good starting solution for real tabu search
         curr=tabuSearchDefaultPreliminary(curr,MAX_STEPS_FIRST,MAX_NOT_IMP_FIRST, SILENT);
          printf("SecondStartScore: %d\n",solution_get_matches(curr));
         //Real tabu Search
         curr=tabuSearchDefault(curr,MaxSteps,MaxNotImp, seed, NORMAL);
     }
        //end
    gettimeofday(&tvEnd, NULL);

    // diff
    timeval_subtract(&tvDiff, &tvEnd, &tvBegin);
    printf("ExecutionTime: %ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
    printf("####################################\n");

    write_output(argv[2], curr);

    solution_free(start);
    solution_free(curr);

     return(SUCCESS);
}


