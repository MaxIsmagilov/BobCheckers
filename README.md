# BobCheckers

BobCheckers is a (better than a certain other) [checkers](docs/CheckersRules.md) engine, designed to beat that certain other checkers engine. This engine is designed by Max Ismagilov.
For the official Github repo, visit [go here](https://github.com/MaxIsmagilov/BobCheckers).

## Features

### Current:
1. pre-calculated move tables
2. force-capture validation
3. multiple-capture forcing
4. negascout algorithm
5. make/unmake structure
6. PCI compliant: [documentation for more information](docs/PolyCheckersInterface.md)
7. transposition tables
8. quiescence search
9. piece-square table evaluation 

### To be added:
1. tapered eval (evaluation where the evaluation changes as the game progresses)
2. texel-tuning (similar to training an AI system, texel-tuning finds the optimal values to get the best result)
3. multithreading (stretch, I hate multithreading personally)

## System requirements
- Requred:
    - a compiler capable of c++20 
    - 4 GB RAM
- Reccomended:
    - G++/GCC 12
    - 8 GB RAM

## Installation instructions

1. download the relese
2. make a folder called `build` inside this one and change to int
3. run `cmake ../ && make`
4. the execuatble will be at the path `build/src/BobCheckers_1.0.0`