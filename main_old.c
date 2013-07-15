#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "error.h"
#include "solution.h"
#include "input.h"
#include "output.h"
#include "neigh_swap_rotate.h"
#include "tabu.h"
/*
int main (int argc, char** argv) {
     int n;
     solution_t *start=NULL;
     solution_t *curr=NULL;
     neigh_t* neighborhood_inner=NULL;
     neigh_t* neighborhood_edge=NULL;
     neigh_t* neighborhood_corner=NULL;
     neighbor_t* candidate=NULL;

     if (argc!=3) {
          print_err(WRONG_PARAM);
          return(FAILURE);
     }

     squares=read_input(argv[1], &n);

     start=solution_greedy(n);
     curr=start;

     neighborhood_inner=neighborhood_generation_inner(curr,neighborhood_inner);
     neighborhood_edge=neighborhood_generation_edge(curr,neighborhood_edge);
     neighborhood_corner=neighborhood_generation_corner(curr,neighborhood_corner);

/*
     //DEBUG
     candidate=neighborhood_best_neighbor(neighborhood_inner);
     neighbor_print(candidate);
     curr=neighbor_into_solution(curr,candidate,OVERWRITE);
*/
/*
     //DEBUG
     candidate=neighborhood_best_neighbor(neighborhood_edge);
     neighbor_print(candidate);
     curr=neighbor_into_solution(curr,candidate,OVERWRITE);
*/
/*
     //DEBUG
     candidate=neighborhood_best_neighbor(neighborhood_corner);
     neighbor_print(candidate);
     curr=neighbor_into_solution(curr,candidate,OVERWRITE);
*/
/*
     //DEBUG
     int cnt=0;
     candidate=neighborhood_best_neighbor(neighborhood_inner);
     //neighbor_print(candidate);
     while ( candidate!=NULL  ) {
               //neighbor_print(candidate);
               cnt++;
               neighbor_discard(candidate);
               candidate=neighborhood_best_neighbor(neighborhood_inner);
     }
     printf("%d\n",cnt); //35
*/

/*
     write_output(argv[2], start);

     solution_free(start);
     neighborhood_free(neighborhood_inner);
     neighborhood_free(neighborhood_edge);
     neighborhood_free(neighborhood_corner);

*/

    solution_t *tmp;
    tmp=NULL;

//tabuSearch(solution_t *start,int tabuSize,int step,int noImpr,stopMethod sm)

    tmp=tabuSearch(start,7,7,700,NO_IMPROVING);

   //tmp=tabuSearch(start,7,1000000,7,NUM_STEP);

   //tmp=tabuSearchAlternative(start,7,1000000,2,NUM_STEP);

   //tmp=tabuSearchAlternative(start,7,1000000,5000,NO_IMPROVING);


    write_output(argv[2], tmp);

     return(SUCCESS);
}


