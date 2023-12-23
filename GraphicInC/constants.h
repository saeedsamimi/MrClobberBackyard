#pragma once
#include <allegro5/allegro_color.h>
#define BOARD_SIZE 15
#define SQUARE_SIZE 50
#define MARGIN 4.5

ALLEGRO_COLOR BLUE;
ALLEGRO_COLOR GREEN;
ALLEGRO_COLOR RED;
ALLEGRO_COLOR MAGENTA;
ALLEGRO_COLOR DOG_COLOR;
ALLEGRO_COLOR MOUSE_COLOR;

#define INIT_DISPLAY_SUCCESS 0
#define INIT_DISPLAY_ALLEGRO_ERR 1
#define INIT_DISPLAY_ERR 2
#define INIT_DISPLAY_PRIMITIVES_ERR 4

#define FLAG_CAT   0b10000000
#define FLAG_DOG   0b01000000
#define FLAG_MOUSE 0b00100000
#define FLAG_RWALL 0b00001000
#define FLAG_LWALL 0b00000100
#define FLAG_UWALL 0b00000010
#define FLAG_DWALL 0b00000001

typedef struct {
	unsigned short int x, y;
} MOUSE;
typedef struct {
	char state, index;
	unsigned short int x, y;
	unsigned short int power, speed;
	ALLEGRO_COLOR color;
} CAT;
typedef struct {
	unsigned short int x, y;
	unsigned short int speed;
} DOG;

enum MOVEMENT {
    UP,
    DOWN,
    RIGHT,
    LEFT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT
};

enum CHARACTER_TYPE {
	DOG_CHARACTER,
	CAT_CHARACTER,
	MOUSE_CHARACTER
};


#define MOUSE_COUNT 18
#define DOG_COUNT 4
#define CAT_COUNT 4
#define WALL_COUNT 18