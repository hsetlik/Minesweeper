//
//  GameClass.cpp
//  Minesweeper
//
//  Created by Hayden on 8/10/20.
//  Copyright © 2020 Hayden Setlik. All rights reserved.
//
#include "GameClass.hpp"
#include <cmath>

Game::Game() {
    }
Game::~Game() {
    }
void Game::init(const char *title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if(fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        IMG_Init(IMG_INIT_PNG);
        printf("SDL_Img initialized\n");
        //verifies that SDL started up correctly
        printf("SDL Initialized\n");
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if(window){
            //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            printf("Window Created\n");
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer){
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            printf("Renderer Created\n");
        }
        isRunning = true;
    }
    //create a temporary surface for the image
    SDL_Surface *tempSurfaceEmpty = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/TwoDEngine/TwoDEngine/PNG_Assets/hidden_tileQuarter.png");
    //convert that temporary surface to a texture for actual use
    emptyTile = SDL_CreateTextureFromSurface(renderer, tempSurfaceEmpty);
    //gets rid of that temporary surface
    SDL_FreeSurface(tempSurfaceEmpty);
    //doing the above for the rest of the PNGs
    //the hidden tile
    SDL_Surface *tempSurfaceHidden = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/TwoDEngine/TwoDEngine/PNG_Assets/empty_tileQuarter.png");
    hiddenTile = SDL_CreateTextureFromSurface(renderer, tempSurfaceHidden);
    SDL_FreeSurface(tempSurfaceHidden);
    //the flag tile
    SDL_Surface *tempSurfaceFlag = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/TwoDEngine/TwoDEngine/PNG_Assets/flag_tileQuarter.png");
    flagTile = SDL_CreateTextureFromSurface(renderer, tempSurfaceFlag);
    SDL_FreeSurface(tempSurfaceFlag);
    //the mine tile
    SDL_Surface *tempSurfaceMine = IMG_Load("/Users/SFMAdmin/Desktop/Programming/SDL_projects/TwoDEngine/TwoDEngine/PNG_Assets/mine_tileQuarter.png");
    mineTile = SDL_CreateTextureFromSurface(renderer, tempSurfaceMine);
    SDL_FreeSurface(tempSurfaceMine);
    printf("PNG files loaded to textures\n");
    grid.init();
    printf("Grid initialized\n");
}
void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT: {
            isRunning = false;
            break;
        }
        case SDL_MOUSEBUTTONDOWN:{
            double _clickX;
            double _clickY;
            int clickXpx, clickYpx;
            SDL_GetMouseState(&clickXpx, &clickYpx);
            _clickX = clickXpx/25;
            _clickY = clickYpx/25;
            int clickX = round(_clickX);
            int clickY = round(_clickY);
            switch (event.button.button) {
                case SDL_BUTTON_LEFT:
                    grid.leftClickAt(clickX, clickY);
                    break;
                case SDL_BUTTON_RIGHT:
                    grid.rightClickAt(clickX, clickY);
                    break;
                default:
                    break;
            }
            
            
            //do stuff when the mouse is clicked
            break;
        }
        default: {
            break;
        }
    }
}
void Game::update()
{
    grid.updateTextures();
    //printf("Textures updated\n");
}
void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();
    printf("Game Cleaned\n");
}

