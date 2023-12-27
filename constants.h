#pragma once
#include <allegro5/allegro_color.h>
#define BOARD_SIZE 15
#define SQUARE_SIZE 60
#define MARGIN 3
#define SCORE_BOARD_WIDTH 500

ALLEGRO_COLOR BLUE;
ALLEGRO_COLOR GREEN;
ALLEGRO_COLOR RED;
ALLEGRO_COLOR MAGENTA;
ALLEGRO_COLOR DOG_COLOR;
ALLEGRO_COLOR MOUSE_COLOR;
ALLEGRO_COLOR COLOR1, COLOR2, COLOR3, COLOR4,WHITE,BLACK;

#define INIT_DISPLAY_SUCCESS 0
#define INIT_DISPLAY_ALLEGRO_ERR 1
#define INIT_DISPLAY_ERR 2
#define INIT_DISPLAY_PRIMITIVES_ERR 3
#define INIT_DISPLAY_FONT_ERR 4
#define INIT_DISPLAY_TRUETYPE_FONT_ERR 5
#define INIT_DISPLAY_FONT_NOT_FOUND_ERR 6
#define INIT_DISPLAY_IMAGE_ADDON_ERR 7
#define INIT_DISPLAY_IMG_NOT_FOUND 8

#define FLAG_CAT   1<<0
#define FLAG_DOG   1<<1
#define FLAG_MOUSE 1<<2
#define FLAG_RWALL 1<<3
#define FLAG_LWALL 1<<4
#define FLAG_UWALL 1<<5
#define FLAG_DWALL 1<<6
#define FLAG_CHOCO 1<<7
#define FLAG_FISH  1<<8

#define MOUSE_COUNT 18
#define DOG_COUNT 4
#define CAT_COUNT 4
#define WALL_COUNT 18
#define CHOCO_COUNT 8
#define FISH_COUNT 10
 
typedef struct {
	unsigned short int x, y;
	unsigned short points;
} MOUSE;
typedef struct {
	char state, index;
	unsigned short int x, y;
	unsigned short int attackPoint, defencePoint, mousePoint;
	ALLEGRO_COLOR color;
} CAT;
typedef struct {
	unsigned short int x, y;
	unsigned short int speed;
	unsigned short int attackPoint, defencePoint;
	char model;
} DOG;

enum MOVEMENT {
	UP,
	DOWN,
	RIGHT,
	LEFT,
	UP_LEFT,
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
	NO_MOVE
};

enum CHARACTER_TYPE {
	DOG_CHARACTER,
	CAT_CHARACTER,
	MOUSE_CHARACTER,
	NO_CHARACTER
};