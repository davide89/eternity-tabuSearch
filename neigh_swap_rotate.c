#include "neigh_swap_rotate.h"


/******************************MOVE******************************/

move_t* move_new() {
     move_t *tmp;

     tmp=(move_t*)malloc(sizeof(move_t));
     if (tmp==NULL) {
          print_err(MEMORY_ALLOC);
          exit(1);
     }
     return(tmp);
}


int  move_cmp(move_t* a, move_t* b) {
     return ( !( (a->pos1==b->pos1 && a->rotation1==b->rotation1 && a->square_index1==b->square_index1
               && a->pos2==b->pos2 && a->rotation2==b->rotation2 && a->square_index2==b->square_index2)
               || (a->pos1==b->pos2 && a->rotation1==b->rotation2 && a->square_index1==b->square_index2
               && a->pos2==b->pos1 && a->rotation2==b->rotation1 && a->square_index2==b->square_index1) ) );
}


move_t* move_cpy(move_t* dst,move_t* src) {

     if (dst==NULL)
          dst=move_new();
     dst->pos1=src->pos1;
     dst->rotation1=src->rotation1;
     dst->square_index1=src->square_index1;
     dst->pos2=src->pos2;
     dst->rotation2=src->rotation2;
     dst->square_index2=src->square_index2;
     dst->move_type=src->move_type;
     return(dst);
}


move_t* move_reverse(move_t* dst, move_t* src, solution_t* s) {

     if (dst==NULL)
          dst=move_new();
     dst->pos1=src->pos2;
     dst->rotation1=piece_get_rotation(solution_get_piece(s,src->pos2));
     dst->square_index1=src->square_index1;
     dst->pos2=src->pos1;
     dst->rotation2=piece_get_rotation(solution_get_piece(s,src->pos1));
     dst->square_index2=src->square_index2;
     dst->move_type=src->move_type;
     return(dst);
}


