#pragma once
#include "../constants.h"
#include "../flagManager.h"
#include "stdio.h"
#include "../map.h"

int canMove(int x, int y, enum MOVEMENT move) {
  int currentPos = map[y][x];
  int nextPos;
  enum CHARACTER_TYPE current_character;
  enum CHARACTER_TYPE nextPos_character = NO_CHARACTER;
  // Define next Position and Check for walls and Borders
  switch (move)
  {
  case UP:
    if (hasFlag(currentPos, FLAG_UWALL)) return 0; // Check For Walls
    if (y <= 0) return 0; // Check for Page Border
    nextPos = map[y-1][x];
    break;
  case DOWN:
    if (hasFlag(currentPos, FLAG_DWALL)) return 0;
    if (y + 1 >= BOARD_SIZE) return 0;
    nextPos = map[y + 1][x];
    break;
  case RIGHT:
    if (hasFlag(currentPos, FLAG_RWALL)) return 0;
    if (x + 1 >= BOARD_SIZE) return 0;
    nextPos = map[y][x + 1];
    break;
  case LEFT:
    if (hasFlag(currentPos, FLAG_LWALL)) return 0;
    if (x <= 0) return 0;
    nextPos = map[y][x - 1];
    break;
  case UP_RIGHT:
    if (hasFlag(currentPos, FLAG_RWALL) && hasFlag(currentPos, FLAG_UWALL)) return 0;
    if (y - 1 < 0 || x + 1 <= BOARD_SIZE) return 0;
    nextPos = map[y - 1][x + 1];
    break;
  case UP_LEFT:
    if (hasFlag(currentPos, FLAG_UWALL) && hasFlag(currentPos, FLAG_LWALL)) return 0;
    if (y - 1 < 0 || x - 1 < 0) return 0;
    nextPos = map[y - 1][x - 1];
    break;
  case DOWN_RIGHT:
    if (hasFlag(currentPos, FLAG_RWALL) && hasFlag(currentPos, FLAG_DWALL)) return 0;
    if (y + 1 >= BOARD_SIZE || x + 1 <= BOARD_SIZE) return 0;
    nextPos = map[y + 1][x + 1];
    break;
  case DOWN_LEFT:
    if (hasFlag(currentPos, FLAG_LWALL) && hasFlag(currentPos, FLAG_DWALL)) return 0;
    if (y + 1 >= BOARD_SIZE || x - 1 < 0) return 0;
    nextPos = map[y + 1][x - 1];
    break;
  }
  //----------- Set Current Character -----------
  if (hasFlag(currentPos, FLAG_DOG)) {
    current_character = DOG_CHARACTER;
  }
  if (hasFlag(currentPos, FLAG_CAT)) {
    current_character = CAT_CHARACTER;
  }
  if (hasFlag(currentPos, FLAG_MOUSE)) {
    current_character = MOUSE_CHARACTER;
  }
  //---------- Set Next Position Character --------
  printf("%d is the data of next pos", nextPos);
  if (hasFlag(nextPos, FLAG_DOG)) {
    nextPos_character = DOG_CHARACTER;
  }
  if (hasFlag(nextPos, FLAG_CAT)) {
    nextPos_character = CAT_CHARACTER;
  }
  if (hasFlag(nextPos, FLAG_MOUSE)) {
    nextPos_character = MOUSE_CHARACTER;
  }
  // Condition - Check Overlap for non-cat characters 
  // Cat's can be overlaped!
  if (nextPos_character == NO_CHARACTER) return 1;
  if (current_character == CAT_CHARACTER && current_character == nextPos_character) 
    return 1;
  return 0;
}


void dogRandomMove() {
    int direction_number;
    enum MOVEMENT movement;
    int new_x;
    int new_y;
    // UP = 1; DOWN = 2;RIGHT = 3;LEFT = 4
    for(int dog_index = 0; dog_index< DOG_COUNT;dog_index++) {
        int x = dogs[dog_index].x;
        int y = dogs[dog_index].y;
        for(int speed =0;speed<dogs[dog_index].speed;speed++) {           
          for(int i=0;i<5;i++)
          {
              new_x = x;
              new_y = y;
              direction_number = (rand() % (4)) + 1;
              switch (direction_number)
              {
              case 1:
                  //UP
                  movement = UP;
                  new_y = y-1;
                  break;
              case 2:
                  //RIGHT
                  movement = RIGHT;
                  new_x = x+1;
                  break;
              case 3:
                  //DOWN
                  movement = DOWN;
                  new_y = y+1;
                  break;
              case 4:
                  //LEFT
                  movement = LEFT;
                  new_x = x-1;
                  break;
              }
              if(canMove(x,y,movement)) break;
          }        
          addFlag(&map[new_y][new_x],FLAG_DOG);
          removeFlag(&map[y][x],FLAG_DOG);
          al_rest(0.3);
        }
        dogs[dog_index].x = new_x;
        dogs[dog_index].y = new_y;
    }
}

void mouseRandomMove() {
    int direction_number;
    enum MOVEMENT movement;
    int new_x;
    int new_y;
    // UP = 1; DOWN = 2;RIGHT = 3;LEFT = 4
    for(int mouse_index = 0; mouse_index< MOUSE_COUNT;mouse_index++) {
        int x = mouses[mouse_index].x;
        int y = mouses[mouse_index].y;
        
        for(int i=0;i<5;i++)
        {
            new_x = x;
            new_y = y;
            direction_number = (rand() % (4)) + 1;
            switch (direction_number)
            {
            case 1:
                //UP
                movement = UP;
                new_y = y-1;
                break;
            case 2:
                //RIGHT
                movement = RIGHT;
                new_x = x+1;
                break;
            case 3:
                //DOWN
                movement = DOWN;
                new_y = y+1;
                break;
            case 4:
                //LEFT
                movement = LEFT;
                new_x = x-1;
                break;
            }
            if(canMove(x,y,movement)) break;
        }
        
        addFlag(&map[new_y][new_x],FLAG_MOUSE);
        removeFlag(&map[y][x],FLAG_MOUSE);

        mouses[mouse_index].x = new_x;
        mouses[mouse_index].y = new_y;
    }
}