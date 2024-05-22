#include "board_stack.hpp"

namespace BobCheckers {

BoardStack::BoardStack() noexcept {}

BoardStack::BoardStack(const Board &other) { boards[0] = other; }

Board &BoardStack::get_top() { return boards[top]; }

void BoardStack::make_move(Move &mv) {
  boards[top + 1] = boards[top];
  boards[++top].move(mv);
}

void BoardStack::make_move(Move &&mv) {
  boards[top + 1] = boards[top];
  boards[++top].move(mv);
}

void BoardStack::make_partial_move(Move &mv) {
  boards[top + 1] = boards[top];
  boards[++top].move(mv);
  boards[top].flip_side();
}

void BoardStack::make_partial_move(Move &&mv) {
  boards[top + 1] = boards[top];
  boards[++top].move(mv);
  boards[top].flip_side();
}

void BoardStack::unmake_move() { --top; }

} // namespace BobCheckers