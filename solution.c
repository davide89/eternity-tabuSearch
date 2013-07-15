#include "solution.h"

/******************************DIRECTIONS******************************/
dir is_adjacent(int pos1, int pos2, int n) {
     int r1,c1, r2, c2;
     dir d;

     r1=pos1/n;
     c1=pos1%n;
     r2=pos2/n;
     c2=pos2%n;
     for (d=DOWN;d<=RIGHT;d++) {
          if ( (r2==r1+ADJACENT[d*2]) && c2==c1+ADJACENT[d*2+1])
               return d;
     }
     return NONE;
}


dir direction_edge(int pos, int n) {
     int r,c;
     dir d;

     r=pos/n;
     c=pos%n;
     if (r==0)      d=UP;
     else if (r== (n-1))    d=DOWN;
     else
          (c==0) ? (d=LEFT) : (d=RIGHT);
     return d;
}


corner_dir direction_corner(int pos, int n) {
     corner_dir d;
     int r,c;

     r=pos/n;
     c=pos%n;
     if (r==0 && c==0)      d=UPPER_LEFT;
     else if (r== 0 && c==(n-1))    d=UPPER_RIGHT;
     else if (r== (n-1) && c==0)    d=LOWER_LEFT;
     else if (r== (n-1) && c==(n-1))    d=LOWER_RIGHT;
     return(d);
}


/******************************SQUARE******************************/

square_t* square_new(int c_down, int c_left, int c_up, int c_right){
     square_t* tmp;

     tmp=(square_t*)malloc(sizeof(square_t));
     if (tmp==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
     }
     tmp->colour[DOWN]=c_down;
     tmp->colour[LEFT]=c_left;
     tmp->colour[UP]=c_up;
     tmp->colour[RIGHT]=c_right;
     return(tmp);
}


int square_grey(square_t* sq) {
      int cnt=0;
      int i;

      for (i=0; i<4; i++) {
           if (sq->colour[i]==GREY) {
                cnt++;
           }
      }
     return (cnt);
}


int square_rotate_edge(square_t* sq, dir side) {
     int i;

     for (i=0;i<4;i++) {
          if (sq->colour[i]==GREY) {
               //find the gray colour
               break;
          }
     }
     //clockwise rotation offset
     if (side>=i) {
          return(side-i);
     } else {
          //transform a direct anticlockwise rotation offset
          //in the corrisponding clockwise rotation offset
          return(side+4-i);
     }
}


int square_rotate_corner(square_t* sq, corner_dir side) {
     int i;

     //it works thanks to the correspondance between first gray
     //triangle directions and corners defined in the enum
     for (i=0;i<4;i++) {
          if (sq->colour[i]==GREY) {
               //case handler for RIGHT-DOWN grey couple
               if (i==0 && sq->colour[3]==GREY) {
                    i=3;
               }
               break;
          }
     }
     //clockwise rotation offset
     if (side>=i) {
          return(side-i);
     } else {
          //transform a direct anticlockwise rotation offset
          //in the corrisponding clockwise rotation offset
          return(side+4-i);
     }
}


/******************************PIECE******************************/

piece_t* piece_new(int square_idx, int rot) {
     piece_t* tmp;

     tmp=(piece_t*)malloc(sizeof(piece_t));
     if (tmp==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
     }
     tmp->rotation=rot;
     tmp->square_index=square_idx;
     return(tmp);
}


/******************************SOLUTION******************************/

solution_t *solution_new(int n) {
     int i;
     solution_t* s;

     s=(solution_t*)malloc(sizeof(solution_t));
     if (s==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
     }
     s->n=n;
     s->matches=0;
     s->mat=(piece_t***)malloc(n*sizeof(piece_t**));
     if (s->mat==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
     }
     for (i=0;i<n;i++) {
          s->mat[i]=(piece_t**)malloc(n*sizeof(piece_t*));
          if (s->mat[i]==NULL) {
               print_err(MEMORY_ALLOC);
               exit(FAILURE);
          }
     }
     return(s);
}


void solution_free(solution_t* s) {
     int i,j;

     for (i=0;i<s->n;i++) {
          for (j=0;j<s->n;j++) {
               piece_free(s->mat[i][j]);
          }
          free(s->mat[i]);
     }
     free(s->mat);
     free(s);
}


