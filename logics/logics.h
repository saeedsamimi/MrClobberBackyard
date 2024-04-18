#pragma once
#include "../constants.h"
#include "../flagManager.h"
#include "stdio.h"
#include "../map.h"
#include <time.h>
#include "random.h"


short int dices[CAT_COUNT] = {-1};
short int playersTurn[CAT_COUNT];
short int priority[CAT_COUNT] = {-1};

unsigned short int REMAINING_FISHES = FISH_COUNT;
unsigned short int REMAINING_MOUSES = MOUSE_COUNT;
int canMove(int,int,enum MOVEMENT move);
void dogRandomMove();
void eat(int, int, int);
void mouseRandomMove();
int trap(int,int,int);

int canMove(int x, int y, enum MOVEMENT move) {
  if(x>BOARD_SIZE || y>BOARD_SIZE) return 0;
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
  if(current_character == DOG_CHARACTER && nextPos_character == CAT_CHARACTER) return 0; 
  return 0;
}

void dogRandomMove() {
  int direction_number;
  enum MOVEMENT movement;
  int new_x;
  int new_y;
  char sw = 0;
  // UP = 1; DOWN = 2;RIGHT = 3;LEFT = 4
  for (int dog_index = 0; dog_index < DOG_COUNT; dog_index++) {
    int x = dogs[dog_index].x;
    int y = dogs[dog_index].y;
    if(x > BOARD_SIZE || y>BOARD_SIZE) continue;
    for (int speed = 0; speed < dogs[dog_index].speed; speed++) {
      for (int i = 0; i < 5; i++)
      {
        x = dogs[dog_index].x;
        y = dogs[dog_index].y;
        new_x = x;
        new_y = y;
        direction_number = random_function(4) + 1;
        switch (direction_number)
        {
        case 1:
          //UP
          movement = UP;
          new_y = y - 1;
          break;
        case 2:
          //RIGHT
          movement = RIGHT;
          new_x = x + 1;
          break;
        case 3:
          //DOWN
          movement = DOWN;
          new_y = y + 1;
          break;
        case 4:
          //LEFT
          movement = LEFT;
          new_x = x - 1;
          break;
        }
        if (new_x < 0 || new_x >= BOARD_SIZE) new_x = x;
        if (new_y < 0 || new_y >= BOARD_SIZE) new_y = y;
        if (canMove(x, y, movement)) {
          sw = 1;
          break;
        }
      }
      if (sw) {
        dogs[dog_index].x = new_x;
        dogs[dog_index].y = new_y;
        addFlag(&map[new_y][new_x], FLAG_DOG);
        removeFlag(&map[y][x], FLAG_DOG);
      }
    }
  }
}

// Function for Cats to eat Chocolates and Fishes
void eat(int x,int y,int cat_index) {
  int current_home = map[y][x];
  // Eating Chocolate
  if(hasFlag(current_home,FLAG_CHOCO)) {
    cats[cat_index].attackPoint += 1;
    removeFlag(&map[y][x],FLAG_CHOCO);
  }
  // Eating Fish
  if(hasFlag(current_home,FLAG_FISH)) { 
    unsigned short int points;
    for(int i=0;i<FISH_COUNT;i++) {
      if(fishes[i].x == x && fishes[i].y ==y) {
        points = fishes[i].points;
        cats[cat_index].defencePoint += points;
        removeFlag(&map[y][x],FLAG_FISH);
        REMAINING_FISHES--;
        if(REMAINING_FISHES < CAT_COUNT) {
          clearFishes();
          __initFishes();
        }
      }
    }
  }
  //Eating Mouse
  if (hasFlag(current_home, FLAG_MOUSE)) {
    for (int i = 0; i < MOUSE_COUNT; i++) {
      if (mouses[i].x == x && mouses[i].y == y && mouses[i].points != INVALID_MOUSE_POINT) {
        unsigned short int mousePoints = mouses[i].points;
        cats[cat_index].mousePoint += mousePoints;
        cats[cat_index].mice_count++;
        mouses[i].points = INVALID_MOUSE_POINT;
        mouses[i].cat_index = cat_index;
        removeFlag(&map[y][x], FLAG_MOUSE);
        REMAINING_MOUSES--;
      }
    }
    //-------
  }
  int mouse_counter = 0;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (hasFlag(map[i][j], FLAG_MOUSE))
        mouse_counter++;
    }
  }
  //-----------
}

void releaseMice(int cat_number,int count) {
  for(int i=0;i<MOUSE_COUNT && count > 0;i++)
    if(mouses[i].cat_index == cat_number && mouses[i].points == INVALID_MOUSE_POINT) {
      // this is simpler
      mouses[i].points = mouses[i].mouse_type;
      // Select location for released mouses 
      MOUSE current_mouse = mouses[i];
      if(hasFlag(map[current_mouse.y][current_mouse.x],FLAG_CAT) || hasFlag(map[current_mouse.y][current_mouse.x],FLAG_MOUSE)) {
        int new_x,new_y;
        while(1) {
          new_x = random_function(15);
          new_y = random_function(15);
          if(!hasFlag(map[new_y][new_x],FLAG_CAT) && !hasFlag(map[new_y][new_x],FLAG_MOUSE)) break;
        }
        mouses[i].x = new_x;
        mouses[i].y = new_y;
        addFlag(&map[new_y][new_x],FLAG_MOUSE);
        mouses[i].cat_index = -1;
        count--;
        REMAINING_MOUSES++;
      }
      else {
        mouses[i].points = mouses[i].mouse_type;
        addFlag(&map[mouses[i].y][mouses[i].x], FLAG_MOUSE);
        mouses[i].cat_index = -1;
        count--;
        REMAINING_MOUSES++;
      }
      //------------------------------------
    }
}

