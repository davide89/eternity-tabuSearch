#include "tabu.h"


tabuList_t *tabuList_new(int tabu_size,int expireT){
    tabuList_t *tmp;
    int i;

     tmp=(tabuList_t*)malloc(sizeof(tabuList_t));
     if (tmp==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
     }
     tmp->tabu=(move_t**)malloc(tabu_size*sizeof(move_t*));
     if (tmp->tabu==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
     }
     for(i=0;i<tabu_size;i++) {
          tmp->tabu[i]=NULL;
     }
     tmp->tabu_size=tabu_size;

     tmp->expireTime=(int*)malloc(tabu_size*sizeof(int));
     if (tmp->expireTime==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
     }

     for(i=0;i<tabu_size;i++){
        tmp->expireTime[i]=0;
     }

     tmp->eTime=expireT;

     return(tmp);
}


termination_t *termination_new(int st,int ni,stopMethod m){
    termination_t *tmp;

    tmp=(termination_t*)malloc(sizeof(termination_t));
    if (tmp==NULL) {
          print_err(MEMORY_ALLOC);
          exit(FAILURE);
    }

    tmp->thresholdStop=st;
    tmp->currentStep=0;
    tmp->noImprovingStop=ni;
    tmp->noImprovingStep=0;

    tmp->method=m;

return(tmp);
}

void termination_free(termination_t *t){

    free(t);
return;
}

void tabuList_free(tabuList_t *t){
    int i;

    //printTabu(t);

    for(i=0;i<t->tabu_size && t->tabu[i]!=NULL;i++){
        move_free(t->tabu[i]);
    }
    free(t->tabu);
    free(t->expireTime);
    free(t);
return;
}




int lookup_move(tabuList_t *l,neighbor_t *cand,int best,solution_t *s,int* tabuHit){
    int i;
    int aspiration,tabu,found;

    move_t *tmp;
    tmp=NULL;


    aspiration=check_AspirationCriteria(best,cand);
    tabu=-1;
    found=0;

    for(i=0; i < l->tabu_size && (l->tabu[i]!=NULL) && found==0; i++){
        if(move_cmp(l->tabu[i],cand->move)==0)
        {
            //forbidden move
            sort_tabuList(i,l);
            tabu=i;
            found=1;
            (*tabuHit)++;
        }
    }





    //add move in the tabu list
    if(found==0){
        tmp=move_reverse(tmp,cand->move,s);
        addMove_tabu(l,tmp);
    }


    //check expire time
    for(i=0;i<l->tabu_size && (l->tabu[i]!=NULL);i++){
        //increment expireTime[i]
        (l->expireTime[i])++;

        if(l->expireTime[i]==l->eTime){
            //tabu move must be deleted
            move_free(l->tabu[i]);
            shiftElementReverse(i,l);
        }
    }


    //printTabu(l);


    if(aspiration>0){
        return(ASPCRITERIA);
    }

    if(tabu==-1){
        return(NOTABU);
    }

return(ISTABU);
}



void addMove_tabu(tabuList_t *l,move_t *m){
     int i;

        for(i=0;i<l->tabu_size && l->tabu[i]!=NULL;i++) {
             if (move_cmp(l->tabu[i],m)==0) {
                  move_free(m);
                  return;
             }
        }
        if (l->tabu_size>0) {
          if (l->tabu[l->tabu_size-1]!=NULL) {
               move_free(l->tabu[l->tabu_size-1]);
          }
          shiftElement((l->tabu_size)-1,l);

        //add new element
        l->tabu[0]=m;
        l->expireTime[0]=0;
        }

return;
}

void sort_tabuList(int p,tabuList_t *list){
        move_t* tmp=NULL;
        int exp;

        if(p==0){
            //nothing to do
            return;
        }

        //backup value
        tmp=list->tabu[p];
        exp=list->expireTime[p];

        shiftElement(p,list);

        //restore value with the highest priority
        list->tabu[0]=tmp;
        list->expireTime[0]=exp;
        //list->expireTime[0]=0;
return;
}


void shiftElement(int start, tabuList_t *list){
        //shift
        int j;

        for(j=start;j>0;j--){
            if(list->tabu[j-1]!=NULL){
                list->expireTime[j]=list->expireTime[j-1];
                list->tabu[j]=list->tabu[j-1];
            }
        }
return;
}


void shiftElementReverse(int start, tabuList_t *list){
        //shift
        int j;

        for(j=start;j<(list->tabu_size-1) && list->tabu[j+1]!=NULL;j++){

            list->expireTime[j]=list->expireTime[j+1];
            list->tabu[j]=list->tabu[j+1];
        }

        //last element must be NULL
        list->tabu[j]=NULL;
        list->expireTime[j]=0;
return;
}