int move_matches_diff_inner(solution_t* s, move_t* move) {
     int tot;
     piece_t *p1, *p2, *p1_old, *p2_old;
     dir d;

     p1=piece_new(move->square_index1,move->rotation1);
     p2=piece_new(move->square_index2,move->rotation2);

     //rotation of the same piece
     //p1==p2 if it is the same piece
     if (move->pos1==move->pos2) {
          tot=s->matches - solution_piece_matches_inner(s,NULL,move->pos1)
               + solution_piece_matches_inner(s, p1, move->pos1);
     } else {
          //non adjacent swap
          //old pieces matches are subtracted
          tot=s->matches - solution_piece_matches_inner(s,NULL,move->pos1) - solution_piece_matches_inner(s,NULL,move->pos2);
          //new pieces matches are added
          tot+=solution_piece_matches_inner(s, p1, move->pos1) + solution_piece_matches_inner(s,p2, move->pos2);

          d=is_adjacent(move->pos2,move->pos1,s->n); //swap_pos->other_pos
          //adjacent swap
          if (d!=NONE) {
               p1_old=s->mat[(move->pos2)/(s->n)][(move->pos2)%(s->n)];
               p2_old=s->mat[(move->pos1)/(s->n)][(move->pos1)%(s->n)];
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
     piece_free(p1);
     piece_free(p2);
     return(tot);
}


int move_matches_diff_edge(solution_t* s, move_t* move) {
     int tot;
     piece_t *p1, *p2, *p1_old, *p2_old;
     dir d;

     //new pieces are already rotated in the right way
     p1=piece_new(move->square_index1,move->rotation1);
     p2=piece_new(move->square_index2,move->rotation2);


     //non adjacent swap
     //old pieces matches are subtracted
     tot=s->matches - solution_piece_matches_edge(s,NULL,move->pos1) - solution_piece_matches_edge(s,NULL,move->pos2);
     //new pieces matches are added
     tot+=solution_piece_matches_edge(s,p1,move->pos1) + solution_piece_matches_edge(s,p2,move->pos2);

     d=is_adjacent(move->pos2,move->pos1,s->n); //swap_pos->other_pos
     //adjacent swap
     if (d!=NONE) {
          p1_old=s->mat[(move->pos2)/(s->n)][(move->pos2)%(s->n)];
          p2_old=s->mat[(move->pos1)/(s->n)][(move->pos1)%(s->n)];
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

     p1=piece_new(move->square_index1,move->rotation1);
     p2=piece_new(move->square_index2,move->rotation2);


     //always non adjacent swap
     //old pieces matches are subtracted
     tot=s->matches - solution_piece_matches_corner(s,NULL,move->pos1) - solution_piece_matches_corner(s,NULL,move->pos2);
     //new pieces matches are added
     tot+=solution_piece_matches_corner(s,p1,move->pos1) + solution_piece_matches_corner(s,p2,move->pos2);
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

     int r1,r2,c1,c2;

     if (write_mode==DUPLICATE)
          tmp=solution_cpy(tmp,s);
     else
          tmp=s;

     r1=n->move->pos1/tmp->n;
     r2=n->move->pos2/tmp->n;
     c1=n->move->pos1%tmp->n;
     c2=n->move->pos2%tmp->n;
     tmp->matches=n->matches;
     piece_free(tmp->mat[r1][c1]);
     tmp->mat[r1][c1]=NULL;
     tmp->mat[r1][c1]=piece_new(n->move->square_index1,n->move->rotation1);
     if(n->move->pos1!=n->move->pos2) {
          piece_free(tmp->mat[r2][c2]);
          tmp->mat[r2][c2]=NULL;
          tmp->mat[r2][c2]=piece_new(n->move->square_index2,n->move->rotation2);
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
     free(n);
}


neigh_t* neighborhood_generation_inner(solution_t* base, neigh_t* neigh, int swap_pos) {
     int i, j, k=0, z=0, num, r,c;

     if (neigh==NULL) {
          neigh=neighborhood_new(NEIGH_INNER_SIZE(base->n));
     } else if (neigh->size!=NEIGH_INNER_SIZE(base->n)) {
          neighborhood_free(neigh);
          neigh=NULL;
          neigh=neighborhood_new(NEIGH_INNER_SIZE(base->n));
     }
     num=base->n-1;
     r=swap_pos/base->n;
     c=swap_pos%base->n;
     for (i=1; i<num; i++) {
          for (j=1; j<num; j++) {
               if (i==r && j==c) {
                    //code replication to enhance speed, avoiding to check the inner if cluase
                    //for each solution in the neighborhood
                    do {
                         if ( ((z%16)/4)==0 && (z%4)!=base->mat [r][c]->rotation) {
                              neigh->neighbors[k]->move->pos1=swap_pos;
                              neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
                              neigh->neighbors[k]->move->rotation1=z%4;
                              neigh->neighbors[k]->move->pos2=swap_pos;
                              neigh->neighbors[k]->move->square_index2=base->mat[r][c]->square_index;
                              neigh->neighbors[k]->move->rotation2=z%4;
                              neigh->neighbors[k]->move->move_type=INNER;
                              //DEBUG
                              //move_print(neigh->neighbors[k]->move);
                              neigh->neighbors[k]->matches=move_matches_diff_inner(base,neigh->neighbors[k]->move);
                              //DEBUG
                              //neighbor_print(neigh->neighbors[k]);
                              k++;
                         }
                         z++;
                    } while (z%16!=0);
               } else {
                    do {
                         neigh->neighbors[k]->move->pos1=i*(base->n)+j;
                         neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
                         neigh->neighbors[k]->move->rotation1=(z%16)/4;
                         neigh->neighbors[k]->move->pos2=swap_pos;
                         neigh->neighbors[k]->move->square_index2=base->mat[i][j]->square_index;
                         neigh->neighbors[k]->move->rotation2=(z%16)%4;
                         neigh->neighbors[k]->move->move_type=INNER;
                         //DEBUG
                         //move_print(neigh->neighbors[k]->move);
                         neigh->neighbors[k]->matches=move_matches_diff_inner(base,neigh->neighbors[k]->move);
                         //DEBUG
                         //neighbor_print(neigh->neighbors[k]);
                         k++;
                         z++;
                    } while (z%16!=0);
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
     //printf("k %d  z %d\n",k,z);
     return(neigh);
}

/*VERSION WITH MATCHES=-1 INSTEAD OF SIZE-13 (+MEMORY 13 WASTE, - CPU Z INCREASE)
neigh_t* neighborhood_generation_inner(solution_t* base, neigh_t* neigh, int swap_pos) {
     int i, j, k=0, z=0, num, r,c;

     if (neigh==NULL) {
          neigh=neighborhood_new(NEIGH_INNER_SIZE(base->n));
     } else if (neigh->size!=NEIGH_INNER_SIZE(base->n)) {
          neighborhood_free(neigh);
          neigh=NULL;
          neigh=neighborhood_new(NEIGH_INNER_SIZE(base->n));
     }
     num=base->n-1;
     r=swap_pos/base->n;
     c=swap_pos%base->n;
     for (i=1; i<num; i++) {
          for (j=1; j<num; j++) {
               if (i==r && j==c) {
                    //code replication to enhance speed, avoiding to check the inner if cluase
                    //for each solution in the neighborhood
                    do {
                              neigh->neighbors[k]->move->pos1=swap_pos;
                              neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
                              neigh->neighbors[k]->move->rotation1=k%4;
                              neigh->neighbors[k]->move->pos2=swap_pos;
                              neigh->neighbors[k]->move->square_index2=base->mat[r][c]->square_index;
                              neigh->neighbors[k]->move->rotation2=k%4;
                              neigh->neighbors[k]->move->move_type=INNER;
                              //DEBUG
                              //move_print(neigh->neighbors[k]->move);
                              if (neigh->neighbors[k]->move->rotation1==base->mat[r][c]->rotation)
                                   neigh->neighbors[k]->matches=-1;
                              else
                                   neigh->neighbors[k]->matches=move_matches_diff_inner(base,neigh->neighbors[k]->move);
                              //DEBUG
                              //neighbor_print(neigh->neighbors[k]);
                              k++;
                    } while (k%16!=0);
               } else {
                    do {
                         neigh->neighbors[k]->move->pos1=i*(base->n)+j;
                         neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
                         neigh->neighbors[k]->move->rotation1=(k%16)/4;
                         neigh->neighbors[k]->move->pos2=swap_pos;
                         neigh->neighbors[k]->move->square_index2=base->mat[i][j]->square_index;
                         neigh->neighbors[k]->move->rotation2=(k%16)%4;
                         neigh->neighbors[k]->move->move_type=INNER;
                         //DEBUG
                         //move_print(neigh->neighbors[k]->move);
                         neigh->neighbors[k]->matches=move_matches_diff_inner(base,neigh->neighbors[k]->move);
                         //DEBUG
                         //neighbor_print(neigh->neighbors[k]);
                         k++;
                    } while (k%16!=0);
               }
          }
     }
     //printf("k %d\n",k);

     return(neigh);
}

*/


neigh_t* neighborhood_generation_edge(solution_t* base, neigh_t* neigh, int swap_pos) {
     int i, k=0, num, r,c;
     int d;


     if (neigh==NULL) {
          neigh=neighborhood_new(NEIGH_EDGE_SIZE(base->n));
     } else if (neigh->size!=NEIGH_EDGE_SIZE(base->n)) {
          neighborhood_free(neigh);
          neigh=NULL;
          neigh=neighborhood_new(NEIGH_EDGE_SIZE(base->n));
     }
     num=base->n-1;
     r=swap_pos/base->n;
     c=swap_pos%base->n;
     d=direction_edge(swap_pos,base->n);
     //UP
     for (i=1; i<num; i++) {
          if (r!=0 || c!=i) {
               neigh->neighbors[k]->move->pos1=i;
               neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
               neigh->neighbors[k]->move->rotation1=square_rotate_edge(squares[base->mat[r][c]->square_index],UP);
               neigh->neighbors[k]->move->pos2=swap_pos;
               neigh->neighbors[k]->move->square_index2=base->mat[0][i]->square_index;
               neigh->neighbors[k]->move->rotation2=square_rotate_edge(squares[base->mat[0][i]->square_index],d);
               neigh->neighbors[k]->move->move_type=EDGE;
               //DEBUG
               //move_print(neigh->neighbors[k]->move);
               neigh->neighbors[k]->matches=move_matches_diff_edge(base,neigh->neighbors[k]->move);
               //DEBUG
               //neighbor_print(neigh->neighbors[k]);
               k++;
          }
     }
     //LEFT
     for (i=1; i<num; i++) {
          if (r!=i || c!=0) {
               neigh->neighbors[k]->move->pos1=i*(base->n);
               neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
               neigh->neighbors[k]->move->rotation1=square_rotate_edge(squares[base->mat[r][c]->square_index],LEFT);
               neigh->neighbors[k]->move->pos2=swap_pos;
               neigh->neighbors[k]->move->square_index2=base->mat[i][0]->square_index;
               neigh->neighbors[k]->move->rotation2=square_rotate_edge(squares[base->mat[i][0]->square_index],d);
               neigh->neighbors[k]->move->move_type=EDGE;
               //DEBUG
               //move_print(neigh->neighbors[k]->move);
               neigh->neighbors[k]->matches=move_matches_diff_edge(base,neigh->neighbors[k]->move);
               //DEBUG
               //neighbor_print(neigh->neighbors[k]);
               k++;
          }
     }
     //RIGHT
     for (i=1; i<num; i++) {
          if (r!=i || c!=base->n-1) {
               neigh->neighbors[k]->move->pos1=i*(base->n)+(base->n-1);
               neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
               neigh->neighbors[k]->move->rotation1=square_rotate_edge(squares[base->mat[r][c]->square_index],RIGHT);
               neigh->neighbors[k]->move->pos2=swap_pos;
               neigh->neighbors[k]->move->square_index2=base->mat[i][base->n-1]->square_index;
               neigh->neighbors[k]->move->rotation2=square_rotate_edge(squares[base->mat[i][base->n-1]->square_index],d);
               neigh->neighbors[k]->move->move_type=EDGE;
               //DEBUG
               //move_print(neigh->neighbors[k]->move);
               neigh->neighbors[k]->matches=move_matches_diff_edge(base,neigh->neighbors[k]->move);
               //DEBUG
               //neighbor_print(neigh->neighbors[k]);
               k++;
          }
     }
     //DOWN
     for (i=1; i<num; i++) {
          if (r!=base->n-1 || c!=i) {
               neigh->neighbors[k]->move->pos1=(base->n-1)*(base->n)+i;
               neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
               neigh->neighbors[k]->move->rotation1=square_rotate_edge(squares[base->mat[r][c]->square_index],DOWN);
               neigh->neighbors[k]->move->pos2=swap_pos;
               neigh->neighbors[k]->move->square_index2=base->mat[base->n-1][i]->square_index;
               neigh->neighbors[k]->move->rotation2=square_rotate_edge(squares[base->mat[base->n-1][i]->square_index],d);
               neigh->neighbors[k]->move->move_type=EDGE;
               //DEBUG
               //move_print(neigh->neighbors[k]->move);
               neigh->neighbors[k]->matches=move_matches_diff_edge(base,neigh->neighbors[k]->move);
               //DEBUG
               //neighbor_print(neigh->neighbors[k]);
               k++;
          }
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


neigh_t* neighborhood_generation_corner(solution_t* base, neigh_t* neigh, int swap_pos) {
     int k=0, r,c;
     corner_dir cd;


     if (neigh==NULL) {
          neigh=neighborhood_new(NEIGH_CORNER_SIZE);
     } else if (neigh->size!=NEIGH_CORNER_SIZE) {
          neighborhood_free(neigh);
          neigh=NULL;
          neigh=neighborhood_new(NEIGH_CORNER_SIZE);
     }

     cd=direction_corner(swap_pos,base->n);
     r=swap_pos/base->n;
     c=swap_pos%base->n;

     //UPPER-LEFT
     if (r!=0 || c!=0) {
          neigh->neighbors[k]->move->pos1=0;
          neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
          neigh->neighbors[k]->move->rotation1=square_rotate_corner(squares[base->mat[r][c]->square_index],UPPER_LEFT);
          neigh->neighbors[k]->move->pos2=swap_pos;
          neigh->neighbors[k]->move->square_index2=base->mat[0][0]->square_index;
          neigh->neighbors[k]->move->rotation2=square_rotate_corner(squares[base->mat[0][0]->square_index],cd);
          neigh->neighbors[k]->move->move_type=CORNER;
          //DEBUG
          //move_print(neigh->neighbors[k]->move);
          neigh->neighbors[k]->matches=move_matches_diff_corner(base,neigh->neighbors[k]->move);
          //DEBUG
          //neighbor_print(neigh->neighbors[k]);
          k++;
     }

     //UPPER-RIGHT
     if (r!=0 || c!=base->n-1) {
          neigh->neighbors[k]->move->pos1=base->n-1;
          neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
          neigh->neighbors[k]->move->rotation1=square_rotate_corner(squares[base->mat[r][c]->square_index],UPPER_RIGHT);
          neigh->neighbors[k]->move->pos2=swap_pos;
          neigh->neighbors[k]->move->square_index2=base->mat[0][base->n-1]->square_index;
          neigh->neighbors[k]->move->rotation2=square_rotate_corner(squares[base->mat[0][base->n-1]->square_index],cd);
          neigh->neighbors[k]->move->move_type=CORNER;
          //DEBUG
          //move_print(neigh->neighbors[k]->move);
          neigh->neighbors[k]->matches=move_matches_diff_corner(base,neigh->neighbors[k]->move);
          //DEBUG
          //neighbor_print(neigh->neighbors[k]);
          k++;
     }

     //LOWER-LEFT
     if (r!=base->n-1 || c!=0) {
          neigh->neighbors[k]->move->pos1=(base->n)*(base->n-1);
          neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
          neigh->neighbors[k]->move->rotation1=square_rotate_corner(squares[base->mat[r][c]->square_index],LOWER_LEFT);
          neigh->neighbors[k]->move->pos2=swap_pos;
          neigh->neighbors[k]->move->square_index2=base->mat[base->n-1][0]->square_index;
          neigh->neighbors[k]->move->rotation2=square_rotate_corner(squares[base->mat[base->n-1][0]->square_index],cd);
          neigh->neighbors[k]->move->move_type=CORNER;
          //DEBUG
          //move_print(neigh->neighbors[k]->move);
          neigh->neighbors[k]->matches=move_matches_diff_corner(base,neigh->neighbors[k]->move);
          //DEBUG
          //neighbor_print(neigh->neighbors[k]);
          k++;
     }

     //LOWER-RIGHT
     if (r!=base->n-1 || c!=base->n-1) {
          neigh->neighbors[k]->move->pos1=(base->n)*(base->n)-1;
          neigh->neighbors[k]->move->square_index1=base->mat[r][c]->square_index;
          neigh->neighbors[k]->move->rotation1=square_rotate_corner(squares[base->mat[r][c]->square_index],LOWER_RIGHT);
          neigh->neighbors[k]->move->pos2=swap_pos;
          neigh->neighbors[k]->move->square_index2=base->mat[base->n-1][base->n-1]->square_index;
          neigh->neighbors[k]->move->rotation2=square_rotate_corner(squares[base->mat[base->n-1][base->n-1]->square_index],cd);
          neigh->neighbors[k]->move->move_type=CORNER;
          //DEBUG
          //move_print(neigh->neighbors[k]->move);
          neigh->neighbors[k]->matches=move_matches_diff_corner(base,neigh->neighbors[k]->move);
          //DEBUG
          //neighbor_print(neigh->neighbors[k]);
          k++;
     }

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
          if (found==0 && n->neighbors[i]->matches>-1) {
               max=n->neighbors[i];
               found=1;
          } else if (found==1 && neighbor_cmp_matches(max,n->neighbors[i])<0) {
               max=n->neighbors[i];
          }
     }
     return(max);
}