void mouseRandomMove() {
  int direction_number;
  enum MOVEMENT movement = NO_MOVE;
  int new_x;
  int new_y;
  // UP = 1; DOWN = 2;RIGHT = 3;LEFT = 4
  for (int mouse_index = 0; mouse_index < REMAINING_MOUSES; mouse_index++) {
    if (mouses[mouse_index].points == INVALID_MOUSE_POINT) continue;
    int x = mouses[mouse_index].x;
    int y = mouses[mouse_index].y;
    for (int points = 0; points < mouses[mouse_index].points; points++) {
      for (int i = 0; i < 5; i++)
      {
        new_x = x;
        new_y = y;
        direction_number = random_function(8) + 1;
        switch (direction_number)
        {
        case 1:
          //UP
          movement = UP;
          new_y = y - 1;
          break;
        case 2:
          //RIGHT
          movement = RIGHT;
          new_x = x + 1;
          break;
        case 3:
          //DOWN
          movement = DOWN;
          new_y = y + 1;
          break;
        case 4:
          //LEFT
          movement = LEFT;
          new_x = x - 1;
          break;
        case 5:
          // UP_RIGHT
          movement = UP_RIGHT;
          new_x = x + 1;
          new_y = y - 1;
          break;
        case 6:
          //UP_LEFT
          movement = UP_LEFT;
          new_x = x - 1;
          new_y = y - 1;
          break;
        case 7:
          // DOWN_LEFT
          movement = DOWN_LEFT;
          new_x = x - 1;
          new_y = y + 1;
          break;
        case 8:
          // DOWN_RIGHT
          new_x = x + 1;
          new_y = y + 1;
          break;
        }
        if (new_x < 0 || new_x >= BOARD_SIZE) new_x = x;
        if (new_y < 0 || new_y >= BOARD_SIZE) new_y = y;
        if (canMove(x, y, movement)) break;
      }
      addFlag(&map[new_y][new_x], FLAG_MOUSE);
      removeFlag(&map[y][x], FLAG_MOUSE);
      x = new_x;
      y = new_y;
    }

    mouses[mouse_index].x = new_x;
    mouses[mouse_index].y = new_y;
  }
}

