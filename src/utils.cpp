#include "utils.hpp"

namespace BobCheckers::utils {

u32 indexto64(u32 index) { return (index * 2) + !((index / 4) % 2); }

u32 indexto32(u32 index) { return ((index) / 2); }

bool get_bit(u32 board, u32 square) { return (board & (1UL << square)); }

u32 push_bit(u32 board, u32 square) { return (board | (1UL << square)); }

u32 pop_bit(u32 board, u32 square) { return board & ~(1UL << square); }

std::string print_bitboard(const u32 &bb) {
  std::string str = "";
  for (int i = 0; i < 32; i++) {
    if (!((i / 4) % 2) && i % 4 == 0)
      str += "  ";
    str += (get_bit(bb, i)) ? '1' : '0';
    str += "   ";
    if (i % 4 == 3) {
      str += '\n';
    }
  }
  return str;
}

u32 count_bits(const u32 &bitboard) {
  u32 test = bitboard;
  unsigned int bitnumber = 0;
  while (test) {
    test &= test - 1;
    bitnumber++;
  }
  return bitnumber;
}

u32 LSB_index(const u32 &test) {
  return (test) ? count_bits((test & -test) - 1) : 32;
}

} // namespace BobCheckers::utils