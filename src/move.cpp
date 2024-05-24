#include <iostream>

#include "move.hpp"

namespace BobCheckers
{

Move::Move() {}

Move::Move(const Move& other) {
  move_vec = other.move_vec;
  _hvalue  = other._hvalue;
}

Move::Move(const std::string& movename) : move_vec{} {
  // Convert UCI-like move notation to move_vec
  for (size_t i = 0; i < movename.length(); i += 2) {
    std::string coordinate = movename.substr(i, 2);
    // std::cout << utils::coordinates_to_index(coordinate) << std::endl;
    int index = utils::coordinates_to_index(coordinate);
    move_vec.push_back(index);
  }
}

std::string Move::print_move() const {
  if (move_vec.empty()) {
    return "0000";
  }
  std::string result = "";
  for (int i : move_vec) {
    result += utils::coordinates[utils::indexto64(i)];
  }
  return std::forward<std::string>(result);
}

std::string Move::print_move_info() const {
  std::string result = "";
  for (int i : move_vec) {
    result += utils::coordinates[utils::indexto64(i)];
  }
  result += " ";
  result += std::to_string(_hvalue);
  return std::forward<std::string>(result);
}

bool Move::operator<(const Move& other) const { return this->_hvalue < other._hvalue; }
bool Move::operator>(const Move& other) const { return this->_hvalue > other._hvalue; }

void Move::operator=(const Move& other) {
  move_vec = other.move_vec;
  _hvalue  = other._hvalue;
}

std::string Move::print_move_list(const std::vector<Move>& moves) {
  std::string result = "";
  for (const auto& m : moves) {
    result += m.print_move() + " ";
  }
  return std::forward<std::string>(result);
}

}  // namespace BobCheckers