int check_TerminationCriteria(termination_t *t){

     //DEBUG
     //printf("Step: %d\n",t->currentStep);

    if((t->method==NO_IMPROVING && ((t->noImprovingStop==t->noImprovingStep) || (t->currentStep==t->thresholdStop)))){
        return(STOP);
    }

    if((t->method==NUM_STEP && (t->thresholdStop==t->currentStep))){
        return(STOP);
    }

return(CONTINUE);
}


int increment_move(termination_t *t,int imp){

    if(imp==NOT_IMPROVED){
        t->noImprovingStep++;
    }
    //currentStep in any case is incremented
    t->currentStep++;

return(check_TerminationCriteria(t));
}



int check_AspirationCriteria(int best,neighbor_t *candidate){
    int curr;

    curr=neighbor_get_matches(candidate);
return(curr-best);
}




void printTabu(tabuList_t *l){
    int i;

    for(i=0;i<l->tabu_size && l->tabu[i]!=NULL;i++){
        printf("Tabu[%d]\n",i);
        printf("\t%d\n",l->tabu[i]->square_index1);
        printf("\t%d\n",l->tabu[i]->pos1);
        printf("\t%d\n",l->tabu[i]->rotation1);
        printf("\t%d\n",l->tabu[i]->square_index2);
        printf("\t%d\n",l->tabu[i]->pos2);
        printf("\t%d\n",l->tabu[i]->rotation2);
        printf("\t%d\n",l->tabu[i]->move_type);
        printf("\tExpire time: %d\n",l->expireTime[i]);
    }

return;
}


int lookClone(tabuList_t *l){
    int z;
    int i,j;
    int* vet;

    vet=(int*)malloc(sizeof(int)*l->tabu_size);
    for (i=0;i<l->tabu_size;i++) {
         vet[i]=0;
    }

    printf("-----------------------------------\nCLONES\n");
    z=0;
    for(i=0;i<l->tabu_size && l->tabu[i]!=NULL;i++){
        for(j=i+1;j<l->tabu_size && l->tabu[j]!=NULL;j++){
            if(move_cmp(l->tabu[i],l->tabu[j])==0){
                    vet[i]=1;
                    vet[j]=1;
                }
            }
        }
    for(i=0;i<l->tabu_size;i++) {
         if (vet[i]==1) {
          printf("move %d\n",i);
          move_print(l->tabu[i]);
          z++;
         }
    }
    free(vet);
    return(z);
}


/******PARAMETRIZED VERSION******/

solution_t *tabuSearchP(solution_t *start,tabuListNum tln,int tabuSizeCorner,int tabuSizeEdge,int tabuSizeInner,int expire,int step,int noImpr,cornerGenerator cg,edgeGenerator eg,innerGenerator ig, int seed, int type, int verb){

    solution_t *sol=NULL;

    if(tln==SINGLE){
        sol=tabuSearchSingleP(start,tabuSizeCorner,0,0,step,noImpr,NO_IMPROVING,expire,cg,eg,ig,seed,type,verb);
    }
    else{
        //TRIPLE
#if HILL
        sol=tabuSearchMultipleP(start,tabuSizeCorner,tabuSizeEdge,tabuSizeInner,step,noImpr,NUM_STEP,expire,cg,eg,ig,seed,type,verb);
#else
#if STUCK
        sol=tabuSearchMultipleP(start,tabuSizeCorner,tabuSizeEdge,tabuSizeInner,step,noImpr,NUM_STEP,expire,cg,eg,ig,seed,type,verb);
#else
        sol=tabuSearchMultipleP(start,tabuSizeCorner,tabuSizeEdge,tabuSizeInner,step,noImpr,NO_IMPROVING,expire,cg,eg,ig,seed,type,verb);
#endif
#endif
    }
return(sol);
}


