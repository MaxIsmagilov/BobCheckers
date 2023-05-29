#ifndef BobChkrsAPI
#define BobChkrsAPI

#include "BobCheckersBackend.hh"
#include <cstring>
#include <iostream>

namespace Bob_checkers
{

namespace PCI
{

/// @brief the main PCI board
Board _MAIN_BOARD;

/// @brief main transposition table
tt_util::Transposition_Table _MAIN_TABLE;

/// @brief parses a list of moves and applies it
/// @param input 
void parse_moves(std::string&& input)
{
    // if the string is null, exit
    if (!input.size()) return;

    // find the next space or the end of the string
    int next_space = 0;
    while (input[next_space] != '\0' &&  input[next_space] != ' ') next_space++;

    // create a string containing the next move
    std::string move(input.substr(0, next_space));

    // generate moves
    move_generator::Move_Generator mg(_MAIN_BOARD);
    std::vector<Move_Wrapper> moves(mg());

    // exit if there are no moves
    if (!moves.size()) return;

    // go through the moves, checking if any of them match
    Move_Wrapper mw;
    for (Move_Wrapper check : moves)
    {
        mw = Move_Wrapper(check);

        // if a move matches, exit the loop
        if (check.print_move() == move) break;
    }

    // make the move
    _MAIN_BOARD.move(mw);

    // exit if no moves left
    if (next_space == input.length()) return;

    // parse the next moves recursively
    parse_moves(input.substr(next_space+1));
}

/// @brief parses an input string
/// @param input 
/// @return whether to break or not
int parse_input(std::string&& input)
{
    // create a character pointer
    char* value;

    // process `.exit` command
    // note: this command is separate from the PCI, it is used for terminal gameplay
    if (value = strstr(input.c_str(), ".exit"))
     {  return 1; }

    // process `endgame` command
    if (value = strstr(input.c_str(), "endgame"))
     {  return 0; }

    // process `pcinewgame` command
    else if (value = strstr(input.c_str(), "pcinewgame"))
     {  _MAIN_BOARD.load_pos("1b1b1b1b/b1b1b1b1/1b1b1b1b/8/8/w1w1w1w1/1w1w1w1w/w1w1w1w1 w");    }  

    // process `isready` command
    else if (value = strstr(input.c_str(), "isready"))
     {  std::cout << "readyok\n"; }

    // process `go depth` command
    else if (value = strstr(input.c_str(), "go depth"))
     {  value += 9;
        algo::Move_Info mi = algo::get_best_move(std::stoi(value) , _MAIN_BOARD, _MAIN_TABLE);  
        std::cout << "do move " << mi._mvwrpr.print_move() << "\n"; 
        _MAIN_BOARD.move(mi._mvwrpr);   }

    // process `.play` command
    // note: this command is separate from the PCI, it is used for terminal gameplay
    else if (value = strstr(input.c_str(), ".play"))
     {  value += 6;
        algo::Move_Info mi = algo::get_best_move(std::stoi(value) , _MAIN_BOARD, _MAIN_TABLE);  
        std::cout << "do move " << mi._mvwrpr.print_move() << "\n"; 
        _MAIN_BOARD.move(mi._mvwrpr);  
        std::cout << mi._value/100.0F << ", " << mi._node_count << " nodes\n";
        std::cout << _MAIN_BOARD.print_board() << "\n"; }

    // process `.printboard` command
    // note: this command is separate from the PCI, it is used for terminal gameplay
    else if (value = strstr(input.c_str(), ".printboard"))
     {  std::cout << _MAIN_BOARD.print_board() << "\n"; }

    // process `move` command
    else if (value = strstr(input.c_str(), "move"))
     {  parse_moves(input.substr(5)); }

    return 0;
}

/// @brief connects to the PCI loop
/// @note will run indefinitely
void PCI_loop()
{
    // allocate a buffer
    char buffer[256];
    while (true)
    {
        // sync stdin
        std::cin.sync();

        // get the line/command
        if (std::cin.getline(buffer, 100))
        {
            // process the command
            if (parse_input(std::string(buffer))) break;
        }
    }
}

} // end of PCI namespace

} // end of main Bob_checkers namespace

#endif