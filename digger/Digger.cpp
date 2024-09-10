#include <iostream>
#include "functions.h"

Game* Game::instance = nullptr;

int main(int argc, char* argv[]) {
    Game::instance = new Game();  
    Game::instance->Run(argc, argv);
    return 0;
}