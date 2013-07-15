#include "neigh_swap_rotate.h"


/******************************MOVE******************************/

move_t* move_new() {
     move_t *tmp;

     tmp=(move_t*)malloc(sizeof(move_t));
     if (tmp==NULL) {
          print_err(MEMORY_ALLOC);
          exit(1);
     }



             tmp->inList=0;


     return(tmp);
}


int  move_cmp(move_t* a, move_t* b) {
     return (!(a->pos==b->pos && a->rotation==b->rotation && a->square_index==b->square_index));
}


move_t* move_cpy(move_t* dst,move_t* src) {

     if (dst==NULL)
          dst=move_new();
     dst->pos=src->pos;
     dst->rotation=src->rotation;
     dst->square_index=src->square_index;
     dst->move_type=src->move_type;
     return(dst);
}

move_t* move_reverse(move_t* dst, move_t* src) {
     if (dst==NULL)
          dst=move_new();
     dst->pos=src->pos;
     dst->rotation=src->rotation;
     dst->square_index=src->square_index;
     dst->move_type=src->move_type;
}

move_t* move_reverse(move_t* dst, move_t* src);



int move_matches_diff_inner(solution_t* s, move_t* move) {
     int tot;
     piece_t *p1, *p2, *p1_old, *p2_old;
     dir d;

     p1=s->mat[1][1];
     p2=piece_new(move->square_index,move->rotation);

     //rotation of the first piece
     if (s->n+1==move->pos) {
          tot=s->matches - solution_piece_matches_inner(s,NULL,s->n+1) + solution_piece_matches_inner(s, p2, s->n+1);
     } else {
          //non adjacent swap
          //old pieces matches are subtracted
          tot=s->matches - solution_piece_matches_inner(s,NULL,move->pos) - solution_piece_matches_inner(s,NULL,s->n+1);
          //new pieces matches are added
          tot+=solution_piece_matches_inner(s, p2, s->n+1) + solution_piece_matches_inner(s,p1, move->pos);

          d=is_adjacent(s->n+1,move->pos,s->n);
          //adjacent swap
          if (d!=NONE) {
               p1_old=p1;
               p2_old=s->mat[(move->pos)/(s->n)][(move->pos)%(s->n)];
               if ( PIECE_COLOUR(p1_old,d)==PIECE_COLOUR(p2_old,OPPOSITE(d)) )
                   //the match has been subtracted twice
                   tot++;
               //to avoid counting matches with the previous version of themselves
               if ( PIECE_COLOUR(p2,d)==PIECE_COLOUR(p2_old,OPPOSITE(d)) )
                    tot--;
               if ( PIECE_COLOUR(p1,OPPOSITE(d))==PIECE_COLOUR(p1_old,d) )
                    tot--;
               //to count the match between the new versions
               if ( PIECE_COLOUR(p1,OPPOSITE(d))==PIECE_COLOUR(p2,d) )
                    tot++;
          }
     }
     //piece_free(p1); //placeholder for enhanced versions
     piece_free(p2);
     return(tot);
}


int move_matches_diff_edge(solution_t* s, move_t* move) {
     int tot;
     piece_t *p1, *p2, *p1_old, *p2_old;
     dir d;

     d=direction_edge(move->pos,s->n);
     p1=piece_new(s->mat[0][1]->square_index,square_rotate_edge(squares[s->mat[0][1]->square_index],d));
     p2=piece_new(move->square_index,move->rotation);


     //non adjacent swap
     //old pieces matches are subtracted
     tot=s->matches - solution_piece_matches_edge(s,NULL,move->pos) - solution_piece_matches_edge(s,NULL,1);
     //new pieces matches are added
     tot+=solution_piece_matches_edge(s,p2,1) + solution_piece_matches_edge(s,p1,move->pos);

     d=is_adjacent(1,move->pos,s->n);
     //adjacent swap
     if (d!=NONE) {
          p1_old=s->mat[0][1];
          p2_old=s->mat[(move->pos)/(s->n)][(move->pos)%(s->n)];
          if ( PIECE_COLOUR(p1_old,d)==PIECE_COLOUR(p2_old,OPPOSITE(d)) )
              //the match has been subtracted twice
              tot++;
          //to avoid counting matches with the previous version of themselves
          if ( PIECE_COLOUR(p2,d)==PIECE_COLOUR(p2_old,OPPOSITE(d)) )
               tot--;
          if ( PIECE_COLOUR(p1,OPPOSITE(d))==PIECE_COLOUR(p1_old,d) )
               tot--;
          //to count the match between the new versions
          if ( PIECE_COLOUR(p1,OPPOSITE(d))==PIECE_COLOUR(p2,d) )
               tot++;
     }
     piece_free(p1);
     piece_free(p2);
     return(tot);
}


