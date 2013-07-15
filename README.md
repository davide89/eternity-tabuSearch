eternity-tabu
=============

Eternity II solver based on a Tabu Search approach.

During our Optmization Methods and Algotihms course, professor has given us an assignement: to write an Eternity II solver based on a Tabu Search approach. Here it is our desperate attempt. 

### DOWNLOAD

You can download the Linux 32 bit version from the Download folder

### DEFAULT RUN

* The program uses the default values already set during the tuning phase.
* The <max_steps_threshold> states the number of iterations before stop
* The <not_improving_steps_threshold> enable the multi-start mechanism. It indicates the number of steps the best solution can not be improved before diversification. To disable it, please insert the same value of <max_steps_threshold>
* The <random_seed> parameter is an integer value > 1 used as seed in the solution initialization and diversification
* The software prints to stdout some stats during the run  

`./eternity <pieces_input> <pieces_output> <max_steps_threshold> <not_improving_steps_threshold> <random_seed>` 

__Example:__

`./eternity pieces_10x10.txt 10x10_out.txt 100000 100000 484`


### CUSTOM RUN

1. File Input Name

2. File Output Name

3. Tabu List Single/Triple:
    
    SINGLE
    
    TRIPLE

4. Tabu Size Corner: INTEGER

5. Tabu Size Edge: INTEGER

6. Tabu Size Inner: INTEGER

7. Expiration Time: INTEGER

8. Max Steps Threshold: INTEGER

9. Not Improving Steps Threshold: INTEGER

10. Corner Generator:
    
    CCORNER_FIRST
    
    CCORNER_NEXT
    
    CCORNER_OPPOSITE

11. Edge Generator:

    EEDGE_FIRST
    
    EEDGE_NEXT
    
    EEDGE_NEXT_ANTI
    
    EEDGE_POS_OPPOSITE

12. Inner Generator

    IINNER_NEXT
    
    IINNER_OPPOSITE
    
    IINNER_OPPOSITE_AUX
    
    IINNER_RAND

13. Random Initial Solution:

    STATIC_START
    
    RANDOM_START

14. Randomization Seed: INTEGER

__Example:__

`./eternity pieces_10x10.txt 10x10_output.txt TRIPLE 3 10 10 30 100000 100000 CCORNER_NEXT EEDGE_POS_OPPOSITE IINNER_NEXT RANDOM_START 484`