solution_t* solution_cpy(solution_t* dst, solution_t* src) {
      int i,j;

     if (dst!=NULL) {
          solution_free(dst);
          dst=NULL;
     }
          dst=solution_new(src->n);
     dst->matches=src->matches;
     for (i=0;i<dst->n;i++) {
          for (j=0;j<dst->n;j++) {
               dst->mat[i][j]=piece_new(src->mat[i][j]->square_index,src->mat[i][j]->rotation);
          }
     }
     return(dst);
}

void solution_print(solution_t *s) {
     int i,j;
     printf("------------------------------------\nSOLUTION matches %d\n\n",s->matches);
     for (i=0;i<s->n;i++) {
          printf(" | ");
          for (j=0;j<s->n;j++) {
               piece_print(s->mat[i][j]);
               printf(" | ");
          }
          printf("\n");
     }
     printf("------------------------------------\n");
}


solution_t* solution_greedy(int n) {
     solution_t *s;
     int i, dim, j=0, r=0, c=0;
     int corner=0, edge=0;

     s=solution_new(n);
     dim=n*n;
     for (i=0; i<dim; i++) {
          if (squares[i]->type==CORNER) {
               switch (corner) {
                    case 0:
                         s->mat[0][0]=piece_new(i, square_rotate_corner(squares[i],UPPER_LEFT));
                         //DEBUG ROTATION
                         /*printf("%d %d\n",squares[i]->colour[ROTATE(s->mat[0][0]->rotation,LEFT)],
                                squares[i]->colour[ROTATE(s->mat[0][0]->rotation,UP)]);*/
                         corner++;
                         break;
                    case 1:
                         s->mat[0][n-1]=piece_new(i, square_rotate_corner(squares[i],UPPER_RIGHT));
                         //DEBUG ROTATION
                         /*printf("%d %d\n",squares[i]->colour[ROTATE(s->mat[0][n-1]->rotation,UP)],
                                squares[i]->colour[ROTATE(s->mat[0][n-1]->rotation,RIGHT)]);*/
                         corner++;
                         break;
                    case 2:
                         s->mat[n-1][0]=piece_new(i, square_rotate_corner(squares[i],LOWER_LEFT));
                         //DEBUG ROTATION
                         /*printf("%d %d\n",squares[i]->colour[ROTATE(s->mat[n-1][0]->rotation,DOWN)],
                                squares[i]->colour[ROTATE(s->mat[n-1][0]->rotation,LEFT)]);*/
                         corner++;
                         break;
                    case 3:
                         s->mat[n-1][n-1]=piece_new(i,square_rotate_corner(squares[i],LOWER_RIGHT));
                         //DEBUG ROTATION
                         /*printf("%d %d\n",squares[i]->colour[ROTATE(s->mat[n-1][n-1]->rotation,RIGHT)],
                                squares[i]->colour[ROTATE(s->mat[n-1][n-1]->rotation,DOWN)]);*/
                         //corner++; //DEBUG
                         break;
               }
          } else  if (squares[i]->type==EDGE) {
               switch (edge) {
                    case 0:
                         //UP EDGE
                         s->mat[0][1+j]=piece_new(i,square_rotate_edge(squares[i],UP));
                         //DEBUG ROTATION
                         //printf("%d\n",squares[i]->colour[ROTATE(s->mat[0][1+j]->rotation,UP)]);
                         j++;
                         if (j==n-2) {
                              j=0;
                              edge++;
                         }
                         break;
                    case 1:
                         //LEFT EDGE
                         s->mat[1+j][0]=piece_new(i,square_rotate_edge(squares[i],LEFT));
                         //DEBUG ROTATION
                         //printf("%d\n",squares[i]->colour[ROTATE(s->mat[1+j][0]->rotation,LEFT)]);
                         j++;
                         if (j==n-2) {
                              j=0;
                              edge++;
                         }
                         break;
                    case 2:
                         //RIGHT EDGE
                         s->mat[1+j][n-1]=piece_new(i,square_rotate_edge(squares[i],RIGHT));
                         //DEBUG ROTATION
                         //printf("%d\n",squares[i]->colour[ROTATE(s->mat[1+j][n-1]->rotation,RIGHT)]);
                         j++;
                         if (j==n-2) {
                              j=0;
                              edge++;
                         }
                         break;
                    case 3:
                         //DOWN EDGE
                         s->mat[n-1][1+j]=piece_new(i,square_rotate_edge(squares[i],DOWN));
                         //DEBUG ROTATION
                         //printf("%d\n",squares[i]->colour[ROTATE(s->mat[n-1][1+j]->rotation,DOWN)]);
                         j++;
                         //DEBUG
                         /*if (j==n-2) {
                              j=0;
                              edge++;
                         }*/
                         break;
               }
          } else {
               //INNER
               s->mat[1+r][1+c]=piece_new(i,0);
               c++;
               if (c==n-2) {
                    r++;
                    c=0;
               }
          }
     }

/*
     //DEBUG FOR MOVE_MATCHES_DIFF
     s->mat[1][2]->rotation=1;
     //pay attention: the swap_rotate do not
     //consider the initial offset of the piece
     //rot=0 corrsponds to the original piece
     //and not this one that is already rotate by 1
     //and is shown in the visualizer
*/
     s->matches=solution_matches(s);


     //DEBUG PIECE COUNT MATCHES
/*
     //CORNER

     printf("The upper-left corner piece gives %d matches\n",solution_piece_matches_corner(s,NULL,0));
     printf("The upper-right corner piece gives %d matches\n",solution_piece_matches_corner(s,NULL,4));
     printf("The lowet-left corner piece gives %d matches\n",solution_piece_matches_corner(s,NULL,20));
     printf("The lower-right corner piece gives %d matches\n",solution_piece_matches_corner(s,NULL,24));
*/
/*
     //0 MATCHES
     piece_t* tmp;
     tmp=s->mat[1][0];
     s->mat[1][0]=s->mat[3][0];
     s->mat[3][0]=tmp;
     printf("The upper-left corner piece gives %d matches\n",solution_piece_matches_corner(s,NULL,0));
*/
/*
     //2 MATCHES
     piece_t* tmp;
     tmp=s->mat[0][1];
     s->mat[0][1]=s->mat[0][2];
     s->mat[0][2]=tmp;
     printf("The upper-left corner piece gives %d matches\n",solution_piece_matches_corner(s,NULL,0));
*/
/*
     //INNER
     printf("The inner piece 6 (1,1) gives %d matches\n",solution_piece_matches_inner(s,NULL,6)); //0
     printf("The inner piece 16 (2,3) gives %d matches\n",solution_piece_matches_inner(s,NULL,13)); //1
     printf("The inner piece 16 (3,1) gives %d matches\n",solution_piece_matches_inner(s,NULL,16)); //2
*/
/*
     //EDGE
     printf("The edge piece 5 (1,0) gives %d matches\n",solution_piece_matches_edge(s,NULL,5)); //1
     printf("The edge piece 14 (2,4) gives %d matches\n",solution_piece_matches_edge(s,NULL,14)); //3
*/
/*
    // DEBUG COUNT MATCHES
    int matches;
    piece_t* tmp;
    tmp=s->mat[3][0];
    s->mat[3][0]=s->mat[2][0];
    s->mat[2][0]=tmp;
    matches=solution_matches(s);
    printf("Current solution: %d\n",matches); //14
*/

     return(s);
}


