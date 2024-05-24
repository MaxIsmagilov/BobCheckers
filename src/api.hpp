#pragma once

#include "algorithm.hpp"
#include "move.hpp"

namespace BobCheckers {

class Game {
public:
  Game() = default;

  /// @brief starts the game loop for the engin
  /// @note this is the main loop for the engine
  void join_loop();

  /// @brief plays a game at a certain depth
  /// @param depth
  void play_game_at_depth(int depth);

  /// @brief sets the debug flag
  /// @param flag
  void set_debug(bool flag);

private:
  void load_pos(const std::string &fen);

  void make_move(const Move &mv);

  std::tuple<Move, int, u64> search_depth(int depth);

  void parse_input(const std::string &input);

  Board main_board{};
  TranspositionTable tt{};

  bool debug_enabled{false};
};

} // namespace BobCheckers