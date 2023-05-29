
#include "BobCheckersBackend.hh"
#include "BobCheckersAPI.hh"
#include <iostream>
#include <chrono>
#include <thread>

void setup()
{
    Bob_checkers::move_generator::init_move_table();
    Bob_checkers::tt_util::generate_hash_keys();
}

int main()
{
    
    setup();
    
    Bob_checkers::PCI::PCI_loop();
    
    return 0;
}