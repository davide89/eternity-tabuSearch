#include "input.h"

square_t** read_input (char* file_name, int *dim) {
     square_t **squares;
     int c_down, c_left, c_up, c_right;
     int n, tot, i=0,g,corner=0,edge=0,inner=0,edge_num,inner_num;
     FILE *fp;

     fp=fopen(file_name,"r");
     if (fp==NULL) {
          print_err(INPUT_FILE_NOT_FOUND);
          exit(FAILURE);
     }

     if (fscanf(fp,"%d%*d",&n)==EOF) {
          print_err(INPUT_FILE_FORMAT_UNCORRECT);
          exit(FAILURE);
     }

     edge_num=4*n-8;
     inner_num=(n-2)*(n-2);

     tot=n*n;

     squares=(square_t**)malloc(tot*sizeof(square_t*));
          if (!squares) {
               print_err(MEMORY_ALLOC);
               exit(FAILURE);
          }

     for (i=0;i<tot;i++) {
          if ( fscanf(fp,"%d%d%d%d",&c_down,&c_left,&c_up,&c_right) ==EOF && i!=tot-1) {
                              print_err(FEW_PIECES);
                              exit(FAILURE);
          } else {
               squares[i]=square_new(c_down,c_left,c_up,c_right);
               g=square_grey(squares[i]);
               if (g>2) {
                    print_err(GREY_OVER);
                    exit(FAILURE);
               }else if (g==2) {
                    squares[i]->type=CORNER;
                    corner++;
                    if (corner>4) {
                         print_err(CORNER_OVER);
                         exit(FAILURE);
                    }
               } else if (g==1) {
                    squares[i]->type=EDGE;
                    edge++;
                    if (edge>edge_num) {
                         print_err(EDGE_OVER);
                         exit(FAILURE);
                    }
               } else if (g==0){
                    squares[i]->type=INNER;
                    inner++;
                    if (inner>inner_num) {
                         print_err(INNER_OVER);
                         exit(FAILURE);
                    }
               }
          }
     }
     fclose(fp);

     *dim=n;
     return(squares);
}