solution_t *tabuSearchSingleP(solution_t *start, int sizeA,int sizeB, int sizeC,int step,int noImpr,stopMethod sm,int expireTime,cornerGenerator cg,edgeGenerator eg,innerGenerator ig, int seed, int type, int verb){

    solution_t *curr, *best;
    neigh_t *neighborhood;
    neighbor_t *candidate;
    tabuList_t *tabu;
    termination_t *termine;

    /*
    //DEBUG
    move_t* rev=NULL;
    */

    int bestScore,currScore,pastScore;
    int tabuHit;
    int moveIsTabu;
    int count;
    float per;
    int end;


    //POLITICS AUX VARS
    int pos_corner=SWAP_POS_CORNER_FIRST_UPPERLEFT(start->n);
    int pos_edge=SWAP_POS_EDGE_FIRST_LEFT(start->n);
    int pos_inner=SWAP_POS_INNER_FIRST_UPPERLEFT(start->n);
    int pos_inner_aux;


    //init structure
    curr=NULL;
    best=NULL;
    neighborhood=NULL;
    candidate=NULL;
    tabu=NULL;
    termine=NULL;

    bestScore=-1;
    pastScore=-1;
    currScore=-1;
    tabuHit=0;
    moveIsTabu=0;

    int maxNoImproving=-1;

    int bestStep=0;

    //allocation
    tabu=tabuList_new(sizeA,expireTime);
    termine=termination_new(step,noImpr,sm);

    curr=start;
    best=solution_cpy(best,start);
    //DEBUG
    if (verb==VERBOSE) solution_print(start);

    count=0;
    end=CONTINUE;

    moveIsTabu=0;

     /*
    //configure inner Init
    switch(ii){

        case I_UPPERLEFT:
            pos_inner=SWAP_POS_INNER_FIRST_UPPERLEFT(start->n);
            break;

        case I_UPPERRIGHT:
            pos_inner=SWAP_POS_INNER_FIRST_UPPERRIGHT(start->n);
            break;

        case I_LOWERRIGHT:
            pos_inner=SWAP_POS_INNER_FIRST_LOWERRIGHT(start->n);
            break;

        case I_LOWERLEFT:
            pos_inner=SWAP_POS_INNER_FIRST_LOWERLEFT(start->n);
            break;
    }
    */

    //check on random
    if(ig==IINNER_RAND){
        SWAP_POS_RAND_INIT();
    }

    pos_inner_aux=SWAP_POS_INNER_FIRST_UPPERLEFT(start->n);



    while(end==CONTINUE){



if(moveIsTabu==0){


            if(count%3==0){

                switch(cg){

                    case CCORNER_NEXT:
                        neighborhood=neighborhood_generation_corner(curr,neighborhood,
                                                            (pos_corner=SWAP_POS_CORNER_NEXT(pos_corner,curr->n)) );
                        break;

                    case CCORNER_OPPOSITE:
                        neighborhood=neighborhood_generation_corner(curr,neighborhood,
                                                            (pos_corner=SWAP_POS_OPPOSITE(pos_corner,curr->n)) );
                        break;

                    case CCORNER_FIRST:
                        neighborhood=neighborhood_generation_corner(curr,neighborhood,
                                                            (pos_corner=SWAP_POS_CORNER_NEXT(pos_corner,curr->n)) );
                        break;
                }

            }
            else{
                if(count%3==1){
                    switch(eg){

                        case EEDGE_NEXT:
                            neighborhood=neighborhood_generation_edge(curr,neighborhood,
                                                              (pos_edge=SWAP_POS_EDGE_NEXT(pos_edge,curr->n)) );
                            break;

                        case EEDGE_NEXT_ANTI:
                            neighborhood=neighborhood_generation_edge(curr,neighborhood,
                                                              (pos_edge=SWAP_POS_EDGE_NEXT_ANTI(pos_edge,curr->n)) );
                            break;

                        case EEDGE_POS_OPPOSITE:
                            neighborhood=neighborhood_generation_edge(curr,neighborhood,
                                                              (pos_edge=SWAP_POS_OPPOSITE(pos_edge,curr->n)) );
                            break;
                    }

                }
                else{


                    switch(ig){


                        case IINNER_NEXT:
                            neighborhood=neighborhood_generation_inner(curr,neighborhood,
                                                               (pos_inner=SWAP_POS_INNER_NEXT(pos_inner,curr->n)) );
                            break;

                        case IINNER_RAND:
                            neighborhood=neighborhood_generation_inner(curr,neighborhood,
                                                               (pos_inner=SWAP_POS_INNER_RAND(pos_inner,curr->n)) );
                            break;

                        case IINNER_OPPOSITE:
                            neighborhood=neighborhood_generation_inner(curr,neighborhood,
                                        (pos_inner=SWAP_POS_OPPOSITE(pos_inner,curr->n)) );
                            break;

                        case IINNER_OPPOSITE_AUX:
                            neighborhood=neighborhood_generation_inner(curr,neighborhood,
                                        (pos_inner=SWAP_POS_OPPOSITE(pos_inner_aux,curr->n)) );
                            break;
                    }
                    count=-1;
                }
            }
            count++;
        }

         //choose the best in the neighborhood
         candidate=neighborhood_best_neighbor(neighborhood);


        moveIsTabu=0;
        //check if move satisfy either A.C. or tabu list

        //you can use in lookup_move bestScore or currScore (it dipends on the A.C.)

        if(candidate!=NULL && lookup_move(tabu,candidate,bestScore,curr,&tabuHit)==ISTABU){
            //move is tabu
            moveIsTabu=1;
            neighbor_discard(candidate);
        }


        if(moveIsTabu==0 && candidate!=NULL){


            if(count%3==0 && ig==IINNER_OPPOSITE_AUX) {
                    pos_inner_aux=move_get_position1(neighbor_get_move(candidate));
            }

            curr=neighbor_into_solution(curr,candidate,OVERWRITE);

            //evaluating solution
            pastScore=currScore;
            currScore=neighbor_get_matches(candidate);

            if(currScore<=pastScore){
                end=increment_move(termine,NOT_IMPROVED);
            }
            else{
                 //reset noImprovingStep
                if(termine->noImprovingStep>maxNoImproving){
                    maxNoImproving=termine->noImprovingStep;
                }
                termine->noImprovingStep=0;
                end=increment_move(termine,IMPROVED);
            }

            /*
            //DEBUG

            //move_print(rev);
            move_print(neighbor_get_move(candidate));
            solution_print(curr);
            */

            if(currScore>bestScore){
                bestScore=currScore;
                best=solution_cpy(best,curr);
                bestStep=termine->currentStep-1;
            }

            neighborhood_free(neighborhood);
            neighborhood=NULL;
        }


        //printTabu(tabu);


    }

     if (verb!=SILENT) {
          if (verb==VERBOSE) solution_print(best);
          printf("------------------------------------\nRESULTS\n");
          printf("CurrentStep: %d\n",termine->currentStep);
          printf("MaxNotImproving: %d\n",maxNoImproving);
          printf("TabuListHit: %d\n",tabuHit);
          printf("BestScoreStep: %d\n",bestStep);
          printf("BestScore: %d\n",bestScore);
          per=(float)bestScore/(float)solution_max_matches(curr->n)*100;
          printf("SolutionQuality: %.3f%%\n",per);
          printf("------------------------------------\n");
          /*OLD_PRINT_STATS
         solution_print(best);
         printf("************************************\n");
         printf("------------------------------------\nSINGLE TABU LIST\n");
         printf("------------------------------------\nSTOP METHOD: %s\n", (sm==NO_IMPROVING) ? ("NO_IMPROVING") : ("NUM_STEPS") );
         printf("------------------------------------\nPARAMETERS\n");
         printf("thresholdStop: %d\n",termine->thresholdStop);
         printf("currentStep: %d\n",termine->currentStep);
         printf("noImprovingStop: %d\n",termine->noImprovingStop);
         printf("noImprovingStep: %d\n",termine->noImprovingStep);
         printf("------------------------------------\nRESULTS\n");
         printf("TabuList hit: %d\n",tabuHit);
         printf("Max of NoImproving: %d\n",maxNoImproving);
         printf("Last Score: %d\n",currScore);
         printf("Best Score: %d\n",bestScore);
         //DEBUG
         //printf("\tLast Score: %d\n",solution_get_matches(curr));
         //printf("\tBest Score: %d\n",solution_get_matches(best));
         printf("Maximum Score: %d\n",solution_max_matches(curr->n));
         per=(float)bestScore/(float)solution_max_matches(curr->n)*100;
         printf("Solution Quality: %.3f%%\n",per);
         printf("------------------------------------\n");
          //DEBUG
          //printTabu(tabu);

          //DEBUG
         //printf("Clones in tabu list are: %d",lookClone(tabu));
         */
     }


    termination_free(termine);
    tabuList_free(tabu);
    solution_free(curr);

return(best);
}



