#include "BobCheckersBackend.hh"
#include <iostream>
#include <chrono>
#include <thread>



using namespace Bob_checkers;
using namespace Bob_checkers::move_generator;
using namespace Bob_checkers::algo;
using namespace std::chrono;

void setup()
{
    init_move_table();
}

int main()
{
    setup();

    Board bd;
    bd.load_pos("1b1b1b1b/b1b1b1b1/1b1b1b1b/8/8/w1w1w1w1/1w1w1w1w/w1w1w1w1 w");
    std::cout << bd.print_board() << "\n\n";

    while (true)
    {
        char e;
        std::cin >> e;

        const auto begin = high_resolution_clock::now();

        move_info i = get_best_move(9, bd);

        const auto end = high_resolution_clock::now();
        const auto duration = duration_cast<milliseconds>(end - begin);

        std::cout << i._mvwrpr.print_move() << " eval is: " << i._value << " @" 
                  << ((float) i._node_count / duration.count()) << "k nodes/second\n";

        bd.move(i._mvwrpr);
        std::cout << bd.print_board() << "\n\n";
    }

    
    return 0;
}