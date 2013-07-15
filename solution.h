#ifndef SOLUTION_H_INCLUDED
#define SOLUTION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "error.h"

//PLEASE, DECLARE ALL POINTERS TO STRUCTURES EQUAL TO NULL
//OTHERWISE AN UNPREDICTABLE BEHAVIOUR MAY OCCUR
//PLEASE, DO THAT EVEN AFTER CALLING FREE METHODS

/******************************CONSTANTS******************************/
//colour number associated to grey or edge colour
#define GREY 0


/******************************ENUM******************************/
typedef enum square_type{
     CORNER,
     EDGE,
     INNER,
} square_type;

//colours are read in clockwise direction starting from the bottom
//DO NOT CHANGE THE ORDER
typedef enum dir {
     DOWN,
     LEFT,
     UP,
     RIGHT,
     NONE,
} dir;

//offset to sum to row and column to obtain the
//adjacent square in the solution in a given direction
//use: mat[r+ADJACENT[direction*2]][c+ADJACENT[direction*2+1]]
static const int ADJACENT[8] =
//   r,    c
{   1,    0,    //DOWN
     0,   -1,    //LEFT
     -1,   0,    //UP
     0,    1,    //RIGHT
};

//the order corresponds to the direction of the first grey triangle
//of the adjacent couple in the corner
//DO NOT CHANGE THE ORDER
typedef enum corner_dir {
     LOWER_LEFT,    //DOWN
     UPPER_LEFT,     //LEFT
     UPPER_RIGHT,   //UP
     LOWER_RIGHT,  //RIGHT
} corner_dir;



/******************************STRUCTURES******************************/
typedef struct square_s {
     int colour[4];
     square_type type;
} square_t;


//square in the solution
typedef struct piece_s {
     int square_index;
     int rotation;
} piece_t;


typedef struct solution_s {
     piece_t*** mat;
     int matches;
     int n;
} solution_t;


/******************************GLOBAL_VARS******************************/
square_t **squares;


/******************************DIRECTIONS******************************/
//CLOCKWISE ROTATION
//returns the colour index corresponding to direction after rotation
#define ROTATE(rotation,direction) (4-rotation+direction)%4
//(rotation+direction)%4 is ANTICLOCKWISE

#define OPPOSITE(direction) (direction+2)%4

//returns the edge of the piece in pos1 adjacent to the
//piece in pos2. NONE if they are not adjacent
dir is_adjacent(int pos1, int pos2, int n);

//returns the direction of the edge in a given position
//it is up to the user to give a valid position
dir direction_edge(int pos, int n);

//returns the direction of the corner in a given position
//it is up to the user to give a valid position
corner_dir direction_corner(int pos, int n);


/******************************SQUARE******************************/
square_t* square_new(int c_down, int c_left, int c_up, int c_right);

#define  square_free(square) free(square)

//returns an int array
#define square_get_colours(square) square->colour

//returns a square_type
#define square_get_type(square) square->type

//counts the number of grey colours
int square_grey(square_t*);

//automatically rotate the grey triangle in the direction of the edge given
//returns the rotation offset
int square_rotate_edge(square_t*,dir);

//automatically rotate the grey triangles to fit the given corner
//returns the rotation offset
int square_rotate_corner(square_t*,corner_dir);


/******************************PIECE******************************/
piece_t* piece_new(int square_index, int rotation);

#define piece_free(piece) free(piece)

#define piece_get_square_index(piece) piece->square_index

#define piece_get_rotation(piece) piece->rotation

#define PIECE_COLOUR(piece,direction) \
     squares[piece->square_index]->colour[ROTATE(piece->rotation,direction)]

#define piece_print(piece) printf("%3d",piece->square_index)

/******************************SOLUTION******************************/
//each piece has still to be allocated
solution_t* solution_new(int n);

void solution_free(solution_t*);

#define solution_get_matches(solution) solution->matches

#define solution_get_size(solution) solution->n

#define solution_get_piece(solution,position) solution->mat[position/solution->n][position%solution->n]

//user is in charge of checking src correct initialization
//dst will be allocated if equals to NULL
solution_t* solution_cpy(solution_t* dst, solution_t* src);

void solution_print(solution_t *s);

//return a fast and very simple feasible solution
solution_t* solution_greedy(int);

//shuffle the solution given
solution_t* solution_shuffle(solution_t* s, int seed);
solution_t* solution_shuffle2(solution_t* s, int seed);

//diversify the solution given (max 6 swap)
solution_t* solution_diversification(solution_t* s, int seed);

//print the list of the solution pieces
void solution_print_pieces(solution_t*,FILE*);

//calculate the maximum number of matches for a given
//solution dimension
#define solution_max_matches(n) (2*n*(n-1))

//counts matches in a solution
int solution_matches(solution_t*);

//count matches for a piece in a given position
//if NULL is passed, p will be put equal to the
//piece corresponding to position in the solution
int solution_piece_matches_corner(solution_t* s, piece_t* p, int position);

int solution_piece_matches_edge(solution_t* s, piece_t* p, int position);

int solution_piece_matches_inner(solution_t* s, piece_t* p, int position);

/******************************RANDOM _POSITION******************************/
#define INNER_RAND(pos,n) ( (pos=( rand()%(n*(n-1)-2-(n+1))+(n+1) )) %n==0 ) ?  ( pos+1 ) :  ( ( pos%n==n-1) ? ( pos-1 ) : (pos) )

#define EDGE_UP_RAND(pos,n) rand()%(n-2)+1
#define EDGE_DOWN_RAND(pos,n) rand()%(n*n-1-(n*(n-1)+1))+(n*(n-1)+1)
#define EDGE_LEFT_RAND(pos,n) (EDGE_UP_RAND(pos,n)%n)*n
#define EDGE_RIGHT_RAND(pos,n) (EDGE_UP_RAND(pos,n)%n)*n+n-1

#endif // SOLUTION_H_INCLUDED