int move_matches_diff_corner(solution_t* s, move_t* move) {
     int tot;
     piece_t *p1, *p2;
     corner_dir d;

     d=direction_corner(move->pos,s->n);
     p1=piece_new(s->mat[0][0]->square_index,square_rotate_corner(squares[s->mat[0][0]->square_index],d));
     p2=piece_new(move->square_index,move->rotation);


     //always non adjacent swap
     //old pieces matches are subtracted
     tot=s->matches - solution_piece_matches_corner(s,NULL,move->pos) - solution_piece_matches_corner(s,NULL,0);
     //new pieces matches are added
     tot+=solution_piece_matches_corner(s,p2,0) + solution_piece_matches_corner(s,p1,move->pos);
     piece_free(p1);
     piece_free(p2);
     return(tot);
}



/******************************NEIGHBOR******************************/

neighbor_t* neighbor_new() {
     neighbor_t* tmp;

     tmp=(neighbor_t*)malloc(sizeof(neighbor_t));
     if (tmp==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
     }
     tmp->move=move_new();
     return(tmp);
}



solution_t* neighbor_into_solution(solution_t* s, neighbor_t* n, int write_mode) {
     solution_t* tmp=NULL;
     dir d;
     corner_dir cd;

     if (write_mode==DUPLICATE)
          tmp=solution_cpy(tmp,s);
     else
          tmp=s;

     tmp->matches=n->matches;
     piece_free(tmp->mat[n->move->pos/tmp->n][n->move->pos%tmp->n]);
     if (n->move->move_type==INNER) {
          tmp->mat[n->move->pos/tmp->n][n->move->pos%tmp->n]=tmp->mat[1][1];
          tmp->mat[1][1]=piece_new(n->move->square_index,n->move->rotation);
     } else if (n->move->move_type==EDGE) {
          d=direction_edge(n->move->pos,s->n);
          tmp->mat[0][1]->rotation=square_rotate_edge(squares[tmp->mat[0][1]->square_index],d);
          tmp->mat[n->move->pos/tmp->n][n->move->pos%tmp->n]=tmp->mat[0][1];
          tmp->mat[0][1]=piece_new(n->move->square_index,n->move->rotation);
     } else {
          cd=direction_edge(n->move->pos,s->n);
          tmp->mat[0][0]->rotation=square_rotate_corner(squares[tmp->mat[0][0]->square_index],cd);
          tmp->mat[n->move->pos/tmp->n][n->move->pos%tmp->n]=tmp->mat[0][0];
          tmp->mat[0][0]=piece_new(n->move->square_index,n->move->rotation);
     }
     return(s);
}



/******************************NEIGHBORHOOD******************************/

neigh_t* neighborhood_new(int neigh_size) {
     neigh_t* neigh;
     int i;

     neigh=(neigh_t*)malloc(sizeof(neigh_t));
     if (neigh==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
     }
     neigh->size=neigh_size;
     neigh->neighbors=(neighbor_t**)malloc(neigh_size*sizeof(neighbor_t*));
     if (neigh->neighbors==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
     }
     for (i=0;i<neigh->size;i++) {
          neigh->neighbors[i]=neighbor_new();
     }
     return(neigh);
}


void neighborhood_free(neigh_t* n) {
     int i;

     for (i=0;i<n->size;i++) {
          neighbor_free(n->neighbors[i]);
     }
     free(n->neighbors);
}


