#!/bin/bash

valgrind --tool=memcheck --leak-check=full --show-reachable=yes --log-file=valgrind_base.log ./eternity ../../pieces_05x05.txt ../../05x05_output.txt >/dev/null