void solution_print_pieces(solution_t* s, FILE* fp) {
     int i, j;

     for (i=0; i < s->n; i++) {
          for (j=0; j < s->n; j++) {
               fprintf(fp,"%d %d\n",s->mat[i][j]->square_index, s->mat[i][j]->rotation);
          }
     }
     return;
}


int solution_matches(solution_t* s){
    int i,j,n;
    int colA,colB; //used to make the code readable
    int cnt;
    piece_t ***mat;

    n=s->n;
    mat=s->mat;
    cnt=0;
    for(i=0;i<n;i++){
        if(i==0){
            //only horizontal-square_tcount
            for(j=1;j<n;j++){
                colA=PIECE_COLOUR(mat[i][j],LEFT);
                //squares[mat[i][j]->square_index]->colour[ROTATE(mat[i][j]->rotation,LEFT)];
                colB=PIECE_COLOUR(mat[i][j-1],RIGHT);
                //colB=squares[mat[i][j-1]->square_index]->colour[ROTATE(mat[i][j-1]->rotation,RIGHT)];
                if(colA==colB){
                cnt++;
                }
            }
        }
        else{
            //counting both vertical and horizontal
                //first column
                colA=PIECE_COLOUR(mat[i][0],UP);
                //colA=squares[mat[i][0]->square_index]->colour[ROTATE(mat[i][0]->rotation,UP)];
                colB=PIECE_COLOUR(mat[i-1][0],DOWN);
                //colB=squares[mat[i-1][0]->square_index]->colour[ROTATE(mat[i-1][0]->rotation,DOWN)];
                if(colA==colB){
                    cnt++;
                }
            for(j=1;j<n;j++){
                //horizontal
                colA=PIECE_COLOUR(mat[i][j],LEFT);
                //colA=squares[mat[i][j]->square_index]->colour[ROTATE(mat[i][j]->rotation,LEFT)];
                colB=PIECE_COLOUR(mat[i][j-1],RIGHT);
                //colB=squares[mat[i][j-1]->square_index]->colour[ROTATE(mat[i][j-1]->rotation,RIGHT)];
                if(colA==colB){
                    cnt++;
                }
                //vertical
                colA=PIECE_COLOUR(mat[i][j],UP);
                //colA=squares[mat[i][j]->square_index]->colour[ROTATE(mat[i][j]->rotation,UP)];
                colB=PIECE_COLOUR(mat[i-1][j],DOWN);
                //colB=squares[mat[i-1][j]->square_index]->colour[ROTATE(mat[i-1][j]->rotation,DOWN)];
                if(colA==colB){
                    cnt++;
                }
            }
        }
    }
return(cnt);
}



