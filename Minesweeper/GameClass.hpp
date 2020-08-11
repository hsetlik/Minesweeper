//
//  GameClass.hpp
//  Minesweeper
//
//  Created by Hayden on 8/10/20.
//  Copyright © 2020 Hayden Setlik. All rights reserved.
#ifndef GameClass_hpp
#define GameClass_hpp

#include <stdio.h>
#include "/Library/Frameworks/SDL2.framework/Versions/A/Headers/SDL.h"
#include "/Library/Frameworks/SDL2_image.framework/Headers/SDL_image.h"
#include <fstream>
class Tile {
public:
    Tile();
    ~Tile();
    void init(int x, int y, bool hasMine);
    int xVal;
    int yVal;
    bool isHidden;
    bool hasMine;
    bool hasFlag;
    int minesAdjacent();
    SDL_Rect tileRect;
    int currentTexture;
    void setTexture();
private:
    int _topLeftXPx;
    int _topLeftYPx;
};


class Game{
public:
    Game();
    ~Game();
    SDL_Texture * mineTile;
    SDL_Texture * emptyTile;
    SDL_Texture * flagTile;
    SDL_Texture * hiddenTile;
    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    //handles user inputs and whatever the hell else
    void handleEvents();
    //updates all game objects
    void update();
    //draws appropriate graphics based on updated game logic
    void render();
    //manages memory
    void clean();
    bool running() {return isRunning;}
    Tile grid[16][16];
private:
    int _redlevel = 255;
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
};


#endif /* GameClass_hpp */
