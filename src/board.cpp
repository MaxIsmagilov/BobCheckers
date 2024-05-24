#include <cmath>

#include "board.hpp"

namespace BobCheckers
{

Board::Board() : bitboards{{0ULL, 0ULL, 0ULL, 0ULL}}, side_to_move{1} {}

Board::Board(const Board& other)
    : bitboards{{other[0], other[1], other[2], other[3]}}, side_to_move{other.get_side()} {}

Board::Board(Board&& other) : bitboards{{other[0], other[1], other[2], other[3]}}, side_to_move{other.get_side()} {}

void Board::operator=(const Board& other) {
  bitboards    = {other[0], other[1], other[2], other[3]};
  side_to_move = other.get_side();
}

u32& Board::operator[](int index) { return bitboards[index]; }

u32 Board::operator[](int index) const { return bitboards[index]; }

u32 Board::get_side() const { return side_to_move; }

void Board::flip_side() { side_to_move ^= 1; }

void Board::move(const Move& mv) {
  // just executes single moves one by one
  std::vector<u32> moves{std::move(mv.move_vec)};
  for (u32 i = 0; i < moves.size() - 1; i++) {
    single_move(moves[i], moves[i + 1]);
  }
  flip_side();
}

void Board::move(Move&& mv) {
  // just executes single moves one by one
  std::vector<u32> moves{std::move(mv.move_vec)};
  for (u32 i = 0; i < moves.size() - 1; i++) {
    single_move(moves[i], moves[i + 1]);
  }
  flip_side();
}

void Board::load_pos(std::string position) {
  bitboards = {0Ull, 0ULL, 0ULL, 0ULL};
  int index{0};
  int strpos{0};
  while (position[strpos] != ' ') {
    char token = position[strpos];
    if (token > '0' && token <= '9')
      index += token - '0';
    else if (token == '/')
      ;
    else {
      u32& bitboard = [&]() -> u32& {
        switch (token) {
          case 'w':
            return bitboards[0];
          case 'W':
            return bitboards[1];
          case 'b':
            return bitboards[2];
          case 'B':
            return bitboards[3];
        }
        return bitboards[0];
      }();
      bitboard |= 1ULL << utils::indexto32(index);
      index++;
    }
    strpos++;
  }
  side_to_move = (position[++strpos] == 'w');
}

std::string Board::print_board() const {
  // create pieces
  char piece_names[4]   = {'w', 'W', 'b', 'B'};
  char board_repr[8][8] = {};

  // fill the board representation with empty squares first
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) board_repr[i][j] = ' ';

  // fill the board representation
  for (int i = 0; i < 4; i++) {
    u32 bitboard = bitboards[i];
    while (bitboard) {
      int index                        = utils::LSB_index(bitboard);
      bitboard                         = utils::pop_bit(bitboard, index);
      index                            = utils::indexto64(index);
      board_repr[index / 8][index % 8] = piece_names[i];
    }
  }

  // create the board string
  std::string board = "    a   b   c   d   e   f   g   h   \n  +---+---+---+---+---+---+---+---+\n";

  // add the board
  for (int i = 0; i < 8; i++) {
    board += std::to_string(8 - i) + " ";
    for (int j = 0; j < 8; j++) board += "| " + std::string(1, board_repr[i][j]) + " ";
    board += "|\n  +---+---+---+---+---+---+---+---+\n";
  }

  // add the side to move
  board += "side to move: ";
  board += ((side_to_move == 1) ? "white\n" : "black\n");

  return board;
}

void Board::single_move(u32 square1, u32 square2) {
  // identify if the move captures
  bool capture = ((std::max(square1, square2) - std::min(square1, square2)) > 5);

  // find the starting and ending bits
  const u32 startbit = 1ULL << square1;
  const u32 endbit   = 1ULL << square2;

  // init piece variable
  int piece{-1};

  // init promotion variable
  bool promote{false};

  // find piece and promotion status
  // for white
  if (side_to_move) {
    if (startbit & bitboards[0])
      piece = 0;
    else
      piece = 1;
    promote = (piece == 0 && (utils::indexto64(square2) / 8 == 0));
  }

  // for black
  else {
    if (startbit & bitboards[2])
      piece = 2;
    else
      piece = 3;
    promote = (piece == 2 && (utils::indexto64(square2) / 8 == 7));
  }

  // remove the piece on the origin square
  bitboards[piece] &= ~startbit;

  // promote if needed
  if (promote) bitboards[piece + 1] |= endbit;

  // else dont promote
  else
    bitboards[piece] |= endbit;

  // remove any possible captured pieces if relevant
  if (capture) {
    bitboards[0] &= ~(1ULL << ((utils::indexto64(square1) + utils::indexto64(square2)) / 4));
    bitboards[1] &= ~(1ULL << ((utils::indexto64(square1) + utils::indexto64(square2)) / 4));
    bitboards[2] &= ~(1ULL << ((utils::indexto64(square1) + utils::indexto64(square2)) / 4));
    bitboards[3] &= ~(1ULL << ((utils::indexto64(square1) + utils::indexto64(square2)) / 4));
  }
}
}  // namespace BobCheckers