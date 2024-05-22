#pragma once

#include "evaluation.hpp"
#include "move_generator.hpp"
#include "transposition.hpp"

namespace BobCheckers {

class Algorithm {

public:
  int movecount{0};

  /// @brief arbitrarily large number to use in place of infinity
  static constexpr int infinity = 1000000;

  /// @brief checkmate value
  static constexpr int game_over = 100000;

  static constexpr int quiescence_depth{3};

  static constexpr int LMR_R{2};

  static constexpr int start_reduction{4};

  static constexpr int cut_depth{1000};

  TranspositionTable &table;

  BoardStack board_stack;

  /// @brief Constructor for Algorithm
  /// @param bd
  /// @param tt
  Algorithm(Board bd, TranspositionTable &tt);

  /// @brief the main search function
  /// @param depth
  /// @return the best move
  Move search(int depth);

private:
  /// @brief the negamax algorithm
  /// @param depth
  /// @param alpha
  /// @param beta
  /// @return the result of the step of negamax
  /// @note called recursively
  int negamax(int depth, int alpha, int beta, const int color, const bool cut);

  /// @brief waits for the search to quiet down
  /// @param depth
  /// @param alpha
  /// @param beta
  /// @param color
  /// @return the quiescence value
  int quiescence(int depth, int alpha, int beta, int color);
};
} // namespace BobCheckers