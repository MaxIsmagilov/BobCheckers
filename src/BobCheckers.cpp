#include "Board.hh"
#include <iostream>

using namespace Bob_checkers;
using namespace Bob_checkers::move_generator;

void setup()
{
    init_move_table();
}

int main()
{
    setup();

    Board bd;
    bd.load_pos("1b1b1b1b/b1b1b1b1/1b1b1b1b/8/8/w1w1w1w1/1w1w1w1w/w1w1w1w1 w");

    std::vector<move_wrapper> movelist = get_legal_moves(bd);

    for (move_wrapper mw : movelist)
    {
        std::cout << mw.print_move() << "\n";
    }

    return 0;
}