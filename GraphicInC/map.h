#pragma once
#include "constants.h"
#include "flagManager.h"
#include <math.h>

CAT cats[CAT_COUNT];
DOG dogs[DOG_COUNT];
MOUSE mouses[MOUSE_COUNT];
short int map[BOARD_SIZE][BOARD_SIZE];

void __initColors() {
	BLUE = al_map_rgb_f(0, 0, 1);
	GREEN = al_map_rgb_f(0, 1, 0);
	RED = al_map_rgb_f(1, 0, 0);
	MAGENTA = al_map_rgb_f(1, 0, 1);
	DOG_COLOR = al_map_rgb(160, 82, 45);
	MOUSE_COLOR = al_map_rgb(0, 0, 139);
}

float __generateRandomMap() {
	// ---- start -- reset board map and modifing two iterators
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) for (j = 0; j < BOARD_SIZE; j++) map[i][j] = 0;
	// ---- prereserve -- the location of all cats
	map[BOARD_SIZE / 2][BOARD_SIZE / 2] = FLAG_CAT;
	// ---- init -- reset to zero variance and change the random seed
	float variance = 0;
	srand(time(NULL));
	// ---- init -- dogs
	for (i = 0; i < DOG_COUNT; i++) {
		dogs[i].speed = 1;
		do {
			dogs[i].x = rand() % (BOARD_SIZE / 2) + (i % 2 ? BOARD_SIZE / 2 : 0);
			dogs[i].y = rand() % (BOARD_SIZE / 2) + (i > 1 ? BOARD_SIZE / 2 : 0);
			if (!map[dogs[i].y][dogs[i].x]) {
				map[dogs[i].y][dogs[i].x] = FLAG_DOG;
				break;
			}
		} while (1);
		variance += dogs[i].x, 2 + dogs[i].y;
	}
	// ---- init -- cats
	{
		CAT t;
		t.speed = 1;
		t.power = 1;
		t.state = 'a';
		t.x = BOARD_SIZE / 2;
		t.y = BOARD_SIZE / 2;
		for (i = 0; i < CAT_COUNT; i++) t.index = i + 1, cats[i] = t;
		cats[0].color = BLUE;
		cats[1].color = GREEN;
		cats[2].color = RED;
		cats[3].color = MAGENTA;
	}
	// ---- init -- mouses
	for (i = 0; i < MOUSE_COUNT; i++) {
		do {
			mouses[i].x = rand() % BOARD_SIZE;
			mouses[i].y = rand() % BOARD_SIZE;
			if (!map[mouses[i].y][mouses[i].x]) {
				map[mouses[i].y][mouses[i].x] = FLAG_MOUSE;
				break;
			}
		} while (1);
		variance += mouses[i].x + mouses[i].y;
	}
	// ---- solve -- variance
	{
		float ave = variance / (DOG_COUNT + MOUSE_COUNT) / 2;
		for (i = 0; i < 4; i++)
			variance += fabs((dogs[i].x - ave) * (dogs[i].y - ave));
		for (i = 0; i < MOUSE_COUNT; i++)
			variance += fabs((mouses[i].x - ave) * (mouses[i].y - ave));
	}
	return variance / (DOG_COUNT + MOUSE_COUNT);
}

char __noWall(int x,int y) {
	if (hasFlag(map[y][x], FLAG_RWALL) ||
		hasFlag(map[y][x], FLAG_LWALL) ||
		hasFlag(map[y][x], FLAG_UWALL) ||
		hasFlag(map[y][x], FLAG_DWALL))
		return 0;
	return 1;
}

void __generateRandomWalls() {
	srand(time(NULL));
	for (int i = 0; i < WALL_COUNT; i++) {
		int x = 1 + rand() % (BOARD_SIZE-2);
		int y = 1 + rand() % (BOARD_SIZE-2);
		if (__noWall(x, y)) {
			switch (rand() % 4) {
			case 0:
				addFlag(&map[y][x], FLAG_RWALL);
				addFlag(&map[y][x + 1], FLAG_LWALL);
				break;
			case 1:
				addFlag(&map[y][x], FLAG_LWALL);
				addFlag(&map[y][x - 1], FLAG_RWALL);
				break;
			case 2:
				addFlag(&map[y][x], FLAG_DWALL);
				addFlag(&map[y + 1][x], FLAG_UWALL);
				break;
			case 3:
				addFlag(&map[y][x], FLAG_UWALL);
				addFlag(&map[y - 1][x], FLAG_DWALL);
				break;
			}
		}
		else i--;
	}
}

void setMap() {
	float variance;
	__initColors();
	do variance = __generateRandomMap();
	while (variance < 25);
	__generateRandomWalls();
}