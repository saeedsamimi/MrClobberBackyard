#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include "save/fileManager.h"
#include <stdio.h>
#include "map.h"

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


void mice_map() {
  int mice_count = 0;
  for(int i=0;i<BOARD_SIZE;i++) {
    for(int j=0;j<BOARD_SIZE;j++) {
      printf("%3d ",map[i][j]);
      if(hasFlag(map[i][j],FLAG_MOUSE)) mice_count++;
    }
    printf("\n");
  }
  printf("MICE COUNT : %d\n\n\n",mice_count);
}

void __testLoadState(ALLEGRO_TEXTLOG *log,gameState* _state) {
	// ---- test ----- test the map
	int row, col;
	for (row = 0; row < BOARD_SIZE; row++) {
		for (col = 0; col < BOARD_SIZE; col++)
			al_append_native_text_log(log, " %-3d", _state->_map[row][col]);
		al_append_native_text_log(log, "\n");
	}
}