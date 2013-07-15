#ifndef NEIGH_SWAP_ROTATE_H_INCLUDED
#define NEIGH_SWAP_ROTATE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "error.h"
#include "solution.h"

//PLEASE, DECLARE ALL POINTERS TO STRUCTURES EQUAL TO NULL
//OTHERWISE AN UNPREDICTABLE BEHAVIOUR MAY OCCUR
//PLEASE, DO THAT EVEN AFTER CALLING FREE METHODS

/******************************CONSTANTS******************************/
#define NEIGH_CORNER_SIZE (4-1)
/*
     Swaps of a corner element with another corner element      4
     The swap with the element itself is not considered               -1
*/


#define NEIGH_EDGE_SIZE(n) (4*(n-2)-1)
/*
     Swaps of an edge element with another edge element          4*(n-2)
     The swap with the element itself is not considered               -1
*/

#define NEIGH_INNER_SIZE(n) (((n-2)*(n-2)*4*4)-13)
/*
     Swaps of an inner element with another element                   (n-2)^2
     multiplied for the four possible rotations of the element       *4
     and for the four possible rotation of the other element         *4
     The swap with the first element itself is considered to count
     the simple rotations of the element without the initial one.
     The initial rotation offset of the first element and the fact
     that the first and second element are the same lead to
     not valid moves                                                                           -13
*/

#define OVERWRITE 1
#define DUPLICATE 0



/******************************STRUCTURES******************************/
//after the move: piece1 -> pos1,rot1 and piece2 -> pos2, rot2
typedef struct move_s {
     int square_index1;
     int pos1;
     int rotation1;
     int square_index2;
     int pos2;
     int rotation2;
     square_type move_type;
} move_t;

typedef struct neighbor_s {
     move_t* move;
     int matches;
} neighbor_t;

typedef struct neigh_s {
     neighbor_t** neighbors;
     int size;
} neigh_t;



/******************************MOVE******************************/
move_t* move_new();

#define move_free(move) free(move)

//returns an int
#define move_get_square_index1(move) move->square_index1

//returns an int
#define move_get_position1(move) move->pos1

//returns an int
#define move_get_rotation1(move) move->rotation1

//returns an int
#define move_get_square_index2(move) move->square_index2

//returns an int
#define move_get_position2(move) move->pos2

//returns an int
#define move_get_rotation2(move) move->rotation2

//returns a square_type
#define move_get_type(move) move->move_type

//the user is in charge of checking moves correctness
int  move_cmp(move_t*, move_t*);

//the user has to provide a correct src
move_t* move_cpy(move_t* dst, move_t*src);

//returns the reverse of the given move. Designed expressly for tabu serach.
//the user has to provide a correct src and s
move_t* move_reverse(move_t* dst, move_t* src, solution_t* s);

//for debugging
#define move_print(m) \
     printf("square_idx1 %d position1 %d rotation1 %d\n",m->square_index1,m->pos1,m->rotation1); \
     printf("square_idx2 %d position2 %d rotation2 %d\n",m->square_index2,m->pos2,m->rotation2); \
     printf("move_type %d\n", m->move_type)

//functions that count the matches of the neighbor solution in a differential way
int move_matches_diff_inner(solution_t* s, move_t* move);

int move_matches_diff_edge(solution_t* s, move_t* move);

int move_matches_diff_corner(solution_t* s, move_t* move);


/******************************NEIGHBOR******************************/
neighbor_t* neighbor_new();

#define neighbor_free(neighbor) move_free(neighbor->move); free(neighbor)

//returns a move_t
#define neighbor_get_move(neighbor) neighbor->move

//returns an int
#define neighbor_get_matches(neighbor) neighbor->matches

//for debugging
#define neighbor_print(n) \
     move_print(n->move); printf("matches %d\n",n->matches)

//Returns n<-1 if a<b, n=0 if a==b, n>1 if a>b
//the user has to check the neighbors correct allocation
//and matches assignment
#define neighbor_cmp_matches(a,b) (a->matches-b->matches)

//Discard the selected neighbor from the neighborhood
#define neighbor_discard(neighbor) neighbor->matches=-1;

//transform a neighbor in a solution applying the move
//if write_mode=OVERWRITE the solution passed as first
//argument is changed, if write_mode=DUPLICATE a new
//solution will be created without changing the previous one
//the user is in charge of cheking the right allocation of all arguments
solution_t* neighbor_into_solution(solution_t* s, neighbor_t* n, int write_mode);


/******************************NEIGHBORHOOD******************************/
neigh_t* neighborhood_new(int neigh_size);

void neighborhood_free(neigh_t*);

