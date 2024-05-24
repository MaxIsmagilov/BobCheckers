#include "utils.hpp"

namespace BobCheckers
{

u32 utils::indexto64(u32 index) { return (index * 2) + !((index / 4) % 2); }

u32 utils::indexto32(u32 index) { return ((index) / 2); }

bool utils::get_bit(u32 board, u32 square) { return (board & (1UL << square)); }

u32 utils::push_bit(u32 board, u32 square) { return (board | (1UL << square)); }

u32 utils::pop_bit(u32 board, u32 square) { return board & ~(1UL << square); }

std::string utils::print_bitboard(const u32& bb) {
  std::string str = "";
  for (int i = 0; i < 32; ++i) {
    if (!((i / 4) % 2) && i % 4 == 0) str += "  ";
    str += (get_bit(bb, i)) ? '1' : '0';
    str += "   ";
    if (i % 4 == 3) {
      str += '\n';
    }
  }
  return str;
}

u32 utils::count_bits(const u32& bitboard) {
  u32          test      = bitboard;
  unsigned int bitnumber = 0;
  while (test) {
    test &= test - 1;
    bitnumber++;
  }
  return bitnumber;
}

u32 utils::LSB_index(const u32& test) { return (test) ? utils::count_bits((test & -test) - 1) : 32; }

u32 utils::coordinates_to_index(const std::string& coordinate) {
  return utils::indexto32((coordinate[0] - 'a') + ('8' - coordinate[1]) * 8);
}

}  // namespace BobCheckers