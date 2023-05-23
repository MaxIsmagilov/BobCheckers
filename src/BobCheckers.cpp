#include "Board.hh"
#include <iostream>

using namespace Bob_checkers;

int main()
{
    Board bd;
    bd.load_pos("1b1b1b1b/b1b1b1b1/1b1b1b1b/8/8/w1w1w1w1/1w1w1w1w/w1w1w1w1 w");
    bd.move((int)_E3, (int) _F4);
    std::cout << bd.print_board();
}