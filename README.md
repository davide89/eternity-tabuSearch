eternity-tabuSearch
===================

Eternity II solver based on a Tabu Search approach.

During our Optmization Methods and Algotihms course, professor has given us an assignement: to write an Eternity II solver based on a Tabu Search approach. Here it is our desperate attempt.

DOWNLOAD

You can download the Linux 32 bit version from the Download folder

DEFAULT RUN

The program uses the default values already set during the tuning phase.
The states the number of iterations before stop
The enable the multi-start mechanism. It indicates the number of steps the best solution can not be improved before diversification. To disable it, please insert the same value of
The parameter is an integer value > 1 used as seed in the solution initialization and diversification
The software prints to stdout some stats during the run
./eternity <pieces_input> <pieces_output> <max_steps_threshold> <not_improving_steps_threshold> <random_seed>

Example:

./eternity pieces_10x10.txt 10x10_out.txt 100000 100000 484

CUSTOM RUN

File Input Name

File Output Name

Tabu List Single/Triple:

SINGLE

TRIPLE

Tabu Size Corner: INTEGER

Tabu Size Edge: INTEGER

Tabu Size Inner: INTEGER

Expiration Time: INTEGER

Max Steps Threshold: INTEGER

Not Improving Steps Threshold: INTEGER

Corner Generator:

CCORNER_FIRST

CCORNER_NEXT

CCORNER_OPPOSITE

Edge Generator:

EEDGE_FIRST

EEDGE_NEXT

EEDGE_NEXT_ANTI

EEDGE_POS_OPPOSITE

Inner Generator

IINNER_NEXT

IINNER_OPPOSITE

IINNER_OPPOSITE_AUX

IINNER_RAND

Random Initial Solution:

STATIC_START

RANDOM_START

Randomization Seed: INTEGER
