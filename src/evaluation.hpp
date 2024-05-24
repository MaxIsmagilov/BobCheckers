#pragma once

#include "board.hpp"
#include "move.hpp"

namespace BobCheckers
{

/// @brief evaluation namespace
namespace Evaluator
{

/// @brief flips the square
/// @param square
/// @return a flipped square
#define flip_square(square) (31 - (square))

/// @brief man value constants
constexpr int man_values[32]{0,  0,  0,  0,    //
                             24, 10, 8,  9,    //
                             8,  10, 10, 24,   //
                             3,  4,  5,  1,    //
                             2,  13, 8,  1,    //
                             1,  2,  1,  0,    //
                             -1, 7,  9,  25,   //
                             -5, 23, 10, 32};  //

/// @brief king value constants
constexpr int king_values[32]{-7,  -1, -3, -10,  //
                              -5,  0,  -1, -1,   //
                              1,   10, 8,  -1,   //
                              -1,  10, 11, 1,    //
                              2,   11, 10, -1,   //
                              -1,  8,  10, 0,    //
                              -1,  -1, -1, -1,   //
                              -10, 0,  -1, -7};  //

/// @brief piece value constants
constexpr int piece_values[4]{100, 400, -100, -400};

/// @brief finds the heuristic of the move
/// @param piece
/// @param mv
/// @return an int
int heuristic(int piece, const Move& mv);

/// @brief evaluates the position
/// @param bd
/// @return the evaluation (an int)
int eval(const Board& bd);

}  // namespace Evaluator

}  // namespace BobCheckers