# BobCheckers

BobCheckers is a (better than a certain other) checkers engine, designed to beat that certain other checkers engine

# Features

## current:
1. pre-calculated move tables
2. force-capture validation
3. multiple-capture forcing
4. negascout algorithm
5. make/unmake structure
6. PCI compliant
7. transposition tables
8. quiescence search
9. piece-square table evaluation 

## to be added:
1. tapered eval?
2. texel-tuning

# System requirements
- Requred:
    - a compiler capable of c++17
    - 4 GB RAM
- Reccomended:
    - G++/GCC 12
    - 8 GB RAM

# Installation instructions

1. download the release
2. create a folder in the main folder called `bin`
3. if makefile is installed along with GCC , run `make all`, otherwise, just use your prefered compiler, linking `BobCheckersBackend.hh` and `BobCheckersAPI.hh` to `BobCheckers.cpp`. if makefile is used, the executable will be found in the `bin` folder
4. add the executable to a PCI-compliant interface