#include "transposition.hpp"

namespace BobCheckers {

void TTUtils::generate_hash_keys() {
  std::default_random_engine generator;
  std::uniform_int_distribution<u64> distribution(0ULL, 0xFFFFFFFFFFFFFFFFULL);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 32; j++) {
      hash_keys[i][j] = distribution(generator);
    }
  }
}

const u64 TTUtils::get_key(const Board &bd) {
  u32 cpy[4] = {bd[0], bd[1], bd[2], bd[3]};
  u64 key = 0ULL;
  for (int i = 0; i < 4; i++) {
    while (cpy[i]) {
      int j = utils::LSB_index(cpy[i]);
      cpy[i] = utils::pop_bit(cpy[i], j);
      key ^= TTUtils::hash_keys[i][j];
    }
  }
  return key;
}

u64 TranspositionTable::get_index(const u64 key) const { return key % length; }

TranspositionTable::TranspositionTable() noexcept {
  std::fill(table.begin(), table.end(), failed_entry);
}

TTUtils::TT_Entry TranspositionTable::find(const u64 key) {
  return (key == table[get_index(key)]._key) ? table[get_index(key)]
                                             : failed_entry;
}

void TranspositionTable::add(TTUtils::TT_Entry &tte) {
  table[get_index(tte._key)] = std::forward<TTUtils::TT_Entry>(tte);
}

void TranspositionTable::add(TTUtils::TT_Entry &&tte) {
  table[get_index(tte._key)] = std::forward<TTUtils::TT_Entry>(tte);
}
} // namespace BobCheckers