solution_t *tabuSearchMultipleP(solution_t *start, int sizeCorner,int sizeEdge, int sizeInner,int step,int noImpr,stopMethod sm,int expireTime,cornerGenerator cg,edgeGenerator eg,innerGenerator ig, int seed, int type, int verb){

    solution_t *curr, *best;
    neigh_t *neighborhood;
    neighbor_t *candidate;
    tabuList_t *tabu_inner,*tabu_edge,*tabu_corner;
    tabuList_t *tabu;
    termination_t *termine;
    float per;

    int bestScore,currScore,pastScore;
    int tabuHit;
    int moveIsTabu;
    int count;
    int end;
    int maxNoImproving=-1;

     int bestStep=0;

     //GRAFICO
     //FILE *fp;
     //fp=fopen("grafico.txt","w");

#if HILL
     int hillHit=0;
#endif

#if PLATEAU
     int plateau=0;
     int plateauHit=0;
#endif

#if STUCK
     int stuck=0;
     int stuckHit=0;
#endif
     //POLITICS AUX VARS

    int pos_corner=SWAP_POS_CORNER_FIRST_UPPERLEFT(start->n);
    int pos_edge=SWAP_POS_EDGE_FIRST_LEFT(start->n);
    int pos_inner=SWAP_POS_INNER_FIRST_UPPERLEFT(start->n);
    int pos_inner_aux=0;


     /*
    //configure inner Init
    switch(ii){

        case I_UPPERLEFT:
            pos_inner=SWAP_POS_INNER_FIRST_UPPERLEFT(start->n);
            break;

        case I_UPPERRIGHT:
            pos_inner=SWAP_POS_INNER_FIRST_UPPERRIGHT(start->n);
            break;

        case I_LOWERRIGHT:
            pos_inner=SWAP_POS_INNER_FIRST_LOWERRIGHT(start->n);
            break;

        case I_LOWERLEFT:
            pos_inner=SWAP_POS_INNER_FIRST_LOWERLEFT(start->n);
            break;
    }
    */

    //check on random
    if(ig==IINNER_RAND){
        SWAP_POS_RAND_INIT();
    }

        pos_inner_aux=SWAP_POS_INNER_FIRST_UPPERLEFT(start->n);

    //init structure
    curr=NULL;
    best=NULL;
    neighborhood=NULL;
    candidate=NULL;
    tabu_inner=NULL;
    tabu_corner=NULL;
    tabu_edge=NULL;
    termine=NULL;

    bestScore=-1;
    pastScore=-1;
    currScore=-1;
    tabuHit=0;
    moveIsTabu=0;

    //allocation
    tabu_corner=tabuList_new(sizeCorner,expireTime);
    tabu_edge=tabuList_new(sizeEdge,expireTime);
    tabu_inner=tabuList_new(sizeInner,expireTime);
    tabu=NULL;
    termine=termination_new(step,noImpr,sm);



    curr=start;
    best=solution_cpy(best,start);

    if (verb==VERBOSE) solution_print(start);

    count=0;
    end=CONTINUE;

    moveIsTabu=0;
    while(end==CONTINUE){

        //neighborhood generation

    if(moveIsTabu==0){

#if PLATEAU
          if (plateau==PLATEAU_LENGTH) {
               curr=solution_diversification(curr,seed);
               plateau=0;
               plateauHit++;
               printf("PLATEAU\n");
          }
#endif

            if(count%3==0){

                switch(cg){

                    case CCORNER_NEXT:
                        neighborhood=neighborhood_generation_corner(curr,neighborhood,
                                                            (pos_corner=SWAP_POS_CORNER_NEXT(pos_corner,curr->n)) );
                        break;

                    case CCORNER_OPPOSITE:
                        neighborhood=neighborhood_generation_corner(curr,neighborhood,
                                                            (pos_corner=SWAP_POS_OPPOSITE(pos_corner,curr->n)) );
                        break;

                    case CCORNER_FIRST:
                        neighborhood=neighborhood_generation_corner(curr,neighborhood,
                                                            (pos_corner=SWAP_POS_CORNER_NEXT(pos_corner,curr->n)) );
                        break;
                }
                tabu=tabu_corner;

            }
            else{
                if(count%3==1){
                    switch(eg){

                        case EEDGE_NEXT:
                            neighborhood=neighborhood_generation_edge(curr,neighborhood,
                                                              (pos_edge=SWAP_POS_EDGE_NEXT(pos_edge,curr->n)) );
                            break;

                        case EEDGE_NEXT_ANTI:
                            neighborhood=neighborhood_generation_edge(curr,neighborhood,
                                                              (pos_edge=SWAP_POS_EDGE_NEXT_ANTI(pos_edge,curr->n)) );
                            break;

                        case EEDGE_POS_OPPOSITE:
                            neighborhood=neighborhood_generation_edge(curr,neighborhood,
                                                              (pos_edge=SWAP_POS_OPPOSITE(pos_edge,curr->n)) );
                            break;
                    }



                    tabu=tabu_edge;
                }
                else{


                    switch(ig){


                        case IINNER_NEXT:
                            neighborhood=neighborhood_generation_inner(curr,neighborhood,
                                                               (pos_inner=SWAP_POS_INNER_NEXT(pos_inner,curr->n)) );
                            break;

                        case IINNER_RAND:
                            neighborhood=neighborhood_generation_inner(curr,neighborhood,
                                                               (pos_inner=SWAP_POS_INNER_RAND(pos_inner,curr->n)) );
                            break;

                        case IINNER_OPPOSITE:
                            neighborhood=neighborhood_generation_inner(curr,neighborhood,
                                        (pos_inner=SWAP_POS_OPPOSITE(pos_inner,curr->n)) );
                            break;

                        case IINNER_OPPOSITE_AUX:
                            neighborhood=neighborhood_generation_inner(curr,neighborhood,
                                        (pos_inner=SWAP_POS_OPPOSITE(pos_inner_aux,curr->n)) );
                            break;
                    }
                    tabu=tabu_inner;
                    count=-1;
                }
            }
            count++;
        }

         //choose the best in the neighborhood
         candidate=neighborhood_best_neighbor(neighborhood);


        moveIsTabu=0;
        //check if move satisfy either A.C. or tabu list

        //you can use in lookup_move bestScore or currScore (it dipends on the A.C.)

        if(candidate!=NULL && lookup_move(tabu,candidate,bestScore,curr,&tabuHit)==ISTABU){
            //move is tabu
            moveIsTabu=1;
            neighbor_discard(candidate);
        }


        if(moveIsTabu==0 && candidate!=NULL){

            curr=neighbor_into_solution(curr,candidate,OVERWRITE);

            if(count%3==0 && ig==IINNER_OPPOSITE_AUX) {
                    pos_inner_aux=move_get_position1(neighbor_get_move(candidate));
            }


            //evaluating solution
            pastScore=currScore;
            currScore=neighbor_get_matches(candidate);

            //GRAFICO
            //fprintf(fp,"%d;%d\n",termine->currentStep, currScore);

            if(currScore<=pastScore){
                end=increment_move(termine,NOT_IMPROVED);
#if HILL
                if (termine->noImprovingStep==HILL_LENGTH) {
                    curr=solution_diversification(curr,seed);
                    hillHit++;
                    termine->noImprovingStep=0;
                    //printf("HILL Step:%d\n",termine->currentStep);
               }
#endif
            }
            else{
                //saving value for stats
                if(termine->noImprovingStep>maxNoImproving){
                    maxNoImproving=termine->noImprovingStep;
                }
                //reset noImprovingStep
                termine->noImprovingStep=0;
                end=increment_move(termine,IMPROVED);
            }

#if PLATEAU
            if (currScore==pastScore) plateau++;
            else plateau=0;
#endif

            if(currScore>bestScore){
                bestScore=currScore;
                best=solution_cpy(best,curr);
                bestStep=termine->currentStep-1;
            }
#if STUCK
            else {
                 stuck++;
                 if (type==POST && stuck==noImpr) {
                      stuck=0;
                      stuckHit++;
                      curr=solution_cpy(curr,best);
                      curr=solution_diversification(curr,seed+stuckHit);

                      tabuList_flush(tabu_inner);
                      tabuList_flush(tabu_edge);
                      tabuList_flush(tabu_corner);
                      tabu=NULL;
                 }
            }
#endif

            neighborhood_free(neighborhood);
            neighborhood=NULL;
        }
    }

     if (verb!=SILENT) {
          if (verb==VERBOSE) solution_print(best);
          printf("------------------------------------\nRESULTS\n");
          printf("CurrentStep: %d\n",termine->currentStep);
          printf("MaxNotImproving: %d\n",maxNoImproving);
          printf("TabuListHit: %d\n",tabuHit);
#if PLATEAU
          printf("PlateauHit: %d\n",plateauHit);
#endif
#if HILL
          printf("HillHit: %d\n",hillHit);
#endif
#if STUCK
          printf("StuckHit: %d\n",stuckHit);
#endif
          printf("BestScoreStep: %d\n",bestStep);
          printf("BestScore: %d\n",bestScore);
          per=(float)bestScore/(float)solution_max_matches(curr->n)*100;
          printf("SolutionQuality: %.3f%%\n",per);
          printf("------------------------------------\n");
         /*OLD_PRINT_STATS
         solution_print(best);
         printf("************************************\n");
         printf("------------------------------------\nSEPARATED TABU LISTS\n");
         printf("------------------------------------\nSTOP METHOD: %s\n", (sm==NO_IMPROVING) ? ("NO_IMPROVING") : ("NUM_STEPS") );
         printf("------------------------------------\nPARAMETERS\n");
         printf("thresholdStop: %d\n",termine->thresholdStop);
         printf("currentStep: %d\n",termine->currentStep);
         printf("noImprovingStop: %d\n",termine->noImprovingStop);
         printf("noImprovingStep: %d\n",termine->noImprovingStep);
         printf("------------------------------------\nRESULTS\n");
         printf("TabuList hit: %d\n",tabuHit);
         printf("Max of NoImproving: %d\n",maxNoImproving);
         //printf("Last Score: %d\n",currScore);
         printf("Best Score: %d\n",bestScore);
         //DEBUG
         //printf("\tLast Score: %d\n",solution_get_matches(curr));
         //printf("\tBest Score: %d\n",solution_get_matches(best));
         printf("Maximum Score: %d\n",solution_max_matches(curr->n));
         per=(float)bestScore/(float)solution_max_matches(curr->n)*100;
         printf("Solution quality: %.3f%%\n",per);
         printf("------------------------------------\n");
         */

     }

     //GRAFICO
     //fclose(fp);

    termination_free(termine);
    tabuList_free(tabu_inner);
    tabuList_free(tabu_edge);
    tabuList_free(tabu_corner);
    solution_free(curr);

return(best);
}






