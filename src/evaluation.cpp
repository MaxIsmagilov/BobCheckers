#include "evaluation.hpp"

namespace BobCheckers {

/// @brief finds the heuristic of the move
/// @param piece
/// @param mv
/// @return an int
int Evaluator::heuristic(int piece, const Move &mv) {
  return [&]() -> int {
    switch (piece) {
    case 0:
      return man_values[mv.move_vec.back()] - man_values[mv.move_vec.front()];
    case 1:
      return king_values[mv.move_vec.back()] - king_values[mv.move_vec.front()];

    case 2:
      return man_values[flip_square(mv.move_vec.back())] -
             man_values[flip_square(mv.move_vec.front())];
    case 3:
      return king_values[flip_square(mv.move_vec.back())] -
             king_values[flip_square(mv.move_vec.front())];
    default:
      return 0;
    }
  }();
}

/// @brief evaluates the position
/// @param bd
/// @return the evaluation (an int)
int Evaluator::eval(const Board &bd) {
  // load bitboards into temp values
  u32 pieces[4] = {bd[0], bd[1], bd[2], bd[3]};

  // init value
  int val = 0;

  // go through all pieces
  for (int i = 0; i < 4; i++) {
    // calculate each piece from a set's value and add it to the value variable
    while (pieces[i]) {
      int square = utils::LSB_index(pieces[i]);
      pieces[i] = utils::pop_bit(pieces[i], square);
      val += piece_values[i];
      if (i == 0)
        val += man_values[square];
      else if (i == 1)
        val += king_values[square];
      else if (i == 2)
        val -= man_values[flip_square(square)];
      else if (i == 3)
        val -= king_values[flip_square(square)];
    }
  }

  // return the value
  return val * (bd.get_side() ? 1 : -1);
}

} // namespace BobCheckers