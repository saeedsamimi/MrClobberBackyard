#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

// for test and debug board in the console environment
void __testMap() {
  printf("start testing map: \n");
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      printf(" %-3d ", map[i][j]);
    }
    printf("\n");
  }
}