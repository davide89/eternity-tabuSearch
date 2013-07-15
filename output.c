#include "output.h"

void write_output (char* file_name, solution_t *s) {
     FILE *fp;

     fp=fopen(file_name,"w");
     if (fp==NULL) {
          print_err(ERROR_OPEN_OUTPUT_FILE);
          exit(FAILURE);
     }
     solution_print_pieces(s,fp);
     //time, matches, etc
     fclose(fp);
     return;
}
