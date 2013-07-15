#ifndef NEIGH_SWAP_ROTATE_H_INCLUDED
#define NEIGH_SWAP_ROTATE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "solution.h"


/******************************CONSTANTS******************************/
#define NEIGH_CORNER_SIZE 3
//the 3 remaining corner without the UPPER_LEFT

#define NEIGH_EDGE_SIZE(n) ((4*(n-2))-1)
/*
     Swaps of an edge element with the edge element [0][1]       4*(n-2)
     The swap with the first element itself is not considered       -1
*/

#define NEIGH_INNER_SIZE(n) ((n-2)*(n-2)*4)
/*
     Swaps of an inner element with the inner element [1][1]       (n-2)^2
     multiplied for the four possible rotations of the element       *4
     swapped with the first one.
     The swap with the first element itself is considered to count
     the simple rotations of the element without the initial one.
     To preserve code semplicity, instead of subtract 1 to consider
     the initial rotation offset of the first element, the matches
     of the correspondant solution will be put to -1
*/

#define OVERWRITE 1
#define DUPLICATE 0



/******************************STRUCTURES******************************/
typedef struct move_s {
     int square_index;
     int pos;
     int rotation;
     square_type move_type;

          int inList; //indicates if it is in tabu List

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
#define move_get_square_index(move) move->square_index

//returns an int
#define move_get_position(move) move->pos

//returns an int
#define move_get_rotation(move) move->rotation

//returns a square_type
#define move_get_type(move) move->move_type

//the user is in charge of checkin moves correctness
int  move_cmp(move_t*, move_t*);

//the user has to provide a correct src
move_t* move_cpy(move_t* dst, move_t*src);

//for debugging
#define move_print(m) \
     printf("square_idx %d position %d rotation %d move_type %d\n",m->square_index,m->pos,m->rotation, m->move_type)

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

//generate a neighborhood swapping the element [1][1] with all the others
//elements and rotating them
neigh_t* neighborhood_generation_inner(solution_t* base, neigh_t* neigh);

//generate a neighborhood swapping the element [0][1] with all the others
//elements
neigh_t* neighborhood_generation_edge(solution_t* base, neigh_t* neigh);

//generate a neighborhood swapping the element [0][0] with all the others
//elements
neigh_t* neighborhood_generation_corner(solution_t* base, neigh_t* neigh);

//returns the neighbor in the neighborhood with the max number of matches
neighbor_t* neighborhood_best_neighbor(neigh_t*);


#endif // NEIGH_SWAP_ROTATE_H_INCLUDED
