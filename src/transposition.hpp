#pragma once

#include <array>
#include <random>

#include "board.hpp"
#include "utils.hpp"

namespace BobCheckers
{

namespace TTUtils
{
/// @brief hash keys for table
extern u64 hash_keys[4][32];

/// @brief initializing method to generate hash keys
void generate_hash_keys();

/// @brief calculates the key for a board
/// @param bd
/// @return the hash key
u64 get_key(const Board& bd);

/// @brief entry type enum
enum tt_entry_type : char { EXACT, LBOUND, UBOUND, FAIL };

/// @brief transposition table structure
struct TT_Entry
{
  u64           _key{0ULL};
  int           _value{0};
  tt_entry_type _type;
};
}  // namespace TTUtils

/// @brief size of the transposition table, currently ~2 GB
static constexpr u64 tablesize = sizeof(TTUtils::TT_Entry) * 131072;

/// @brief failed entry constant
static constexpr TTUtils::TT_Entry failed_entry{0ULL, 0, TTUtils::FAIL};

/// @brief transposition table class
class TranspositionTable
{
 private:
  /// @brief the table, an array of the TT_Entry type
  std::array<TTUtils::TT_Entry, tablesize / sizeof(TTUtils::TT_Entry)> table;

  /// @brief
  u64 length{tablesize / sizeof(TTUtils::TT_Entry)};

  /// @brief helper function to find the index in which the entry is stored
  /// @param tte
  /// @return the index
  u64 get_index(const u64 key) const;

 public:
  /// @brief constructor for transposition table
  explicit TranspositionTable() noexcept;

  /// @brief finds the desired entry
  /// @param key
  /// @return the entry or the failed_entry constant
  TTUtils::TT_Entry find(const u64 key);

  /// @brief adds an entry to the transposition table
  /// @param tte
  void add(TTUtils::TT_Entry& tte);

  /// @brief adds an entry to the transposition table
  /// @param tte
  void add(TTUtils::TT_Entry&& tte);
};

}  // namespace BobCheckers