solution_t* tabuSearchDefaultPreliminary(solution_t *start, int step, int noImpr, int verb) {

     solution_t *curr, *best;
     neigh_t *neighborhood;
     neighbor_t *candidate;
     tabuList_t *tabu_inner,*tabu_edge,*tabu_corner;
     tabuList_t *tabu;
     termination_t *termine;
     float per;
     int bestScore,currScore,pastScore;
     int tabuHit;
     int moveIsTabu;
     int count;
     int end;
     int maxNoImproving=-1;
     int bestStep=0;


     //POLITICS AUX VARS
    int pos_corner=SWAP_POS_CORNER_FIRST_UPPERLEFT(start->n);
    int pos_edge=SWAP_POS_EDGE_FIRST_LEFT(start->n);
    int pos_inner=SWAP_POS_INNER_FIRST_UPPERLEFT(start->n);

    //init
    curr=NULL;
    best=NULL;
    neighborhood=NULL;
    candidate=NULL;
    tabu_inner=NULL;
    tabu_corner=NULL;
    tabu_edge=NULL;
    termine=NULL;

    bestScore=-1;
    pastScore=-1;
    currScore=-1;
    tabuHit=0;
    moveIsTabu=0;

    //allocation
    tabu_inner=tabuList_new(TABU_SIZE_INNER_DEFAULT,EXPIRE_TIME_DEFAULT);
    tabu_edge=tabuList_new(TABU_SIZE_EDGE_DEFAULT,EXPIRE_TIME_DEFAULT);
    tabu_corner=tabuList_new(TABU_SIZE_CORNER_DEFAULT,EXPIRE_TIME_DEFAULT);
    tabu=NULL;
    termine=termination_new(step,noImpr,NO_IMPROVING);


    curr=start;
    best=solution_cpy(best,start);

    if (verb==VERBOSE) solution_print(start);

    count=0;
    end=CONTINUE;

    moveIsTabu=0;
    while(end==CONTINUE){

        //neighborhood generation

     if(moveIsTabu==0) {
          if(count%3==0) {
               neighborhood=neighborhood_generation_corner(curr,neighborhood,
                                                       (pos_corner=SWAP_POS_CORNER_NEXT(pos_corner,curr->n)) );
               tabu=tabu_corner;
          } else if(count%3==1) {
               neighborhood=neighborhood_generation_edge(curr,neighborhood,
                                                         (pos_edge=SWAP_POS_OPPOSITE(pos_edge,curr->n)) );
               tabu=tabu_edge;
          } else{
               neighborhood=neighborhood_generation_inner(curr,neighborhood,
                                                          (pos_inner=SWAP_POS_INNER_NEXT(pos_inner,curr->n)) );
               tabu=tabu_inner;
               count=-1;
           }
          count++;
     }

     //choose the best in the neighborhood
     candidate=neighborhood_best_neighbor(neighborhood);

     moveIsTabu=0;
     //check if move satisfy either A.C. or tabu list
     if(candidate!=NULL && lookup_move(tabu,candidate,bestScore,curr,&tabuHit)==ISTABU){
          //move is tabu
          moveIsTabu=1;
          neighbor_discard(candidate);
     }


     if(moveIsTabu==0 && candidate!=NULL){
          curr=neighbor_into_solution(curr,candidate,OVERWRITE);

          //evaluating solution
          pastScore=currScore;
          currScore=neighbor_get_matches(candidate);

          if(currScore<=pastScore){
               end=increment_move(termine,NOT_IMPROVED);
          } else {
                //saving value for stats
                if(termine->noImprovingStep>maxNoImproving){
                    maxNoImproving=termine->noImprovingStep;
                }
                //reset noImprovingStep
                termine->noImprovingStep=0;
                end=increment_move(termine,IMPROVED);
          }
          if(currScore>bestScore){
               bestScore=currScore;
               best=solution_cpy(best,curr);
               bestStep=termine->currentStep-1;
          }
          neighborhood_free(neighborhood);
          neighborhood=NULL;
        }

    }

     if (verb!=SILENT) {
          if (verb==VERBOSE) solution_print(best);
          printf("------------------------------------\nRESULTS\n");
          printf("CurrentStep: %d\n",termine->currentStep);
          printf("MaxNotImproving: %d\n",maxNoImproving);
          printf("TabuListHit: %d\n",tabuHit);
          printf("BestScoreStep: %d\n",bestStep);
          printf("BestScore: %d\n",bestScore);
          per=(float)bestScore/(float)solution_max_matches(curr->n)*100;
          printf("SolutionQuality: %.3f%%\n",per);
          printf("------------------------------------\n");
     }


    termination_free(termine);
    tabuList_free(tabu_inner);
    tabuList_free(tabu_edge);
    tabuList_free(tabu_corner);
    solution_free(curr);

return(best);
}

