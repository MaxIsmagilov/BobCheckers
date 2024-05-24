#include "move_generator.hpp"

#include "evaluation.hpp"
#include <algorithm>

#include <iostream>

namespace BobCheckers {

void MoveGenerator::init_move_table() {
  u64 temp_table[3][64];
  for (int i = 0; i < 64; i++) {
    u64 pos = 1ULL << i;
    u64 table = 0ULL;
    if (pos & ~utils::A_FILE & ~utils::RANK_8)
      table |= 1ULL << (i - 9);
    if (pos & ~utils::H_FILE & ~utils::RANK_8)
      table |= 1ULL << (i - 7);
    temp_table[0][i] = table;
  }
  for (int i = 0; i < 64; i++) {
    u64 pos = 1ULL << i;
    u64 table = 0ULL;
    if (pos & ~utils::A_FILE & ~utils::RANK_1)
      table |= 1ULL << (i + 7);
    if (pos & ~utils::H_FILE & ~utils::RANK_1)
      table |= 1ULL << (i + 9);
    temp_table[1][i] = table;
  }
  for (int i = 0; i < 64; i++) {
    temp_table[2][i] = temp_table[0][i] | temp_table[1][i];
  }

  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 32; i++) {
      silent_table[j][i] = 0UL;
      for (int k = 0; k < 32; k++) {
        if (temp_table[j][utils::indexto64(i)] &
            (1ULL << utils::indexto64(k))) {
          silent_table[j][i] |= 1UL << k;
        }
      }
    }
  }

  for (int i = 0; i < 64; i++) {
    u64 pos = 1ULL << i;
    u64 table = 0ULL;
    if (pos & ~(utils::A_FILE | utils::B_FILE | utils::RANK_8 | utils::RANK_7))
      table |= 1ULL << (i - 18);
    if (pos & ~(utils::H_FILE | utils::G_FILE | utils::RANK_8 | utils::RANK_7))
      table |= 1ULL << (i - 14);
    temp_table[0][i] = table;
  }
  for (int i = 0; i < 64; i++) {
    u64 pos = 1ULL << i;
    u64 table = 0ULL;
    if (pos & ~(utils::A_FILE | utils::B_FILE | utils::RANK_2 | utils::RANK_1))
      table |= 1ULL << (i + 14);
    if (pos & ~(utils::H_FILE | utils::G_FILE | utils::RANK_2 | utils::RANK_1))
      table |= 1ULL << (i + 18);
    temp_table[1][i] = table;
  }
  for (int i = 0; i < 64; i++) {
    temp_table[2][i] = temp_table[0][i] | temp_table[1][i];
  }
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 32; i++) {
      capture_table[j][i] = 0UL;
      for (int k = 0; k < 32; k++) {
        if (temp_table[j][utils::indexto64(i)] &
            (1ULL << utils::indexto64(k))) {
          capture_table[j][i] |= 1UL << k;
        }
      }
    }
  }
}

MoveGenerator::MoveGenerator(Board &&bd) : move_stack{bd}, board{bd} {}

MoveGenerator::MoveGenerator(Board &bd) : move_stack{bd}, board{bd} {}

std::vector<Move> MoveGenerator::get_captures(const int square) {
  // grab the board state
  const Board &bd = move_stack.get_top();

  // init move vector
  std::vector<Move> moves;

  // init constants
  const int aside = (bd.get_side()) ? 0 : 2;
  const int dside = (bd.get_side()) ? 2 : 0;
  const u32 defending_occ = bd[0 + dside] | bd[1 + dside];
  const u32 attacking_occ = bd[0 + aside] | bd[1 + aside];
  const u32 all_occ = defending_occ | attacking_occ;

  // create man moves
  u32 bb = (1UL << square) & bd[aside];
  while (bb) {
    // lookup attack value
    const int start = square;
    bb = utils::pop_bit(bb, start);
    u32 att = capture_table[!(bd.get_side())][start];

    // go through attacks and parse
    while (att) {
      // get a possible end square
      const int end = utils::LSB_index(att);
      att = utils::pop_bit(att, end);

      // parse move if it captures a piece and moves to an available square
      if (((1UL << end) & ~all_occ) &&
          ((1UL << utils::indexto32(
                (utils::indexto64(start) + utils::indexto64(end)) / 2)) &
           defending_occ)) {
        // make a partial move and recursively find next captures
        move_stack.make_partial_move(Move(start, end));
        std::vector<Move> temp = get_captures(end);

        // if there are no future captures, continue
        if (temp.size() == 0 || end / 4 == 0 || end / 4 == 7)
          moves.emplace_back(start, end);

        // else concatenate the future moves to the current ones
        else
          for (Move mw : temp) {
            moves.emplace_back(start, end);
            std::for_each(mw.move_vec.begin() + 1, mw.move_vec.end(),
                          [&](int i) { moves.back().move_vec.push_back(i); });
          }

        // unmake the stack move
        move_stack.unmake_move();
      }
    }
  }

  // create king moves
  bb = (1UL << square) & bd[aside + 1];
  while (bb) {
    // lookup attack value
    const int start = square;
    bb = utils::pop_bit(bb, start);
    u32 att = capture_table[2][start];

    // go through attacks and parse
    while (att) {
      // get a possible end square
      const int end = utils::LSB_index(att);
      att = utils::pop_bit(att, end);

      // parse move if it captures a piece and moves to an available square
      if (((1UL << end) & ~all_occ) &&
          ((1UL << utils::indexto32(
                (utils::indexto64(start) + utils::indexto64(end)) / 2)) &
           defending_occ)) {
        // make a partial move and recursively find next captures
        move_stack.make_partial_move(Move(start, end));
        std::vector<Move> temp = get_captures(end);

        // if there are no future captures, continue
        if (temp.size() == 0)
          moves.emplace_back(start, end);

        // else concatenate the future moves to the current ones
        else
          for (Move mw : temp) {
            moves.emplace_back(start, end);
            std::for_each(mw.move_vec.begin() + 1, mw.move_vec.end(),
                          [&](int i) { moves.back().move_vec.push_back(i); });
          }

        // unmake the stack move
        move_stack.unmake_move();
      }
    }
  }

  // return the resultant vector
  return moves;
}