//THE FUNCTIONS ARE DIVIDED TO AVOID A CASCADE OF IF CLAUSES EVERY TIME

int solution_piece_matches_corner(solution_t* s, piece_t* p, int pos) {
     int r,c,m=0;
     piece_t*** mat;
     int colA,colB;

     r=pos/s->n;
     c=pos%s->n;
     mat=s->mat;
     if (p==NULL) {
          p=mat[r][c];
     }
     if (pos==0) {
               //UPPER_LEFT;
               colA=PIECE_COLOUR(p,DOWN);
               //colA=squares[mat[0][0]->square_index]->colour[ROTATE(mat[0][0]->rotation,DOWN)];
               colB=PIECE_COLOUR(mat[1][0],UP);
               //colB=squares[mat[1][0]->square_index]->colour[ROTATE(mat[1][0]->rotation,UP)];
               if (colA==colB) m++;
               colA=PIECE_COLOUR(p,RIGHT);
               //colA=squares[mat[0][0]->square_index]->colour[ROTATE(mat[0][0]->rotation,RIGHT)];
               colB=PIECE_COLOUR(mat[0][1],LEFT);
               //colB=squares[mat[0][1]->square_index]->colour[ROTATE(mat[0][1]->rotation,LEFT)];
               if (colA==colB) m++;
     } else if(pos==s->n-1) {
               //UPPER_RIGHT;
               colA=PIECE_COLOUR(p,DOWN);
               //colA=squares[mat[0][s->n-1]->square_index]->colour[ROTATE(mat[0][s->n-1]->rotation,DOWN)];
               colB=PIECE_COLOUR(mat[1][s->n-1],UP);
               //colB=squares[mat[1][s->n-1]->square_index]->colour[ROTATE(mat[1][s->n-1]->rotation,UP)];
               if (colA==colB) m++;
               colA=PIECE_COLOUR(p,LEFT);
               //colA=squares[mat[0][s->n-1]->square_index]->colour[ROTATE(mat[0][s->n-1]->rotation,LEFT)];
               colB=PIECE_COLOUR(mat[0][s->n-2],RIGHT);
               //colB=squares[mat[0][s->n-2]->square_index]->colour[ROTATE(mat[0][s->n-2]->rotation,RIGHT)];
               if (colA==colB) m++;
     } else  if( pos==(s->n-1)*(s->n) ) {
               //LOWER_LEFT;
               colA=PIECE_COLOUR(p,UP);
               //colA=squares[mat[s->n-1][0]->square_index]->colour[ROTATE(mat[s->n-1][0]->rotation,UP)];
               colB=PIECE_COLOUR(mat[s->n-2][0],DOWN);
               //colB=squares[mat[s->n-2][0]->square_index]->colour[ROTATE(mat[s->n-2][0]->rotation,DOWN)];
               if (colA==colB) m++;
               colA=PIECE_COLOUR(p,RIGHT);
               //colA=squares[mat[s->n-1][0]->square_index]->colour[ROTATE(mat[s->n-1][0]->rotation,RIGHT)];
               colB=PIECE_COLOUR(mat[s->n-1][1],LEFT);
               //colB=squares[mat[s->n-1][1]->square_index]->colour[ROTATE(mat[s->n-1][1]->rotation,LEFT)];
               if (colA==colB) m++;
     } else {
               //LOWER_RIGHT;
               colA=PIECE_COLOUR(p,UP);
               //colA=squares[mat[s->n-1][s->n-1]->square_index]->colour[ROTATE(mat[s->n-1][s->n-1]->rotation,UP)];
               colB=PIECE_COLOUR(mat[s->n-2][s->n-1],DOWN);
               //colB=squares[mat[spiece_colour->n-2][s->n-1]->square_index]->colour[ROTATE(mat[s->n-2][s->n-1]->rotation,DOWN)];
               if (colA==colB) m++;
               colA=PIECE_COLOUR(p,LEFT);
               //colA=squares[mat[s->n-1][s->n-1]->square_index]->colour[ROTATE(mat[s->n-1][s->n-1]->rotation,LEFT)];
               colB=PIECE_COLOUR(mat[s->n-1][s->n-2],RIGHT);
               //colB=squares[mat[s->n-1][s->n-2]->square_index]->colour[ROTATE(mat[s->n-1][s->n-2]->rotation,RIGHT)];
               if (colA==colB) m++;
     }
     return (m);
}