void Game::render(){
    SDL_RenderClear(renderer);
    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            int texture = grid.gameGrid[x][y].currentTexture;
                //printf("Texture for [%d][%d]: %d\n", x, y, texture);
                SDL_Rect currentRect;
                currentRect.w = 25;
                currentRect.h = 25;
                currentRect.x = 25 * x;
                currentRect.y = 25 * y;
            switch (texture) {
                case 0:
                    SDL_RenderCopy(renderer, hiddenTile, NULL, &currentRect);
                    break;
                case 1:
                SDL_RenderCopy(renderer, flagTile, NULL, &currentRect);
                break;
                case 2:
                SDL_RenderCopy(renderer, mineTile, NULL, &currentRect);
                break;
                case 3:
                    SDL_RenderCopy(renderer, emptyTile, NULL, &currentRect);
                    break;
                /*default:
                  SDL_RenderCopy(renderer, hiddenTile, NULL, &currentRect);
                  break;*/
            }
        }
    }
    SDL_RenderPresent(renderer);
}
// Time for the Tile Class
Tile::Tile(){
}
Tile::~Tile(){
}
void Tile::init(int x, int y){
    xVal = x;
    yVal = y;
    isHidden = true;
    hasFlag = false;
    hasMine = false;
    revealed = false;
    reservedEmpty = false;
    _topLeftXPx = xVal * 25;
    _topLeftYPx = yVal * 25;
    tileRect.x = _topLeftXPx;
    tileRect.y = _topLeftYPx;
    tileRect.h = 25;
    tileRect.w = 25;
}
void Tile::setTexture(){
    if(isHidden){
        currentTexture = 0;
    } else if(hasFlag){
        currentTexture = 1;
    } else if(hasMine) {
        currentTexture = 2;
    } else if(revealed){
        currentTexture = 3;
    }
    }

//Grid stuff

Grid::Grid(){
}
Grid::~Grid(){
}
void Grid::init(){
    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            gameGrid[x][y].init(x, y);
            gameGrid[x][y].setTexture();
            printf("Texture at [%d][%d]: %d\n", x, y, gameGrid[x][y].currentTexture);
            }
        }
    minesSet = false;
    }

bool Grid:: areAdjacent(Tile tileA, Tile tileB){
    //making a list of all 8 adjacent tiles
    Tile adjacentSpaces[8];
    adjacentSpaces[0] = gameGrid[tileA.xVal - 1][tileA.yVal -1] ; //top left
    adjacentSpaces[1] = gameGrid[tileA.xVal][tileA.yVal -1]; //top center
    adjacentSpaces[2] = gameGrid[tileA.xVal + 1][tileA.yVal -1]; // top right
    adjacentSpaces[3] = gameGrid[tileA.xVal +1][tileA.yVal]; // right side
    adjacentSpaces[4] = gameGrid[tileA.xVal + 1][tileA.yVal + 1]; // bottom right
    adjacentSpaces[5] = gameGrid[tileA.xVal][tileA.yVal + 1]; // bottom center
    adjacentSpaces[6] = gameGrid[tileA.xVal - 1][tileA.yVal + 1]; // bottom left
    adjacentSpaces[7] = gameGrid[tileA.xVal - 1][tileA.yVal];// left side
    for(int i = 0; i < 8; i++){
        if(adjacentSpaces[i].xVal == tileB.xVal){
            if(adjacentSpaces[i].yVal == tileB.yVal){
                return true;
                break;
            }
        }
    }
    return false;
}

int Grid::minesAdjacentTo(Tile tile){
    int count = 0;
    Tile adjacentSpaces[8];
    adjacentSpaces[0] = gameGrid[tile.xVal - 1][tile.yVal -1] ; //top left
    adjacentSpaces[1] = gameGrid[tile.xVal][tile.yVal -1]; //top center
    adjacentSpaces[2] = gameGrid[tile.xVal + 1][tile.yVal -1]; // top right
    adjacentSpaces[3] = gameGrid[tile.xVal +1][tile.yVal]; // right side
    adjacentSpaces[4] = gameGrid[tile.xVal + 1][tile.yVal + 1]; // bottom right
    adjacentSpaces[5] = gameGrid[tile.xVal][tile.yVal + 1]; // bottom center
    adjacentSpaces[6] = gameGrid[tile.xVal - 1][tile.yVal + 1]; // bottom left
    adjacentSpaces[7] = gameGrid[tile.xVal - 1][tile.yVal];// left side
    for(int i = 0; i < 8; i++){
        if(adjacentSpaces[i].hasMine){
            count += 1;
        }
    }
    return count;
}

