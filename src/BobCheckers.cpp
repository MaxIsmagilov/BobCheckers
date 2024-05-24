#include "algorithm.hpp"
#include "api.hpp"
#include "evaluation.hpp"
#include "move_generator.hpp"
#include "transposition.hpp"
#include "utils.hpp"

#include <chrono>
#include <iostream>

void setup() {
  BobCheckers::MoveGenerator::init_move_table();
  BobCheckers::TTUtils::generate_hash_keys();
}

int main() {

  setup();

  BobCheckers::Game game;

  game.set_debug(false);

  game.play_game_at_depth(16);

  // game.join_loop();

  return 0;
}