int solution_piece_matches_edge(solution_t* s, piece_t* p, int pos) {
     int r,c,m=0;
     dir edge_dir;
     dir d;
     piece_t*** mat;
     int colA,colB;

     r=pos/(s->n);
     c=pos%(s->n);
     mat=s->mat;
     if (p==NULL) {
          p=mat[r][c];
     }
     edge_dir=direction_edge(pos,s->n);
     for(d=DOWN;d<=RIGHT;d++) {
          if (d!=edge_dir) {
               colA=PIECE_COLOUR(p,d);
               //colA=squares[p->square_index]->colour[ROTATE(p->rotation,d)];
               colB=PIECE_COLOUR(mat[r+ADJACENT[d*2]] [c+ADJACENT[d*2+1]],OPPOSITE(d));
               /*colB=squares[mat[r+ADJACENT[d*2]][c+ADJACENT[d*2+1]]->square_index]->colour
                    [ROTATE(mat[r+ADJACENT[d*2]][c+ADJACENT[d*2+1]]->rotation, (d+2)%4)];*/
               if (colA==colB) m++;
          }
     }
     return (m);
}


int solution_piece_matches_inner(solution_t* s, piece_t* p, int pos) {
     int r,c,m=0;
     dir d;
     piece_t*** mat;
     int colA,colB;

     r=pos/s->n;
     c=pos%s->n;
     mat=s->mat;
     if (p==NULL) {
          p=mat[r][c];
     }
     for(d=DOWN;d<=RIGHT;d++) {
          colA=PIECE_COLOUR(p,d);
          //colA=squares[p->square_index]->colour[ROTATE(p->rotation,d)];
          colB=PIECE_COLOUR(mat[r+ADJACENT[d*2]] [c+ADJACENT[d*2+1]],OPPOSITE(d));
          /*colB=squares[mat[r+ADJACENT[d*2]][c+ADJACENT[d*2+1]]->square_index]->colour
               [ROTATE(mat[r+ADJACENT[d*2]][c+ADJACENT[d*2+1]]->rotation, (d+2)%4)];*/
          if (colA==colB) m++;
     }
     return (m);
}

