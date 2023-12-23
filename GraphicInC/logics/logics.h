#pragma once
#include "../constants.h"
#include "../flagManager.h"
#include "stdio.h"
#include "../map.h"

int canMove(int x,int y,enum MOVEMENT move) {
    int currentPos = map[x][y];
    int nextPos;
    enum CHARACTER_TYPE current_character;
    enum CHARACTER_TYPE nextPos_character;
    // Define next Position and Check for walls and Borders
    switch (move)
    {
        case UP:
            if(hasFlag(currentPos,FLAG_UWALL)) return 0; // Check For Walls
            if(x-1<0) return 0; // Check for Page Border
            nextPos = map[x-1][y];
            break;
        case DOWN:
            if(hasFlag(currentPos,FLAG_DWALL)) return 0;
            if(x+1>=BOARD_SIZE) return 0;
            nextPos = map[x+1][y];
            break;
        case RIGHT:
            if(hasFlag(currentPos,FLAG_RWALL)) return 0;
            if(y+1<= BOARD_SIZE) return 0;
            nextPos = map[x][y+1];
            break;
        case LEFT:
            if(hasFlag(currentPos,FLAG_LWALL)) return 0;
            if(y-1<0) return 0;
            nextPos = map[x][y-1];
            break;
        case UP_RIGHT:
            if(hasFlag(currentPos,FLAG_RWALL) && hasFlag(currentPos,FLAG_UWALL)) return 0;
            if(x-1<0 || y+1<= BOARD_SIZE) return 0;
            nextPos = map[x-1][y+1];
            break;
        case UP_LEFT:
            if(hasFlag(currentPos,FLAG_UWALL) && hasFlag(currentPos,FLAG_LWALL)) return 0;
            if(x-1<0 || y-1<0) return 0;
            nextPos = map[x-1][y-1];
            break;
        case DOWN_RIGHT:
            if(hasFlag(currentPos,FLAG_RWALL) && hasFlag(currentPos,FLAG_DWALL)) return 0;
            if(x+1>=BOARD_SIZE || y+1<= BOARD_SIZE) return 0;
            nextPos = map[x+1][y+1];
            break;
        case DOWN_LEFT:
            if(hasFlag(currentPos,FLAG_LWALL) && hasFlag(currentPos,FLAG_DWALL)) return 0;
            if(x+1>=BOARD_SIZE || y-1<0) return 0;
            nextPos = map[x+1][y-1];
            break;
        default:
            return 0;
            break;
    }
    //----------- Set Current Character -----------
    if(hasFlag(currentPos,FLAG_DOG)) {
        current_character = DOG_CHARACTER;
    }
    if(hasFlag(currentPos,FLAG_CAT)) {
        current_character = CAT_CHARACTER;
    }
    if(hasFlag(currentPos,FLAG_MOUSE)) {
        current_character = MOUSE_CHARACTER;
    }
    //---------- Set Next Position Character --------
    if(hasFlag(nextPos,FLAG_DOG)) {
        current_character = DOG_CHARACTER;
    }
    if(hasFlag(nextPos,FLAG_CAT)) {
        current_character = CAT_CHARACTER;
    }
    if(hasFlag(nextPos,FLAG_MOUSE)) {
        current_character = MOUSE_CHARACTER;
    }
    // Condition - Check Overlap for non-cat characters 
    // Cat's can be overlaped!
    if(current_character != CAT_CHARACTER && current_character == nextPos_character) {
        return 0;
    }
    return 1;
}