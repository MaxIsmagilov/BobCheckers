#include "move.hpp"

namespace BobCheckers {

Move::Move() {}

template <typename... params>
Move::Move(params... squares) : move_vec{squares...} {}

std::string Move::print_move() const {
  std::string result = "";
  for (int i : move_vec) {
    result += utils::coordinates[utils::indexto64(i)];
  }
  return std::forward<std::string>(result);
}

bool Move::operator<(const Move &other) const {
  return this->_hvalue < other._hvalue;
}
bool Move::operator>(const Move &other) const {
  return this->_hvalue > other._hvalue;
}

} // namespace BobCheckers