#pragma once

#include <array>
#include <string>

#include "move.hpp"
#include "utils.hpp"

namespace BobCheckers
{

/// @brief main board class
class Board
{
 public:
  /// @brief default constructor for Board
  Board();

  /// @brief const copy constructor for Board
  /// @param other
  Board(const Board& other);

  /// @brief default constructor for Board
  /// @param other
  Board(Board&& other);

  /// @brief assignment copy operator
  /// @param other
  void operator=(const Board& other);

  /// @brief [] (bracket) operator for board, allows for array-style access
  /// @param index
  /// @return a modifiable reference to the bitboard
  u32& operator[](int index);

  /// @brief [] (bracket) operator for const board, allows for array-style
  /// access
  /// @param index
  /// @return a value of the bitboard
  u32 operator[](int index) const;

  /// @brief gets the side to move
  /// @return 1 or 0 for white or black, respectively.
  u32 get_side() const;

  /// @brief flips the side to move
  void flip_side();
  /*
    /// @brief executes a move
    /// @tparam T should be an int type
    /// @tparam ...params should be an int type
    /// @param square1 the origin square
    /// @param square2 the second square
    /// @param ...other_squares any other squares
    template <typename T, u32... params>
    void move(u32 square1, u32 square2, params... other_squares);*/

  /// @brief executes a move
  /// @param mv
  void move(const Move& mv);

  /// @brief executes a move
  /// @param mv
  void move(Move&& mv);

  /// @brief loads a position
  /// @param position
  /// @note string is fromatted like a chess FEN,
  /// "1b1b1b1b/b1b1b1b1/1b1b1b1b/8/8/w1w1w1w1/1w1w1w1w/w1w1w1w1 w" for the
  /// starting position
  void load_pos(std::string position);

  /// @brief formats a board for printing
  /// @return a std::string representation of the board
  std::string print_board() const;

 private:
  /// @brief std::array of the bitboards,
  /// is formatted as {white men, white kings, black men, black kings}
  std::array<u32, 4> bitboards;

  /// @brief contains the side to move. 1 and 0 for white and black,
  /// respectively.
  u32 side_to_move;

  /// @brief executes one jump of the move
  /// @param square1 the origin square
  /// @param square2 the destination square
  void single_move(u32 square1, u32 square2);
};

}  // namespace BobCheckers