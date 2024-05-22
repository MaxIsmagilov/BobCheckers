#pragma once

#include "utils.hpp"
#include <string>
#include <vector>

namespace BobCheckers {

/// @brief contains a move via a vector of the squares to move to
class Move {
public:
  /// @brief the vector containing Move moves
  std::vector<u32> move_vec;

  /// @brief sort value
  u32 _hvalue{0};

  /// @brief default constructor for Move
  Move();

  /// @brief copy constructor for Move
  /// @param other
  Move(const Move &other);

  /// @brief varaidic template constructor for Move
  /// @tparam ...params
  /// @param ...squares a list of squares, should be an int type
  template <typename... params> Move(params... squares);

  /// @brief formats the move for output
  /// @return a std::string coordinate move, eg. e3d4
  std::string print_move() const;

  bool operator<(const Move &other) const;

  bool operator>(const Move &other) const;
};

} // namespace BobCheckers