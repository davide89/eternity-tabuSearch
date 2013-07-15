
#ifndef TABU_H_INCLUDED
#define TABU_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "solution.h"
#include "neigh_swap_rotate.h"



/******************************CONSTANTS******************************/
#define TABU_SIZE_CORNER_DEFAULT 3
#define TABU_SIZE_EDGE_DEFAULT 10
#define TABU_SIZE_INNER_DEFAULT 10
#define EXPIRE_TIME_DEFAULT 30


#define ISTABU 1
#define ASPCRITERIA 0
#define NOTABU -1

#define NOT_IMPROVED 0
#define IMPROVED 1

#define CONTINUE 0
#define STOP 1

#define NORMAL 1
#define VERBOSE 2
#define SILENT 0

//SET TO 1 IN ORDER TO ENABLE FEATURE CODE COMPILATION
//ONLY TRIPLE LONG VERSION TABU SEARCH
#define PLATEAU 0
#define PLATEAU_LENGTH 8

#define HILL 0
#define HILL_LENGTH 15

#define STUCK 0
#define FLUSH 1
#define BEST_DIV 1
#define STUCK_LENGTH 10000

#define PRE 0
#define POST 1

//in the tabu list the element with index=0 is the "youngest"

/******************************ENUM******************************/

typedef enum tabuListNum{
    SINGLE,
    TRIPLE,
}tabuListNum;

typedef enum cornerGenerator{
    CCORNER_NEXT,
    CCORNER_OPPOSITE,
    CCORNER_FIRST,

}cornerGenerator;

typedef enum edgeGenerator{
    EEDGE_NEXT_ANTI,
    EEDGE_NEXT,
    EEDGE_POS_OPPOSITE,
}edgeGenerator;

typedef enum innerGenerator{
    IINNER_NEXT,
    IINNER_OPPOSITE,
    IINNER_RAND,
    IINNER_OPPOSITE_AUX,
}innerGenerator;

typedef enum innerInit{
    I_UPPERLEFT,
    I_UPPERRIGHT,
    I_LOWERRIGHT,
    I_LOWERLEFT,
}innerInit;

typedef enum stopMethod {
     NO_IMPROVING,    //stop after N nonImproving moves
     NUM_STEP,        //stop after N steps
} stopMethod;

/******************************STRUCTURES******************************/
//this is a minimal tabuList
typedef struct tabuList_s{
    move_t** tabu;
    int tabu_size;
    int *expireTime;
    int eTime;
}tabuList_t;


typedef struct termination_s{
    //stopping criteria after thresholdStop step
    int thresholdStop;
    int currentStep;
    //stopping criteria after noImprovingStop nonImproving Step
    int noImprovingStop;
    int noImprovingStep;

    stopMethod method;
}termination_t;







/******************************ALLOCATORS******************************/

//allocate a new tabu-list
tabuList_t *tabuList_new(int tabu_size,int expireTime);

//allocate termination_t
termination_t *termination_new(int,int,stopMethod);

//free
void termination_free(termination_t*);

void tabuList_free(tabuList_t *t);



/******************************FUNCTIONS******************************/

//resort the list if a move stored is used to forbid the movement
void sort_tabuList(int,tabuList_t *);

//check the A.C. before looking for an element in the tabu list
int check_AspirationCriteria(int best,neighbor_t *candidate);

//shift the elements in tabuList from a starting point
void shiftElement(int,tabuList_t*);

void shiftElementReverse(int start, tabuList_t *list);

//add a new move done in tabu list
void addMove_tabu(tabuList_t*,move_t*);

int lookup_move(tabuList_t *l,neighbor_t *cand,int best,solution_t *s, int* tabuHit);

//increment number of move and call check_TerminationCriteria
int increment_move(termination_t*,int);

//verify if is reached the termination criteria
int check_TerminationCriteria(termination_t*);

void printTabu(tabuList_t *l);

int lookClone(tabuList_t *l);


//TABU SEARCH FUNCS
//Launcher for Parametrized Versions
solution_t *tabuSearchP(solution_t *start,tabuListNum tln,int tabuSizeA,int tabuSizeB,int tabuSizeC,int expire,int step,int noImpr,cornerGenerator corner,edgeGenerator edge,innerGenerator inner,int seed, int type, int verb);

solution_t *tabuSearchSingleP(solution_t *start, int sizeA,int sizeB, int sizeC,int step,int noImpr,stopMethod sm,int expireTime,cornerGenerator cg,edgeGenerator eg,innerGenerator ig, int seed, int type, int verb);

solution_t *tabuSearchMultipleP(solution_t *start, int sizeA,int sizeB, int sizeC,int step,int noImpr,stopMethod sm,int expireTime,cornerGenerator cg,edgeGenerator eg,innerGenerator ig, int seed, int type, int verb);

solution_t* tabuSearchDefaultPreliminary(solution_t *start, int step, int noImpr, int verb);
solution_t* tabuSearchDefault(solution_t *start, int step, int noImpr, int seed, int verb);

void tabuList_flush(tabuList_t* l);
#endif // TABU_H_INCLUDED