//returns an int
#define neigborhood_get_size(neighborhood) neighborhood->size

//generate a neighborhood swapping the inner element in the position swap_pos with all the others
//elements and rotating them. The user has the duty to pass a valid position.
neigh_t* neighborhood_generation_inner(solution_t* base, neigh_t* neigh, int swap_pos);

//generate a neighborhood swapping the edge element in the position swap_pos with all the others
//elements. The user has the duty to pass a valid position.
neigh_t* neighborhood_generation_edge(solution_t* base, neigh_t* neigh, int swap_pos);

//generate a neighborhood swapping the corner element in the position swap_pos with all the others
//elements. The user has the duty to pass a valid position.
neigh_t* neighborhood_generation_corner(solution_t* base, neigh_t* neigh, int swap_pos);

//returns the neighbor in the neighborhood with the max number of matches
neighbor_t* neighborhood_best_neighbor(neigh_t*);


/******************************SWAP_POSITION GENERATORS******************************/

//INITIALIZATION

#define SWAP_POS_CORNER_FIRST_UPPERLEFT(n) (0)
#define SWAP_POS_CORNER_FIRST_UPPERRIGHT(n) (n-1)
#define SWAP_POS_CORNER_FIRST_LOWERRIGHT(n) (n*(n-1)+(n+1))
#define SWAP_POS_CORNER_FIRST_LOWERLEFT(n) (n*(n-1))

#define SWAP_POS_EDGE_FIRST_UP(n) 1
#define SWAP_POS_EDGE_FIRST_LEFT(n) n
#define SWAP_POS_EDGE_FIRST_DOWN(n) n*(n-1)+1
#define SWAP_POS_EDGE_FIRST_RIGHT(n) 2*n+(n-1)

#define SWAP_POS_INNER_FIRST_UPPERLEFT(n) (n+1)
#define SWAP_POS_INNER_FIRST_UPPERRIGHT(n) (n*2-2)
#define SWAP_POS_INNER_FIRST_LOWERRIGHT(n) (n*(n-1)-2)
#define SWAP_POS_INNER_FIRST_LOWERLEFT(n) (n*(n-2)+1)

#define SWAP_POS_EDGE_MEAN_FIRST(n) (n/2)

//to use rand
#define SWAP_POS_RAND_INIT() srand(time(NULL))

//GENERATION

#define SWAP_POS_OPPOSITE(pos,n) ((n*n-1)-pos)

//IT DOESN'T WORK!!!!!
#define SWAP_POS_INNER_MEAN_OPPOSITE(pos1,pos2,n) \
( (((pos1+pos2)/2)%n==0) ? ( (((pos1+pos2)/2)%n==n-1) ? (SWAP_POS_OPPOSITE(((pos1+pos2)/2-1),n)) : (SWAP_POS_OPPOSITE(((pos1+pos2)/2+1),n)) ) : (SWAP_POS_OPPOSITE(((pos1+pos2)/2),n)))

//4 step cycle: the order is clockwise
#define SWAP_POS_CORNER_NEXT(pos,n) \
(pos==0) ? (n-1) : ( (pos==n-1) ? (n*(n-1)) : ( (pos==n*(n-1)) ? (n*n-1) : (0) ) )

//4 step cycle: the order is clockwise
//1->9->23->5
#define SWAP_POS_EDGE_NEXT(pos,n) \
(pos/n==0) ? (pos*n+(n-1)) : ( (pos%n==n-1) ? (n*(n-1)+(pos/n)) : ( (pos/n==n-1) ? (n*(pos%n)) : (pos/n) ) )


//4 step cycle: the order is anticlockwise
#define SWAP_POS_EDGE_NEXT_ANTI(pos,n) \
(pos/n==0) ? (pos*n) : ( (pos%n==0) ? (n*(n-1)+(pos/n)) : ( (pos/n==n-1) ? (n*(pos%n)+(n-1)) : (pos/n) ) )

//4 step cycle: the order is clockwise
//upper-left->upper-right->lower-right->lower-left
#define SWAP_POS_INNER_NEXT(pos,n) \
(pos==n+1) ? (n*2-2) : ( (pos==n*2-2) ? (n*(n-1)-2) : ( (pos==n*(n-1)-2) ? (n*(n-2)+1) : (n+1) ) )

//randomization
#define SWAP_POS_INNER_RAND(pos,n) ( (pos=( rand()%(n*(n-1)-2-(n+1))+(n+1) )) %n==0 ) ?  ( pos+1 ) :  ( ( pos%n==n-1) ? ( pos-1 ) : (pos) )


#endif // NEIGH_SWAP_ROTATE_H_INCLUDED