solution_t* solution_shuffle(solution_t* s, int seed) {
     int k,i,num,pos1,pos2,r1,r2,c1,c2;
     piece_t* tmp;


     if (seed==-1) {
          srand( time(NULL));
     } else {
          srand(seed*10);
     }
     num=rand()%100;
     //printf("%d\n",num);
     pos1=s->n+2;
     pos2=s->n+2;
     r1=pos1/s->n;
     c1=pos1%s->n;
     r2=pos2/s->n;
     c2=pos2%s->n;

     for (k=0;k<num;k++) {
          pos1=INNER_RAND(pos1,s->n);
          //printf("%d\n",pos1);
          pos2=INNER_RAND(pos2,s->n);
          //printf("%d\n",pos2);
          r1=pos1/s->n;
          c1=pos1%s->n;
          r2=pos2/s->n;
          c2=pos2%s->n;
          s->mat[r1][c1]->rotation=rand()%4;
          s->mat[r2][c2]->rotation=rand()%4;
          tmp=s->mat[r1][c1];
          s->mat[r1][c1]=s->mat[r2][c2];
          s->mat[r2][c2]=tmp;
     }
     num=rand()%50;
     //printf("%d\n",num);
     for(k=0;k<num;k++) {
          i=rand()%4;
          switch (i) {
               case 0:
                    pos1=EDGE_DOWN_RAND(pos1,s->n);
                    break;
               case 1:
                    pos1=EDGE_LEFT_RAND(pos1,s->n);
                    break;
               case 2:
                    pos1=EDGE_UP_RAND(pos1,s->n);
                    break;
               case 3:
                    pos1=EDGE_RIGHT_RAND(pos1,s->n);
                    break;
          }
          i=rand()%4;
          switch (i) {
               case 0:
                    pos2=EDGE_DOWN_RAND(pos2,s->n);
                    break;
               case 1:
                    pos2=EDGE_LEFT_RAND(pos2,s->n);
                    break;
               case 2:
                    pos2=EDGE_UP_RAND(pos2,s->n);
                    break;
               case 3:
                    pos2=EDGE_RIGHT_RAND(pos2,s->n);
                    break;
          }
          r1=pos1/s->n;
          c1=pos1%s->n;
          r2=pos2/s->n;
          c2=pos2%s->n;
          tmp=s->mat[r1][c1];
          s->mat[r1][c1]=s->mat[r2][c2];
          s->mat[r2][c2]=tmp;
          s->mat[r1][c1]->rotation=square_rotate_edge(squares[s->mat[r1][c1]->square_index],direction_edge(pos1,s->n));
          s->mat[r2][c2]->rotation=square_rotate_edge(squares[s->mat[r2][c2]->square_index],direction_edge(pos2,s->n));
     }

     s->matches=solution_matches(s);
     return(s);
}

