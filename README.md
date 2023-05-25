# BobCheckers

BobCheckers is a (better than a certain other) checkers engine, designed to beat that certain other checkers engine

# Features

## current:
1. pre-calculated move tables
2. force-capture validation
3. multiple-capture forcing
4. negamax + ab pruning
5. make/unmake structure
6. PCI compliant

## to be added:
1. transposition tables
2. move ordering?

# System requirements
Requred
a compiler capable of c++17
Reccomended
G++/GCC 12

# Installation instructions

1. download the release
2. if makefile is installed along with GCC , run `make all`
2. otherwise, just use your prefered compiler, linking `BobCheckersBackend.hh` and `BobCheckersAPI.hh` to `BobCheckers.cpp` 
3. add the executable to a PCI-compliant interface