

#include "algorithm.hpp"
#include "evaluation.hpp"
#include "move_generator.hpp"
#include "transposition.hpp"

#include <chrono>
#include <iostream>

void setup() {
  BobCheckers::MoveGenerator::init_move_table();
  BobCheckers::TTUtils::generate_hash_keys();
}

int main() {

  setup();

  BobCheckers::Board bd;
  BobCheckers::TranspositionTable tt;

  bd.load_pos("1b1b1b1b/b1b1b1b1/1b1b1b1b/8/8/w1w1w1w1/1w1w1w1w/w1w1w1w1 w");

  BobCheckers::Algorithm alg(bd, tt);

  alg.search(6);

  return 0;
}