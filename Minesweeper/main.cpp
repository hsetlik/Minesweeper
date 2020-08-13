//
//  main.cpp
//  Minesweeper
//
//  Created by Hayden on 8/10/20.
//  Copyright © 2020 Hayden Setlik. All rights reserved.
#include <iostream>
#include"GameClass.hpp"
Game *game = nullptr;
Grid globalGrid;
int* groupX[7];
int* groupY[7];


int main(int argc, const char * argv[]) {
    game = new Game();
    game-> init("Minesweeper", 600, 600, 400, 400, false);
    while(game->running()){
        game->handleEvents();
        game->update();
        game->render();
    }
    game->clean();
    return 0;
}
