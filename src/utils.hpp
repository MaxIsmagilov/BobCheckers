#pragma once

#include <string>

namespace BobCheckers {

/// @brief define the bitboard type
typedef unsigned long int u32;

/// @brief define the 64-bit type
typedef unsigned long long u64;

namespace utils {

/// @brief converts a 32-bit index to a 64-bit index
/// @param index
/// @return a 64-bit index
u32 indexto64(u32 index);

/// @brief converts a 64-bit index to a 32-bit index
/// @param index
/// @return a 32-bit index
u32 indexto32(u32 index);

/// @brief squares of pieces
enum square {
  _A8,
  _B8,
  _C8,
  _D8,
  _E8,
  _F8,
  _G8,
  _H8,
  _A7,
  _B7,
  _C7,
  _D7,
  _E7,
  _F7,
  _G7,
  _H7,
  _A6,
  _B6,
  _C6,
  _D6,
  _E6,
  _F6,
  _G6,
  _H6,
  _A5,
  _B5,
  _C5,
  _D5,
  _E5,
  _F5,
  _G5,
  _H5,
  _A4,
  _B4,
  _C4,
  _D4,
  _E4,
  _F4,
  _G4,
  _H4,
  _A3,
  _B3,
  _C3,
  _D3,
  _E3,
  _F3,
  _G3,
  _H3,
  _A2,
  _B2,
  _C2,
  _D2,
  _E2,
  _F2,
  _G2,
  _H2,
  _A1,
  _B1,
  _C1,
  _D1,
  _E1,
  _F1,
  _G1,
  _H1
};

/// @brief coordinate strings
const std::string coordinates[64] = {
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "a7", "b7", "c7",
    "d7", "e7", "f7", "g7", "h7", "a6", "b6", "c6", "d6", "e6", "f6",
    "g6", "h6", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a4",
    "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a3", "b3", "c3", "d3",
    "e3", "f3", "g3", "h3", "a2", "b2", "c2", "d2", "e2", "f2", "g2",
    "h2", "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"};

/// @brief finds whether a bit exists at an index
/// @param board
/// @param square
/// @return the bit (1 or 0 boolean)
bool get_bit(u32 board, u32 square);

/// @brief adds a bit to the bitboard
/// @param board
/// @param square
/// @return an updated bitboard
u32 push_bit(u32 board, u32 square);

/// @brief removes a bit from the bitboard
/// @param board
/// @param square
u32 pop_bit(u32 board, u32 square);

/// @brief prints a bitboard
/// @param bb
/// @return a string
std::string print_bitboard(const u32 &bb);

/// @brief counts the bits in a bitboard
/// @param bitboard
/// @return the number of bits
u32 count_bits(const u32 &bitboard);

/// @brief  finds the index of the least significant bit
/// @param test
/// @return the index
u32 LSB_index(const u32 &test);

/// @brief converts a coordinate to an index
/// @param coordinate
/// @return
u32 coordinates_to_index(const std::string &coordinate);

/// @brief 'a' file bitboard constant
constexpr u64 A_FILE = 0x0101010101010101ULL;

/// @brief 'b' file bitboard constant
constexpr u64 B_FILE = 0x0202020202020202ULL;

/// @brief 'g' file bitboard constant
constexpr u64 G_FILE = 0x4040404040404040ULL;

/// @brief 'h' file bitboard constant
constexpr u64 H_FILE = 0x8080808080808080ULL;

/// @brief eighth rank bitboard constant
constexpr u64 RANK_8 = 0xFFULL;

/// @brief seventh rank bitboard constant
constexpr u64 RANK_7 = 0xFF00ULL;

/// @brief second rank bitboard constant
constexpr u64 RANK_2 = 0xFF000000000000ULL;

/// @brief first rank bitboard constant
constexpr u64 RANK_1 = 0xFF00000000000000ULL;

} // namespace utils
} // namespace BobCheckers