/// @brief Fight Function for Cats and Dogs
/// @param x x coordinate of Cat or Dog
/// @param y y Coordinate of Cat or Dog
/// @param type type of current item object(0 = cat & 1 = dog)
/// @param index index of current object in cats or dogs array
int fight(int x, int y, unsigned  int type, unsigned int index) {
  if (type == 0) { // Current Object is Cat
    // Cat vs Dog
    if (hasFlag(map[y][x], FLAG_DOG)) {
      DOG fighting_dog = dogs[0];
      int fighting_dog_index = 0;
      // Find Fighting Dog
      for (int dog_index = 0; dog_index < DOG_COUNT; dog_index++) {
        if (dogs[dog_index].x == x && dogs[dog_index].y == y) {
          fighting_dog = dogs[dog_index];
          fighting_dog_index = dog_index;
          break;
        }
      }
      // Fight Logic
      int catDefencePoint = cats[index].defencePoint;
      int catAttackPoint = cats[index].attackPoint;
      int dogDefencePoint = fighting_dog.defencePoint;
      int dogAttackPoint = fighting_dog.attackPoint;

      int catPoint = catDefencePoint * catAttackPoint;
      int dogPoint = dogDefencePoint * dogAttackPoint;

      if (catPoint >= dogPoint) {
        // Cat is winner
        // Reduce Cat power
        if(catDefencePoint == 0) catDefencePoint = 1; // Prevent division by zero error
        cats[index].defencePoint -= (dogDefencePoint / catDefencePoint) * dogAttackPoint;
        // Removing Dog
        fighting_dog.x = REMOVED_DOG;
        fighting_dog.y = REMOVED_DOG;
        removeFlag(&map[y][x], FLAG_DOG); //bug some fixed!
      }
      else if (dogPoint > catPoint) {
        //Dog is winner
        //Cat should be freeze
        cats[index].freeze = -3;
        //Cat & Dog power reduction
        cats[index].attackPoint = 2;
        cats[index].defencePoint = 5;
        if(dogDefencePoint ==0) dogDefencePoint = 1; // Prevent Division by zero error
        fighting_dog.defencePoint -= (catDefencePoint / dogDefencePoint) * catAttackPoint;
        // Releasing mice
        releaseMice(index, cats[index].mice_count);
        cats[index].mice_count = 0;
        cats[index].mousePoint = 0;
      }
      dogs[fighting_dog_index] = fighting_dog;
      return 0;
    }
    // Cat vs Cat
    if (hasFlag(map[y][x], FLAG_CAT)) {
      CAT fighting_cat = cats[0];
      int fighting_cat_index = -1;
      // Find Fighting cat
      for (int cat_index = 0; cat_index < CAT_COUNT; cat_index++) {
        if (cats[cat_index].x == x && cats[cat_index].y == y && cats[cat_index].freeze >= 0) {
          fighting_cat = cats[cat_index];
          fighting_cat_index = cat_index;
          break;
        }
      }
      if(fighting_cat_index != -1) {
        if (fighting_cat.freeze >= 0) {
          // Calculate points
          int curretCatPoint = cats[index].attackPoint * cats[index].defencePoint;
          int fightingCatPoint = fighting_cat.attackPoint * fighting_cat.defencePoint;
          printf("CURRENT CAT POINT: %d\nFIGHTING CAT POINT : %d\n", curretCatPoint, fightingCatPoint);
          if (curretCatPoint >= fightingCatPoint) {
            printf("CURRENT CAT WINS");
            // Current Player is winner
            // Freeze fighting Cat
            fighting_cat.freeze = -3;
            // Getting fighting cat mice
            cats[index].mice_count += fighting_cat.mice_count;
            // transporting mouses
            for (int mice_index = 0; mice_index < MOUSE_COUNT; mice_index++)
              if (mouses[mice_index].cat_index == fighting_cat_index)
                mouses[mice_index].cat_index = index;
            fighting_cat.mice_count = 0;
            cats[index].mousePoint += fighting_cat.mousePoint;
            fighting_cat.mousePoint = 0;
            // reducing power
            cats[index].defencePoint -= (fighting_cat.defencePoint / cats[index].defencePoint) * fighting_cat.attackPoint;
            if (cats[index].defencePoint > 65000) cats[index].defencePoint = 0;
            fighting_cat.attackPoint = 2;
            fighting_cat.defencePoint = 5;
          }
          else if (fightingCatPoint > curretCatPoint) {
            // Fighting cat is winner
            // Freeze current Cat
            cats[index].freeze = -3;
            // Getting current cat mice
            fighting_cat.mice_count += cats[index].mice_count;
            cats[index].mice_count = 0;
            fighting_cat.mousePoint += cats[index].mousePoint;
            cats[index].mousePoint = 0;
            // reducing power
            fighting_cat.defencePoint -= (cats[index].defencePoint / fighting_cat.defencePoint) * cats[index].attackPoint;
            if (fighting_cat.defencePoint > 65000) fighting_cat.defencePoint = 0;
            cats[index].attackPoint = 2;
            cats[index].defencePoint = 5;
          }
          cats[fighting_cat_index] = fighting_cat;
        }
      }
    }
  }
}

/// @brief Trap function for players
/// @param x x of item to be checked for trap
/// @param y y of item to be checked for trap
/// @param index index of current cat
/// return 0 if not trap enabled else return 1
int trap(int x, int y, int index) {
  if (hasFlag(map[y][x], FLAG_TRAP)) {
    if (cats[index].mice_count == 0) {
      if (cats[index].attackPoint > 2) {
        cats[index].attackPoint -= 2;
      }
      else {
        if(cats[index].defencePoint > 3){
          cats[index].defencePoint -= 3;
        }
        else {
          cats[index].defencePoint = 5;
          cats[index].attackPoint = 2;
          cats[index].freeze = -3;
        }
      }
      return 0;
    }
    else{
      //--------- Release Strongest mouse of current cat -------
      int max_mouse_point_index = -1; // Index of Strongest Mouse of current Cat
      int temp_max_mouse_point = 0; // Point of Strongest Mouse of current cat
      for (int mouse_index = 0; mouse_index < MOUSE_COUNT; mouse_index++) {
        if (mouses[mouse_index].points == INVALID_MOUSE_POINT) {
          if (mouses[mouse_index].cat_index == index) {
            if (mouses[mouse_index].mouse_type >= temp_max_mouse_point) {
              temp_max_mouse_point = mouses[mouse_index].mouse_type;
              max_mouse_point_index = mouse_index;
            }
          }
        }
      }
      if(max_mouse_point_index != -1) {
        // Release Strongest mouse of current cat
        mouses[max_mouse_point_index].cat_index = -1;
        mouses[max_mouse_point_index].points = mouses[max_mouse_point_index].mouse_type;
        //Check status for mouse location
        while (hasFlag(map[mouses[max_mouse_point_index].y][mouses[max_mouse_point_index].x], FLAG_MOUSE)) {
          int new_x = random_function(15);
          int new_y = random_function(15);
          mouses[max_mouse_point_index].x = new_x;
          mouses[max_mouse_point_index].y = new_y;
        }
        // Remove Mouse point and Mice count from cat
        cats[index].mice_count -= 1;
        cats[index].mousePoint -= mouses[max_mouse_point_index].mouse_type;
      }
      //---------------------------------------------------
    }
    
  }
  return 0;
}