std::vector<Move> MoveGenerator::get_all_captures() {
  // create final and piece-wise move vectors
  std::vector<Move> moves;
  std::vector<Move> newmoves;

  // go through squares and find captures
  for (int i = 0; i < 32; i++) {
    // get captures on that square
    newmoves = get_captures(i);

    const int piece = [&]() -> int {
      if (board[0] & (1UL << i))
        return 0;
      if (board[1] & (1UL << i))
        return 1;
      if (board[2] & (1UL << i))
        return 2;
      if (board[3] & (1UL << i))
        return 3;
      return 0;
    }();

    // add all new moves to the final vector
    for (Move mw : newmoves) {
      moves.push_back(std::forward<Move>(mw));
      moves.back()._hvalue = Evaluator::heuristic(piece, moves.back()) *
                             moves.back().move_vec.size();
    }
  }

  // return the vector
  return moves;
}

std::vector<Move> MoveGenerator::get_silents() {
  // create blank move vector
  std::vector<Move> moves;

  // create helpful constants
  const int aside = (board.get_side()) ? 0 : 2;
  const int dside = (board.get_side()) ? 2 : 0;
  const u32 defending_occ = board[0 + dside] | board[1 + dside];
  const u32 attacking_occ = board[0 + aside] | board[1 + aside];
  const u32 all_occ = defending_occ | attacking_occ;

  // check man bitboard
  u32 bb = board[aside];
  while (bb) {
    // pop the least significant bit and use it
    // to look up the move value
    int start = utils::LSB_index(bb);
    bb = utils::pop_bit(bb, start);
    u32 att = silent_table[!(board.get_side())][start];

    // parse the moves
    while (att) {
      // find the end square
      int end = utils::LSB_index(att);
      att = utils::pop_bit(att, end);

      // if the end square is not occupied, consider it valid
      if ((1UL << end) & ~all_occ) {
        moves.emplace_back(start, end);
        moves.back()._hvalue = Evaluator::heuristic(aside, moves.back());
      }
    }
  }

  // check king bitboard
  bb = board[aside + 1];
  while (bb) {
    // pop the least significant bit and use it
    // to look up the move value
    int start = utils::LSB_index(bb);
    bb = utils::pop_bit(bb, start);
    u32 att = silent_table[2][start];

    // parse the moves
    while (att) {
      // find the end square
      int end = utils::LSB_index(att);
      att = utils::pop_bit(att, end);

      // if the end square is not occupied, consider it valid
      if ((1UL << end) & ~all_occ) {
        moves.emplace_back(start, end);
        moves.back()._hvalue = Evaluator::heuristic(aside + 1, moves.back());
      }
    }
  }

  // return the resultant vector
  return moves;
}

bool MoveGenerator::captures_available() { return m_captures_available; }

std::vector<Move> MoveGenerator::operator()() {
  std::vector<Move> moves = get_all_captures();

  m_captures_available = true;

  // if there are no captures, set the moves as the silent moves
  // this ensures that if a capture is available, only captures will be
  // considered
  if (moves.size() == 0) {
    moves = get_silents();
    m_captures_available = false;
  }

  std::sort(moves.begin(), moves.end(), std::greater<>());

  // return the resultant vector
  return moves;
}

u32 MoveGenerator::silent_table[3][32] = {};

u32 MoveGenerator::capture_table[3][32] = {};

} // namespace BobCheckers