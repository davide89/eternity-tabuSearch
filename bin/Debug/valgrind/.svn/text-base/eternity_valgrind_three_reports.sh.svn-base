#!/bin/bash


valgrind --tool=memcheck --leak-check=full --show-reachable=yes --log-file=valgrind_base.log ./eternity ../../pieces_05x05.txt ../../05x05_output.txt >/dev/null

valgrind --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes --log-file=valgrind_track_origins.log ./eternity ../../pieces_05x05.txt ../../05x05_output.txt >/dev/null

valgrind --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes -v --log-file=valgrind_verbose.log ./eternity ../../pieces_05x05.txt ../../05x05_output.txt >/dev/null
