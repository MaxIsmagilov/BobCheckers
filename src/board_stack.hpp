#pragma once

#include "board.hpp"
#include "move.hpp"
#include <array>

namespace BobCheckers {

/// @brief manages a stack of boards, used for make/unmake move
class BoardStack {

public:
  /// @brief default constructor for BoardStack
  explicit BoardStack() noexcept;

  /// @brief copy constructor for BoardStack
  /// @param other
  BoardStack(const Board &other);

  /// @brief gets the board at the top of the stack
  /// @return a reference to the board
  Board &get_top();

  /// @brief move for the board stack
  /// @param mv
  void make_move(Move &mv);

  /// @brief move for the board stack
  /// @param mv
  void make_move(Move &&mv);

  /// @brief make a move without flipping sides
  /// @param mv
  void make_partial_move(Move &mv);

  /// @brief make a move without flipping sides
  /// @param mv
  void make_partial_move(Move &&mv);

  /// @brief undoes a move
  void unmake_move();

private:
  /// @brief a std::stack of boards
  std::array<Board, 256> boards;
  u32 top{0};
};

} // namespace BobCheckers