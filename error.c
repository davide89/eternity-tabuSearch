#include<stdio.h>
#include "error.h"

void print_err(err_type err) {
     switch (err) {
          case WRONG_NUM_PARAM:
               fprintf(stderr,"Wrong number of parameters\n");
               break;
          case WRONG_PARAM_TLN:
               fprintf(stderr,"Wrong parameter: Tabu List Num\n");
               break;
          case WRONG_PARAM_CORNER:
               fprintf(stderr,"Wrong parameter: Corner Generator\n");
               break;
          case WRONG_PARAM_EDGE:
               fprintf(stderr,"Wrong parameter: Edge Generator\n");
               break;
          case WRONG_PARAM_INNER:
               fprintf(stderr,"Wrong parameter: Inner Generator\n");
               break;
          case WRONG_PARAM_START:
               fprintf(stderr,"Wrong parameter: Random Initial Solution\n");
               break;
          case INPUT_FILE_NOT_FOUND:
               fprintf(stderr,"Error: input file not found\n");
               break;
          case INPUT_FILE_FORMAT_UNCORRECT:
               fprintf(stderr,"Error: input file format is not correct\n");
               break;
          case MEMORY_ALLOC:
               fprintf(stderr,"Error: memory allocation failure\n");
               break;
           case CORNER_OVER:
               fprintf(stderr,"Error: too much corner squares\n");
               break;
          case EDGE_OVER:
               fprintf(stderr,"Error: too much edge squares\n");
               break;
          case INNER_OVER:
               fprintf(stderr,"Error: too much inner squares\n");
               break;
          case FEW_PIECES:
               fprintf(stderr,"Error: too few squares\n");
               break;
          case GREY_OVER:
               fprintf(stderr,"Error: square with too many grey sides\n");
               break;
          case ERROR_OPEN_OUTPUT_FILE:
               fprintf(stderr,"Error: unable to open output file\n");
               break;
     }
     return;
}
