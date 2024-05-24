#pragma once

#include <vector>

#include "board_stack.hpp"
#include "utils.hpp"

namespace BobCheckers
{

/// @brief move generator function object
class MoveGenerator
{
 public:
  /// @brief initialize move table
  static void init_move_table();

  /// @brief constructor for move generator
  /// @param bd
  MoveGenerator(Board&& bd);

  /// @brief constructor for move generator
  /// @param bd
  MoveGenerator(Board& bd);

  /// @brief get captures on a square
  /// @param square
  /// @return a vector of Move
  std::vector<Move> get_captures(const int square);

  /// @brief gets all captures
  /// @return a vector of Move
  std::vector<Move> get_all_captures();

  /// @brief gets all silent moves
  /// @return a vector of Move
  std::vector<Move> get_silents();

  /// @brief checks whether captures are available
  /// @return true of false
  bool captures_available();

  /// @brief gets all the moves available
  /// @return a vector of moves
  std::vector<Move> operator()();

 private:
  /// @brief the main boardstack
  BoardStack move_stack;

  /// @brief the board that is being investigated
  const Board board;

  bool m_captures_available{false};

  /// @brief silent moves table
  /// @note formatted as {white, black, king}
  static u32 silent_table[3][32];

  /// @brief capture table
  /// @note formatted as {white, black, king}
  static u32 capture_table[3][32];
};

}  // namespace BobCheckers