neigh_t* neighborhood_generation_inner(solution_t* base, neigh_t* neigh) {
     int i, j, k=0, num;

     if (neigh==NULL) {
          neigh=neighborhood_new(NEIGH_INNER_SIZE(base->n));
     } else if (neigh->size!=NEIGH_INNER_SIZE(base->n)) {
          neighborhood_free(neigh);
          neigh=neighborhood_new(NEIGH_INNER_SIZE(base->n));
     }
     num=base->n-1;
     for (i=1; i<num; i++) {
          for (j=1; j<num; j++) {
               if (i==1 && j==1) {
                    //code replication to enhance speed, avoiding to check the inner if cluase
                    //for each solution in the neighborhood
                    do {
                         neigh->neighbors[k]->move->pos=i*(base->n)+j;
                         neigh->neighbors[k]->move->square_index=base->mat[i][j]->square_index;
                         neigh->neighbors[k]->move->rotation=k%4;
                         neigh->neighbors[k]->move->move_type=INNER;
                         //DEBUG
                         //move_print(neigh->neighbors[k]->move);
                         if (neigh->neighbors[k]->move->rotation==base->mat [i][j]->rotation)
                              neigh->neighbors[k]->matches=-1;
                         else
                              neigh->neighbors[k]->matches=move_matches_diff_inner(base,neigh->neighbors[k]->move);
                         //DEBUG
                         //neighbor_print(neigh->neighbors[k]);
                         k++;
                    } while (k%4!=0);
               } else {
                    do {
                         neigh->neighbors[k]->move->pos=i*(base->n)+j;
                         neigh->neighbors[k]->move->square_index=base->mat[i][j]->square_index;
                         neigh->neighbors[k]->move->rotation=k%4;
                         neigh->neighbors[k]->move->move_type=INNER;
                         //DEBUG
                         //move_print(neigh->neighbors[k]->move);
                         neigh->neighbors[k]->matches=move_matches_diff_inner(base,neigh->neighbors[k]->move);
                         //DEBUG
                         //neighbor_print(neigh->neighbors[k]);
                         k++;
                    } while (k%4!=0);
               }
          }
     }

/*
     //DEBUG
     neigh->neighbors[4]->matches=move_matches_diff_inner(base,neigh->neighbors[4]->move);
     neighbor_print(neigh->neighbors[4]);
     neigh->neighbors[5]->matches=move_matches_diff_inner(base,neigh->neighbors[5]->move);
     neighbor_print(neigh->neighbors[5]);
*/
     return(neigh);
}


neigh_t* neighborhood_generation_edge(solution_t* base, neigh_t* neigh) {
     int i, k=0, num;


     if (neigh==NULL) {
          neigh=neighborhood_new(NEIGH_EDGE_SIZE(base->n));
     } else if (neigh->size!=NEIGH_EDGE_SIZE(base->n)) {
          neighborhood_free(neigh);
          neigh=neighborhood_new(NEIGH_EDGE_SIZE(base->n));
     }
     num=base->n-1;
     for (i=2; i<num; i++) {
          neigh->neighbors[k]->move->pos=i;
          neigh->neighbors[k]->move->square_index=base->mat[0][i]->square_index;
          neigh->neighbors[k]->move->rotation=square_rotate_edge(squares[base->mat[0][i]->square_index],UP);
          neigh->neighbors[k]->move->move_type=EDGE;
          //DEBUG
          //move_print(neigh->neighbors[k]->move);
          neigh->neighbors[k]->matches=move_matches_diff_edge(base,neigh->neighbors[k]->move);
          //DEBUG
          //neighbor_print(neigh->neighbors[k]);
          k++;
     }
     for (i=1; i<num; i++) {
          neigh->neighbors[k]->move->pos=i*(base->n);
          neigh->neighbors[k]->move->square_index=base->mat[i][0]->square_index;
          neigh->neighbors[k]->move->rotation=square_rotate_edge(squares[base->mat[i][0]->square_index],UP);
          neigh->neighbors[k]->move->move_type=EDGE;
          //DEBUG
          //move_print(neigh->neighbors[k]->move);
          neigh->neighbors[k]->matches=move_matches_diff_edge(base,neigh->neighbors[k]->move);
          //DEBUG
          //neighbor_print(neigh->neighbors[k]);
          k++;
     }
     for (i=1; i<num; i++) {
          neigh->neighbors[k]->move->pos=i*(base->n)+(base->n-1);
          neigh->neighbors[k]->move->square_index=base->mat[i][base->n-1]->square_index;
          neigh->neighbors[k]->move->rotation=square_rotate_edge(squares[base->mat[i][base->n-1]->square_index],UP);
          neigh->neighbors[k]->move->move_type=EDGE;
          //DEBUG
          //move_print(neigh->neighbors[k]->move);
          neigh->neighbors[k]->matches=move_matches_diff_edge(base,neigh->neighbors[k]->move);
          //DEBUG
          //neighbor_print(neigh->neighbors[k]);
          k++;
     }
     for (i=1; i<num; i++) {
          neigh->neighbors[k]->move->pos=(base->n-1)*(base->n)+i;
          neigh->neighbors[k]->move->square_index=base->mat[base->n-1][i]->square_index;
          neigh->neighbors[k]->move->rotation=square_rotate_edge(squares[base->mat[base->n-1][i]->square_index],UP);
          neigh->neighbors[k]->move->move_type=EDGE;
          //DEBUG
          //move_print(neigh->neighbors[k]->move);
          neigh->neighbors[k]->matches=move_matches_diff_edge(base,neigh->neighbors[k]->move);
          //DEBUG
          //neighbor_print(neigh->neighbors[k]);
          k++;
     }

/*
     //DEBUG
     printf("k %d\n",k);
     neigh->neighbors[4]->matches=move_matches_diff_inner(base,neigh->neighbors[4]->move);
     neighbor_print(neigh->neighbors[4]);
     neigh->neighbors[5]->matches=move_matches_diff_inner(base,neigh->neighbors[5]->move);
     neighbor_print(neigh->neighbors[5]);
*/
     return(neigh);
}


