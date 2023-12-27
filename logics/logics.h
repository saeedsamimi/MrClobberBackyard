#pragma once
#include "../constants.h"
#include "../flagManager.h"
#include "stdio.h"
#include "../map.h"
#include <time.h>

int canMove(int,int,enum MOVEMENT);
void dogRandomMove();
void eat(int, int, int);
void mouseRandomMove();

int canMove(int x, int y, enum MOVEMENT move) {
  int currentPos = map[y][x];
  int nextPos = -1;
  int new_x =x ;
  int new_y =y;
  enum CHARACTER_TYPE current_character = NO_CHARACTER;
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
    if (y <= 0 || x + 1 >= BOARD_SIZE) return 0; //FIXED
    nextPos = map[y - 1][x + 1];
    break;
  case UP_LEFT:
    if (hasFlag(currentPos, FLAG_UWALL) && hasFlag(currentPos, FLAG_LWALL)) return 0;
    if (y <= 0 || x <= 0) return 0; //FIXED
    nextPos = map[y - 1][x - 1];
    break;
  case DOWN_RIGHT:
    if (hasFlag(currentPos, FLAG_RWALL) && hasFlag(currentPos, FLAG_DWALL)) return 0;
    if (y + 1 >= BOARD_SIZE || x + 1 >= BOARD_SIZE) return 0; //FIXED
    nextPos = map[y + 1][x + 1];
    break;
  case DOWN_LEFT:
    if (hasFlag(currentPos, FLAG_LWALL) && hasFlag(currentPos, FLAG_DWALL)) return 0;
    if (y + 1 >= BOARD_SIZE || x <= 0) return 0; //FIXED
    nextPos = map[y + 1][x - 1];
    break;
  }
  if(nextPos == -1) return 0;
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
  //printf("%d is the data of next pos", nextPos);
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
  if (current_character == CAT_CHARACTER) 
    return 1;
  if(current_character == nextPos_character) return 0;
  if(current_character == DOG_CHARACTER && nextPos_character == MOUSE_CHARACTER) return 0;
  if(nextPos_character == DOG_CHARACTER && current_character == MOUSE_CHARACTER) return 0;
  return 0;
}

void dogRandomMove() {
    int direction_number;
    enum MOVEMENT movement;
    int new_x;
    int new_y;
    srand(rand() - time(NULL));
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
              if(new_x <0 || new_x >= BOARD_SIZE) new_x = x;
              if(new_y <0 || new_y >= BOARD_SIZE) new_y = y;
              if(canMove(x,y,movement)) break;
          }        
          addFlag(&map[new_y][new_x],FLAG_DOG);
          removeFlag(&map[y][x],FLAG_DOG);
        }
        dogs[dog_index].x = new_x;
        dogs[dog_index].y = new_y;
    }
}

// Function for Cats to eat Chocolates and Fishes
void eat(int x,int y,int cat_index) {
  int current_home = map[y][x];
  if(hasFlag(current_home,FLAG_CHOCO)) {
    cats[cat_index].attackPoint += 1;
    removeFlag(&map[y][x],FLAG_CHOCO);
  }
  if(hasFlag(current_home,FLAG_FISH)) {
    unsigned short int points;
    for(int i=0;i<FISH_COUNT;i++) {
      if(fishes[i].x == x && fishes[i].y ==y) {
        points = fishes[i].points;
        cats[cat_index].defencePoint += points;
        removeFlag(&map[y][x],FLAG_FISH);
      }
    }
  }
}


void mouseRandomMove() {
    int direction_number;
    enum MOVEMENT movement = NO_MOVE;
    int new_x;
    int new_y;
    srand(rand() - time(NULL));
    // UP = 1; DOWN = 2;RIGHT = 3;LEFT = 4
    for(int mouse_index = 0; mouse_index< MOUSE_COUNT;mouse_index++) {
        int x = mouses[mouse_index].x;
        int y = mouses[mouse_index].y;
        for(int points=0;points<mouses[mouse_index].points;points++) {
          for(int i=0;i<5;i++)
          {
              new_x = x;
              new_y = y;
              direction_number = (rand() % (8)) + 1;
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
              case 5:
                // UP_RIGHT
                movement = UP_RIGHT;
                new_x = x+1;
                new_y = y-1;
                break;
              case 6:
                //UP_LEFT
                movement = UP_LEFT;
                new_x = x-1;
                new_y = y-1;
                break;
              case 7:
                // DOWN_LEFT
                movement = DOWN_LEFT;
                new_x = x-1;
                new_y = y+1;
                break;
              case 8:
              // DOWN_RIGHT
                new_x = x+1;
                new_y = y+1;
                break;
              }
              if(new_x <0 || new_x >= BOARD_SIZE) new_x = x;
              if(new_y <0 || new_y >= BOARD_SIZE) new_y = y;
              if(canMove(x,y,movement)) break;
          }
          x = new_x;
          y = new_y;
          addFlag(&map[new_y][new_x],FLAG_MOUSE);
          removeFlag(&map[y][x],FLAG_MOUSE);
        }

        mouses[mouse_index].x = new_x;
        mouses[mouse_index].y = new_y;
    }
}