solution_t* solution_diversification(solution_t* s, int seed) {
     int k,i,num,pos1,pos2,r1,r2,c1,c2;
     piece_t* tmp;

     if (seed==-1) {
          srand( time(NULL));
     } else {
          srand(seed*10);
     }
     num=rand()%50;
     //printf("%d\n",num);
     pos1=s->n+2;
     pos2=s->n+2;
     r1=pos1/s->n;
     c1=pos1%s->n;
     r2=pos2/s->n;
     c2=pos2%s->n;

     for (k=0;k<num;k++) {
          pos1=INNER_RAND(pos1,s->n);
          //printf("%d\n",pos1);
          pos2=INNER_RAND(pos2,s->n);
          //printf("%d\n",pos2);
          r1=pos1/s->n;
          c1=pos1%s->n;
          r2=pos2/s->n;
          c2=pos2%s->n;
          s->mat[r1][c1]->rotation=rand()%4;
          s->mat[r2][c2]->rotation=rand()%4;
          tmp=s->mat[r1][c1];
          s->mat[r1][c1]=s->mat[r2][c2];
          s->mat[r2][c2]=tmp;
     }
     num=rand()%10;
     //printf("%d\n",num);
     for(k=0;k<num;k++) {
          i=rand()%4;
          switch (i) {
               case 0:
                    pos1=EDGE_DOWN_RAND(pos1,s->n);
                    break;
               case 1:
                    pos1=EDGE_LEFT_RAND(pos1,s->n);
                    break;
               case 2:
                    pos1=EDGE_UP_RAND(pos1,s->n);
                    break;
               case 3:
                    pos1=EDGE_RIGHT_RAND(pos1,s->n);
                    break;
          }
          i=rand()%4;
          switch (i) {
               case 0:
                    pos2=EDGE_DOWN_RAND(pos2,s->n);
                    break;
               case 1:
                    pos2=EDGE_LEFT_RAND(pos2,s->n);
                    break;
               case 2:
                    pos2=EDGE_UP_RAND(pos2,s->n);
                    break;
               case 3:
                    pos2=EDGE_RIGHT_RAND(pos2,s->n);
                    break;
          }
          r1=pos1/s->n;
          c1=pos1%s->n;
          r2=pos2/s->n;
          c2=pos2%s->n;
          tmp=s->mat[r1][c1];
          s->mat[r1][c1]=s->mat[r2][c2];
          s->mat[r2][c2]=tmp;
          s->mat[r1][c1]->rotation=square_rotate_edge(squares[s->mat[r1][c1]->square_index],direction_edge(pos1,s->n));
          s->mat[r2][c2]->rotation=square_rotate_edge(squares[s->mat[r2][c2]->square_index],direction_edge(pos2,s->n));
     }

     s->matches=solution_matches(s);
     return(s);
}

solution_t* solution_shuffle2(solution_t* s, int seed) {
     int k,i,num,pos1,pos2,r1,r2,c1,c2;
     piece_t* tmp;


     if (seed==-1) {
          srand( time(NULL));
     } else {
          srand(seed*10);
     }
     num=rand()%100;
     //printf("%d\n",num);
     pos1=s->n+2;
     pos2=s->n+2;
     r1=pos1/s->n;
     c1=pos1%s->n;
     r2=pos2/s->n;
     c2=pos2%s->n;

     for (k=0;k<num;k++) {
          pos1=INNER_RAND(pos1,s->n);
          //printf("%d\n",pos1);
          pos2=INNER_RAND(pos2,s->n);
          //printf("%d\n",pos2);
          s->mat[r1][c1]->rotation=rand()%4;
          s->mat[r2][c2]->rotation=rand()%4;
          tmp=s->mat[r1][c1];
          s->mat[r1][c1]=s->mat[r2][c2];
          s->mat[r2][c2]=tmp;
     }
     num=rand()%50;
     //printf("%d\n",num);
     for(k=0;k<num;k++) {
          i=rand()%4;
          switch (i) {
               case 0:
                    pos1=EDGE_DOWN_RAND(pos1,s->n);
                    break;
               case 1:
                    pos1=EDGE_LEFT_RAND(pos1,s->n);
                    break;
               case 2:
                    pos1=EDGE_UP_RAND(pos1,s->n);
                    break;
               case 3:
                    pos1=EDGE_RIGHT_RAND(pos1,s->n);
                    break;
          }
          i=rand()%4;
          switch (i) {
               case 0:
                    pos2=EDGE_DOWN_RAND(pos2,s->n);
                    break;
               case 1:
                    pos2=EDGE_LEFT_RAND(pos2,s->n);
                    break;
               case 2:
                    pos2=EDGE_UP_RAND(pos2,s->n);
                    break;
               case 3:
                    pos2=EDGE_RIGHT_RAND(pos2,s->n);
                    break;
          }
          tmp=s->mat[r1][c1];
          s->mat[r1][c1]=s->mat[r2][c2];
          s->mat[r2][c2]=tmp;
          s->mat[r1][c1]->rotation=square_rotate_edge(squares[s->mat[r1][c1]->square_index],direction_edge(pos1,s->n));
          s->mat[r2][c2]->rotation=square_rotate_edge(squares[s->mat[r2][c2]->square_index],direction_edge(pos2,s->n));
     }

     s->matches=solution_matches(s);
     return(s);
}