neigh_t* neighborhood_generation_corner(solution_t* base, neigh_t* neigh) {
     int k=0;


     if (neigh==NULL) {
          neigh=neighborhood_new(NEIGH_CORNER_SIZE);
     } else if (neigh->size!=NEIGH_CORNER_SIZE) {
          neighborhood_free(neigh);
          neigh=neighborhood_new(NEIGH_CORNER_SIZE);
     }

     //UPPER-RIGHT
     neigh->neighbors[k]->move->pos=base->n-1;
     neigh->neighbors[k]->move->square_index=base->mat[0][base->n-1]->square_index;
     neigh->neighbors[k]->move->rotation=square_rotate_corner(squares[base->mat[0][base->n-1]->square_index],UPPER_LEFT);
     neigh->neighbors[k]->move->move_type=CORNER;
     //DEBUG
     //move_print(neigh->neighbors[k]->move);
     neigh->neighbors[k]->matches=move_matches_diff_corner(base,neigh->neighbors[k]->move);
     //DEBUG
     //neighbor_print(neigh->neighbors[k]);
     k++;

     //LOWER-LEFT
     neigh->neighbors[k]->move->pos=(base->n)*(base->n-1);
     neigh->neighbors[k]->move->square_index=base->mat[base->n-1][0]->square_index;
     neigh->neighbors[k]->move->rotation=square_rotate_corner(squares[base->mat[base->n-1][0]->square_index],UPPER_LEFT);
     neigh->neighbors[k]->move->move_type=CORNER;
     //DEBUG
     //move_print(neigh->neighbors[k]->move);
     neigh->neighbors[k]->matches=move_matches_diff_corner(base,neigh->neighbors[k]->move);
     //DEBUG
     //neighbor_print(neigh->neighbors[k]);
     k++;

     //LOWER-RIGHT
     neigh->neighbors[k]->move->pos=(base->n)*(base->n)-1;
     neigh->neighbors[k]->move->square_index=base->mat[base->n-1][base->n-1]->square_index;
     neigh->neighbors[k]->move->rotation=square_rotate_corner(squares[base->mat[base->n-1][base->n-1]->square_index],UPPER_LEFT);
     neigh->neighbors[k]->move->move_type=CORNER;
     //DEBUG
     //move_print(neigh->neighbors[k]->move);
     neigh->neighbors[k]->matches=move_matches_diff_corner(base,neigh->neighbors[k]->move);
     //DEBUG
     //neighbor_print(neigh->neighbors[k]);
     k++;

/*
     //DEBUG
     printf("k %d\n",k);
*/
     return(neigh);
}


neighbor_t* neighborhood_best_neighbor(neigh_t* n) {
     int i, found=0;
     neighbor_t* max=NULL;

     for(i=0;i<n->size;i++) {

         //modified


          if (found==0 && n->neighbors[i]->matches>-1 && n->neighbors[i]->move->inList==0) {
               max=n->neighbors[i];
               found=1;
          } else if (found==1 && neighbor_cmp_matches(max,n->neighbors[i])<0) {
               max=n->neighbors[i];
          }
     }
     return(max);
}
