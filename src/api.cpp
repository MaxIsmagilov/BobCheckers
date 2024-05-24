#include <chrono>
#include <iostream>
#include <tuple>

#include "algorithm.hpp"
#include "api.hpp"

namespace BobCheckers
{

void Game::join_loop() {
  std::string input;
  while (true) {
    std::getline(std::cin, input);
    if (input == "endgame") {
      break;
    }
    parse_input(input);
  }
}

void Game::play_game_at_depth(int depth) {
  parse_input("pcinewgame");
  parse_input("printboard");
  do {
    auto   start     = std::chrono::high_resolution_clock::now();
    auto   dn        = search_depth(depth);
    Move   best_move = std::get<0>(dn);
    int    value     = std::get<1>(dn);
    u64    nodecout  = std::get<2>(dn);
    auto   end       = std::chrono::high_resolution_clock::now();
    double ms_time   = std::chrono::duration<double, std::milli>(end - start).count();
    if (debug_enabled)
      std::cout << "do move " << best_move.print_move() << " info: " << value * (main_board.get_side() ? 1 : -1) << " "
                << nodecout << " nodes @ " << static_cast<double>(nodecout) / ms_time << "k nodes/sec" << std::endl;
    else
      std::cout << "do move " << best_move.print_move() << std::endl;
    if (!best_move.move_vec.empty()) {
      main_board.move(best_move);
    } else
      return;
    parse_input("printboard");
  } while (true);
}

void Game::load_pos(const std::string& fen) { main_board.load_pos(fen); }

void Game::make_move(const Move& mv) { main_board.move(mv); }

std::tuple<Move, int, u64> Game::search_depth(int depth) {
  Algorithm alg(main_board, tt);
  Move      best_move;
  for (int i = std::min(3, depth); i <= depth; ++i) {
    best_move = alg.search(i);
    if (debug_enabled)
      std::cout << "\tinfo: "
                << "depth: " << i << " move: " << best_move.print_move()
                << " score: " << alg.score * (main_board.get_side() ? 1 : -1) << " nodes: " << alg.movecount
                << std::endl;
  }
  return std::make_tuple(best_move, alg.score, alg.movecount);
}

void Game::parse_input(const std::string& input) {
  if (input == "isready") {
    std::cout << "readyok" << std::endl;
  } else if (input == "pcinewgame") {
    load_pos("1b1b1b1b/b1b1b1b1/1b1b1b1b/8/8/w1w1w1w1/1w1w1w1w/w1w1w1w1 w");
  } else if (input == "printboard") {
    std::cout << main_board.print_board() << std::endl;
  } else if (input.find("go depth") != std::string::npos) {
    std::string depth_str = input.substr(9);
    int         depth     = std::stoi(depth_str);
    auto        start     = std::chrono::high_resolution_clock::now();
    auto        dn        = search_depth(depth);
    Move        best_move = std::get<0>(dn);
    int         value     = std::get<1>(dn);
    u64         nodecout  = std::get<2>(dn);
    auto        end       = std::chrono::high_resolution_clock::now();
    double      ms_time   = std::chrono::duration<double, std::milli>(end - start).count();
    if (debug_enabled)
      std::cout << "do move " << best_move.print_move() << " info: " << value * (main_board.get_side() ? 1 : -1) << " "
                << nodecout << " nodes @ " << static_cast<double>(nodecout) / ms_time << "k nodes/sec" << std::endl;
    else
      std::cout << "do move " << best_move.print_move() << std::endl;
    if (!best_move.move_vec.empty()) {
      main_board.move(best_move);
    }
  } else if (input.find("move") != std::string::npos) {
    std::string move_sequence = input.substr(5);
    u32         endpos        = 0;
    while (endpos = move_sequence.find(" "), endpos != std::string::npos) {
      std::string move = move_sequence.substr(0, endpos);
      Move        mv(move);
      make_move(mv);
      move_sequence = move_sequence.substr(endpos + 1);
    }
    Move mv(move_sequence);
    std::cout << "move: " << mv.print_move() << std::endl;
    make_move(Move(move_sequence));
  }
}

void Game::set_debug(bool flag) { debug_enabled = flag; }
}  // namespace BobCheckers
