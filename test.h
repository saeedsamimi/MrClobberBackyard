#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

// for test and debug board in the console environment
void __testMap() {
  FILE* outFile = fopen("log.txt", "wt");
  fprintf(outFile, "start testing map: \n");
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++)
      fprintf(outFile, " %-3d ", map[i][j]);
    fprintf(outFile, "\n");
  }
  fclose(outFile);
}