//
//  randomGridGroups.cpp
//  Minesweeper
//
//  Created by Hayden on 8/12/20.
//  Copyright © 2020 Hayden Setlik. All rights reserved.
//

#include "randomGridGroups.hpp"

Group::Group(){
}
Group::~Group(){
}

void Group:: init(Tile firstTile, Grid grid){
    printf("group begun at: [%d][%d]\n", firstTile.xVal, firstTile.yVal);
    workingGrid = grid;
    members = 0;
    optionCount = 1;
    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            tilesFree[x][y] = false;
        }
    }
    int firstX = firstTile.xVal;
    int firstY = firstTile.yVal;
    printf("first space chosen at: [%d][%d]\n", firstX, firstY);
    workingGrid.gameGrid[firstX][firstY].inGroup = true;
    tilesFree[firstX][firstY] = false;
    _memberTiles[0] = workingGrid.gameGrid[firstX][firstY];
    members++;
    int doubleCheckX = _memberTiles[0].xVal;
    int doubleCheckY = _memberTiles[0].yVal;
    printf("first space assigned to _memberSpaces at: [%d][%d]\n", doubleCheckX, doubleCheckY);
    updateOptions();
    printf("Group initialized\n");
    
}
void Group::updateOptions(){
    printf("updateOptions started\n\n");
    for(int x = 0; x < 16; x++){
        for(int y = 0; y < 16; y++){
            if((workingGrid.areAdjacent(_memberTiles[members - 1],workingGrid.gameGrid[x][y])) &&
               (workingGrid.gameGrid[x][y].inGroup == false))
            {
                bool duplicateChecker = false;
                for(int i = 0; i < optionCount; i++){
                    int optX = _optionTiles[i].xVal;
                    int optY = _optionTiles[i].yVal;
                    if((optX == x )&&( optY == y)){
                        duplicateChecker = true;
                    }
                }
                if(duplicateChecker == false){
                printf("Free space at: [%d][%d]\n", x, y);
                tilesFree[x][y] = true;
                _optionTiles[optionCount] = workingGrid.gameGrid[x][y];
                optionCount++;
                }
            }
        }
    }
    printf("updateOptions finished\n\n");
}

void Group::addTile(){
    int spaceChoice = rand() % (optionCount);
    printf("Possible indeces: %d\n", optionCount);
    printf("Chosen space index: %d\n", spaceChoice);
    int testX = _optionTiles[spaceChoice].xVal;
    int testY = _optionTiles[spaceChoice].yVal;
    printf("X at chosen index: %d\n", testX);
    printf("Y at chosen indes: %d\n", testY);
    Tile chosenSpace = _optionTiles[spaceChoice];
    printf("chosen X: %d\n", chosenSpace.xVal);
    printf("chosen Y: %d\n", chosenSpace.yVal);
    int chosenX = chosenSpace.xVal;
    int chosenY = chosenSpace.yVal;
    tilesFree[chosenX][chosenY] = false;
    chosenSpace.inGroup = true;
    _memberTiles[members] = chosenSpace;
    members++;
}

void Group:: fillGroup(int size){
    for(int i = 0; i < size; i++){
        addTile();
        updateOptions();
    }
}