solution_t* tabuSearchDefault(solution_t *start, int step, int noImpr, int seed, int verb) {

     solution_t *curr, *best;
     neigh_t *neighborhood;
     neighbor_t *candidate;
     tabuList_t *tabu_inner,*tabu_edge,*tabu_corner;
     tabuList_t *tabu;
     termination_t *termine;
     float per;
     int bestScore,currScore,pastScore;
     int tabuHit;
     int moveIsTabu;
     int count;
     int end;
     int maxNoImproving=-1;
     int bestStep=0;

     //stuck counts the not improvement steps for the multi-start
     int stuck=0;
     int stuckHit=0;


     //POLITICS AUX VARS
    int pos_corner=SWAP_POS_CORNER_FIRST_UPPERLEFT(start->n);
    int pos_edge=SWAP_POS_EDGE_FIRST_LEFT(start->n);
    int pos_inner=SWAP_POS_INNER_FIRST_UPPERLEFT(start->n);

    //init
    curr=NULL;
    best=NULL;
    neighborhood=NULL;
    candidate=NULL;
    tabu_inner=NULL;
    tabu_corner=NULL;
    tabu_edge=NULL;
    termine=NULL;

    bestScore=-1;
    pastScore=-1;
    currScore=-1;
    tabuHit=0;
    moveIsTabu=0;

    //allocation
    tabu_inner=tabuList_new(TABU_SIZE_INNER_DEFAULT,EXPIRE_TIME_DEFAULT);
    tabu_edge=tabuList_new(TABU_SIZE_EDGE_DEFAULT,EXPIRE_TIME_DEFAULT);
    tabu_corner=tabuList_new(TABU_SIZE_CORNER_DEFAULT,EXPIRE_TIME_DEFAULT);
    tabu=NULL;
    termine=termination_new(step,noImpr,NUM_STEP);


    curr=start;
    best=solution_cpy(best,start);

    if (verb==VERBOSE) solution_print(start);

    count=0;
    end=CONTINUE;

    moveIsTabu=0;
    while(end==CONTINUE){

        //neighborhood generation

     if(moveIsTabu==0) {
          if(count%3==0) {
               neighborhood=neighborhood_generation_corner(curr,neighborhood,
                                                       (pos_corner=SWAP_POS_CORNER_NEXT(pos_corner,curr->n)) );
               tabu=tabu_corner;
          } else if(count%3==1) {
               neighborhood=neighborhood_generation_edge(curr,neighborhood,
                                                         (pos_edge=SWAP_POS_OPPOSITE(pos_edge,curr->n)) );
               tabu=tabu_edge;
          } else{
               neighborhood=neighborhood_generation_inner(curr,neighborhood,
                                                          (pos_inner=SWAP_POS_INNER_NEXT(pos_inner,curr->n)) );
               tabu=tabu_inner;
               count=-1;
           }
          count++;
     }

     //choose the best in the neighborhood
     candidate=neighborhood_best_neighbor(neighborhood);

     moveIsTabu=0;
     //check if move satisfy either A.C. or tabu list
     if(candidate!=NULL && lookup_move(tabu,candidate,bestScore,curr,&tabuHit)==ISTABU){
          //move is tabu
          moveIsTabu=1;
          neighbor_discard(candidate);
     }


     if(moveIsTabu==0 && candidate!=NULL){
          curr=neighbor_into_solution(curr,candidate,OVERWRITE);

          //evaluating solution
          pastScore=currScore;
          currScore=neighbor_get_matches(candidate);

          if(currScore<=pastScore){
               end=increment_move(termine,NOT_IMPROVED);
          } else {
                //saving value for stats
                if(termine->noImprovingStep>maxNoImproving){
                    maxNoImproving=termine->noImprovingStep;
                }
                //reset noImprovingStep
                termine->noImprovingStep=0;
                end=increment_move(termine,IMPROVED);
          }
          if(currScore>bestScore){
               bestScore=currScore;
               best=solution_cpy(best,curr);
               bestStep=termine->currentStep-1;
          } else {
                 stuck++;
                 if (stuck==noImpr) {
                      stuck=0;
                      stuckHit++;
                      curr=solution_cpy(curr,best);
                      curr=solution_diversification(curr,seed+stuckHit);

                      tabuList_flush(tabu_inner);
                      tabuList_flush(tabu_edge);
                      tabuList_flush(tabu_corner);
                      tabu=NULL;
                 }
            }
          neighborhood_free(neighborhood);
          neighborhood=NULL;
        }

    }

     if (verb!=SILENT) {
          if (verb==VERBOSE) solution_print(best);
          printf("------------------------------------\nRESULTS\n");
          printf("CurrentStep: %d\n",termine->currentStep);
          printf("MaxNotImproving: %d\n",maxNoImproving);
          printf("TabuListHit: %d\n",tabuHit);
          printf("BestScoreStep: %d\n",bestStep);
          printf("BestScore: %d\n",bestScore);
          per=(float)bestScore/(float)solution_max_matches(curr->n)*100;
          printf("SolutionQuality: %.3f%%\n",per);
          printf("------------------------------------\n");
     }


    termination_free(termine);
    tabuList_free(tabu_inner);
    tabuList_free(tabu_edge);
    tabuList_free(tabu_corner);
    solution_free(curr);

return(best);
}

void tabuList_flush(tabuList_t* l) {
     int i;
     for (i=0; i<l->tabu_size && l->tabu[i]!=NULL; i++) {
          move_free(l->tabu[i]);
          l->tabu[i]=NULL;
          l->expireTime[i]=0;
     }
return;
}
