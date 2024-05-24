#include "algorithm.hpp"

#include <iostream>

namespace BobCheckers {

Algorithm::Algorithm(const Board &bd, TranspositionTable &tt)
    : table(tt), board_stack(bd) {}

Move Algorithm::search(int depth) {
  movecount = 0;
  int alpha = -infinity;
  int beta = infinity;
  int value = -infinity;
  Move best_move;

  Board &bd = board_stack.get_top();
  MoveGenerator mg(bd);
  std::vector<Move> moveslist = mg();

  // std::cout << "depth: " << depth << std::endl;

  for (const Move &mw : moveslist) {
    board_stack.make_move(mw);
    int new_value = -negamax(depth - 1, -beta, -alpha, false);

    // std::cout << '\t' << "Move: " << mw.print_move() << " Value: " <<
    // new_value
    //           << std::endl;

    if (new_value > value) {
      value = new_value;
      best_move = mw;
    }
    alpha = std::max(alpha, value);
    board_stack.unmake_move();
  }

  score = value;

  return best_move;
}

int Algorithm::negamax(int depth, int alpha, int beta, const bool cut) {

  Board &bd = board_stack.get_top();
  MoveGenerator mg(bd);
  std::vector<Move> moveslist = mg();

  if (depth <= cut_depth && !mg.captures_available() && cut) {
    depth -= LMR_R;
  }

  bool can_cut = false;

  // increment total nodes
  ++movecount;

  // find a transpositon table entry
  TTUtils::TT_Entry entry = table.find(TTUtils::get_key(bd));
  if (entry._type != TTUtils::FAIL) {
    if (entry._type == TTUtils::EXACT)
      return entry._value;
    if (entry._type == TTUtils::LBOUND)
      alpha = std::max(alpha, entry._value);
    if (entry._type == TTUtils::UBOUND)
      beta = std::min(beta, entry._value);
    if (alpha >= beta)
      return entry._value;
  }

  // return the value if depth cutoff
  if (depth <= 0) {
    if (mg.captures_available())
      return quiescence(quiescence_depth, alpha, beta);
    else
      return Evaluator::eval(bd);
  }

  // if there are no moves, it is game over
  if (moveslist.size() == 0)
    return -(game_over + depth);

  // set an arbitrarily large negative number
  int value = -infinity;

  // store the original alpha
  int original_alpha = alpha;

  int original_beta = beta;

  // negamax through the move list
  for (u32 i = 0; i < moveslist.size(); ++i) {
    board_stack.make_move(moveslist[i]);

    value = std::max(value, -negamax(depth - 1, -beta, -alpha, can_cut));
    if (alpha < value && value < original_beta && i > 0) {
      value =
          std::max(value, -negamax(depth - 1, -original_beta, -value, can_cut));
    }
    alpha = std::max(value, alpha);
    board_stack.unmake_move();
    can_cut = true;
    if (alpha >= beta)
      break;
    beta = alpha + 1;
  }

  // create a new transposition table entry
  TTUtils::TT_Entry newEntry;
  newEntry._value = value;
  if (value <= original_alpha)
    newEntry._type = TTUtils::UBOUND;
  else if (value >= original_beta)
    newEntry._type = TTUtils::LBOUND;
  else
    newEntry._type = TTUtils::EXACT;
  table.add(newEntry);

  // return the value
  return alpha;
}

int Algorithm::quiescence(int depth, int alpha, int beta) {
  ++movecount;

  Board &bd = board_stack.get_top();
  MoveGenerator mg(bd);
  std::vector<Move> moveslist = mg();

  // return the value if node quiet or if depth = 0
  if (depth == 0 || !mg.captures_available())
    return Evaluator::eval(bd);

  // find a transpositon table entry
  TTUtils::TT_Entry entry = table.find(TTUtils::get_key(bd));
  if (entry._type != TTUtils::FAIL) {
    if (entry._type == TTUtils::EXACT)
      return entry._value;
    if (entry._type == TTUtils::LBOUND)
      alpha = std::max(alpha, entry._value);
    if (entry._type == TTUtils::UBOUND)
      beta = std::min(beta, entry._value);
    if (alpha >= beta)
      return entry._value;
  }

  // create a move vector

  // if there are no moves, it is game over
  if (moveslist.size() == 0)
    return -(game_over + depth);

  // set an arbitrarily large negative number
  int value = -infinity;

  // go through the move list
  for (Move mw : moveslist) {
    board_stack.make_move(mw);
    value = std::max(value, -quiescence(depth - 1, -beta, -alpha));
    alpha = std::max(value, alpha);
    board_stack.unmake_move();
    if (alpha >= beta)
      break;
  }

  return value;
}

} // namespace BobCheckers