void Grid::updateTextures() {
    for (int x = 0; x < 16; x++) {
        for(int y = 0; y < 16; y ++){
            gameGrid[x][y].setTexture();
        }
    }
}

Tile* Grid::randomAdjacentBlock(Tile tile){
    //returns a pointer to any one of the 8 squares adjacent to the starting tile
    Tile *adjacentSpaces[8];
    adjacentSpaces[0] = &gameGrid[tile.xVal - 1][tile.yVal -1] ; //top left
    adjacentSpaces[1] = &gameGrid[tile.xVal][tile.yVal -1]; //top center
    adjacentSpaces[2] = &gameGrid[tile.xVal + 1][tile.yVal -1]; // top right
    adjacentSpaces[3] = &gameGrid[tile.xVal +1][tile.yVal]; // right side
    adjacentSpaces[4] = &gameGrid[tile.xVal + 1][tile.yVal + 1]; // bottom right
    adjacentSpaces[5] = &gameGrid[tile.xVal][tile.yVal + 1]; // bottom center
    adjacentSpaces[6] = &gameGrid[tile.xVal - 1][tile.yVal + 1]; // bottom left
    adjacentSpaces[7] = &gameGrid[tile.xVal - 1][tile.yVal];// left side
    //random int between 0 and 7 to choose which adjacent square gets pointed to
    int randIndex = rand() % 8;
    return adjacentSpaces[randIndex];
}


void Grid::setEmptyBlock(int originX, int originY, int numSpaces){
    Tile* emptyGroup[24];
    int assigned = 0;
    emptyGroup[0] = &gameGrid[originX][originY];
    //gameGrid[originX][originY].revealed = true;
    assigned++;
    while(assigned < numSpaces){
        int randIndex = rand() % assigned;
        Tile* pNewTile;
        pNewTile = randomAdjacentBlock(*(emptyGroup[randIndex]));
        emptyGroup[assigned] = pNewTile;
        
    }
    /* array of tile pointers
     initialize the first of the array
     increment the count of assigned spaces
    loop: while the number of tiles assigned < numSpaces(
     1. choose which of the already assigned spaces to start from (need an array of tile pointers which gets modified with each iteration
     2. pick a random tile adjacent to that space
     3. assign that tile
     4. increment the number of assigned tiles
    )*/
}

    

void Grid::revealFirstEmpties(){
    printf("ready to reveal empties\n");
    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            if(gameGrid[x][y].reservedEmpty){
                printf("reserved empty found at: [%d][%d]\n", x, y);
                gameGrid[x][y].isHidden = false;
                gameGrid[x][y].revealed = true;
            }
        }
    }
}

void Grid::initializeMines(int clickX, int clickY){
    printf("initializing mines. . .\n");
    setEmptyBlock(clickX, clickY, 7);
    int totalMines = 40;
    for(int i = 0; i < totalMines; i++){
        int xChoice = rand() % 16;
        int yChoice = rand() % 16;
        if(gameGrid[xChoice][yChoice].reservedEmpty){
            i++;
            break;
        } else {
            gameGrid[xChoice][yChoice].hasMine = true;
            //these lines only for testing
            gameGrid[xChoice][yChoice].isHidden = false;
        }
    }
    revealFirstEmpties();
    minesSet = true;
    printf("mines initialized\n");
}
    
void Grid:: rightClickAt(int clickX, int clickY){
    printf("right click\n");
    if(minesSet == false){
        initializeMines(clickX, clickY);
    } else if(gameGrid[clickX][clickY].isHidden){
        gameGrid[clickX][clickY].hasFlag = true;
        gameGrid[clickX][clickY].isHidden = false;
    }
}

void Grid::leftClickAt(int clickX, int clickY){
    printf("left click\n");
    if(minesSet == false){
        initializeMines(clickX, clickY);
    } else {
        gameGrid[clickX][clickY].isHidden = false;
        gameGrid[clickX][clickY].revealed = true;
        printf("texture of clicked space: %d\n", gameGrid[clickX][clickY].currentTexture);
    }
}
